#pragma once

#include "rack.hpp"
#include "engine/Engine.hpp"

using namespace rack;
extern Plugin *pluginInstance;

/*                
         Λ        
        ╱░╲       
       ╱░░░Λ      
      ╱░░░╱▒╲     
     ╱░░░╱▒▒▒Λ    
    ╱░░░╱▒▒▒╱▓╲   
   ╱░░░╱▒▒▒╱▓▓▓Λ  
  ╱░░░╱▒▒▒╱▓▓▓╱█╲ 
 ▕░░░▕▒▒▒▕▓▓▓▕███▏
  ╲░░░╲▒▒▒╲▓▓▓╲█╱ 
   ╲░░░╲▒▒▒╲▓▓▓V  
    ╲░░░╲▒▒▒╲▓╱   
     ╲░░░╲▒▒▒V    
      ╲░░░╲▒╱     
       ╲░░░V      
        ╲░╱       
         V        
*/   

float LERP(const float _amount, const float _inA, const float _inB);

struct CKSSRot : SvgSwitch 
{
	CKSSRot() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CKSS_rot_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CKSS_rot_1.svg")));
	}
};

struct HCVModule : Module
{
	float normalizeParameter(float value)
    {
        float temp = value*0.1+0.5;
        return clamp(temp, 0.0, 1.0);
    }

    inline float getModulatedValue(int mainParamIndex, int cvInputIndex, int cvScaleIndex)
    {
        return params[mainParamIndex].getValue() + (inputs[cvInputIndex].getVoltage() * params[cvScaleIndex].getValue());
    }

    inline float getNormalizedModulatedValue(int mainParamIndex, int cvInputIndex, int cvScaleIndex)
    {
        return normalizeParameter(getModulatedValue(mainParamIndex, cvInputIndex, cvScaleIndex));
    }

};

struct HCVModuleWidget : ModuleWidget
{
	std::string skinPath = "";

	void setSkinPath(std::string _path)
    {
        skinPath = _path;
    }

	void createScrews(bool _is4HP = false)
    {
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	    if(!_is4HP) addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	    if(!_is4HP) addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    }

	void initializeWidget(Module* module, bool _is4HP = false)
    {
        setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, skinPath)));
        createScrews(_is4HP);
    }

	void createHCVKnob(int _x, int _y, int _paramID)
    {
        addParam(createParam<Davies1900hBlackKnob>(Vec(_x, _y), module, _paramID));
    }

    void createHCVTrimpot(int _x, int _y, int _paramID)
    {
        addParam(createParam<Trimpot>(Vec(_x, _y), module, _paramID));
    }

    void createInputPort(int _x, int _y, int _paramID)
    {
        addInput(createInput<PJ301MPort>(Vec(_x, _y), module, _paramID));
    }

    void createOutputPort(int _x, int _y, int _paramID)
    {
        addOutput(createOutput<PJ301MPort>(Vec(_x, _y), module, _paramID));
    }

};

struct HCVTriggerGenerator
{
    float time = 0.0;
	float triggerTime = 0.001;
    bool process() 
    {
		time += APP->engine->getSampleTime();
		return time < triggerTime;
	}
    void trigger() 
    {
		// Keep the previous pulseTime if the existing pulse would be held longer than the currently requested one.
        if (time + triggerTime >= triggerTime) 
        {
			time = 0.0;
		}
	}
};

struct TriggerGenWithSchmitt
{
	HCVTriggerGenerator trigGen;
	rack::dsp::SchmittTrigger schmitt;

	bool process(bool _trigger)
	{
		if(schmitt.process(_trigger ? 2.0f : 0.0f)) trigGen.trigger();
		return trigGen.process();
	}
};

struct HysteresisGate
{
	bool state = false;
	float trueBound = 1.0f;
	float falseBound = 0.98f;

	bool process(float input)
	{
		if(input > trueBound) state = true;
		else if(input < falseBound) state = false;
		return state;
	}
	bool getState() {return state;}
};