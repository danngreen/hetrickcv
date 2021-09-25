#include "HetrickCV.hpp"
#include "dsp/digital.hpp"

/*                         
 ▶─────┐            ┌─────▶
       │┌──────────┐│      
       ││  2 to 4  │├─────▶
       │└──────────┘│      
       ├─────▶──────┤      
       │            │      
       │            ├─────▶
       │            │      
 ▶─────┘            └─────▶
*/                         

struct TwoToFour : Module
{
	enum ParamIds
	{
		NUM_PARAMS
	};
	enum InputIds
	{
        INA_INPUT,
        INB_INPUT,
		NUM_INPUTS
	};
	enum OutputIds
	{
        OUT1_OUTPUT,
        OUT2_OUTPUT,
        OUT3_OUTPUT,
        OUT4_OUTPUT,
		NUM_OUTPUTS
	};

	enum LightIds
    {
        OUT1_POS_LIGHT, OUT1_NEG_LIGHT,
		OUT2_POS_LIGHT, OUT2_NEG_LIGHT,
		OUT3_POS_LIGHT, OUT3_NEG_LIGHT,
		OUT4_POS_LIGHT, OUT4_NEG_LIGHT,
        NUM_LIGHTS
	};

    float outs[4] = {};

	TwoToFour()
	{
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	void process(const ProcessArgs &args) override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - dataToJson, dataFromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - reset, randomize: implements special behavior when user clicks these from the context menu
};


void TwoToFour::process(const ProcessArgs &args)
{
    const float inA = inputs[INA_INPUT].getVoltage();
    const float inB = inputs[INB_INPUT].getVoltage();

    outs[0] = inA + inB;
    outs[1] = outs[0] * -1.0f;
    outs[3] = inA - inB;
    outs[2] = outs[3] * -1.0f;

    outputs[OUT1_OUTPUT].setVoltage(outs[0]);
    outputs[OUT2_OUTPUT].setVoltage(outs[1]);
    outputs[OUT3_OUTPUT].setVoltage(outs[2]);
	outputs[OUT4_OUTPUT].setVoltage(outs[3]);

	lights[OUT1_POS_LIGHT].setSmoothBrightness(fmaxf(0.0, outs[0] / 5.0), 10);
    lights[OUT1_NEG_LIGHT].setSmoothBrightness(fmaxf(0.0, -outs[0] / 5.0), 10);

    lights[OUT2_POS_LIGHT].setSmoothBrightness(fmaxf(0.0, outs[1] / 5.0), 10);
    lights[OUT2_NEG_LIGHT].setSmoothBrightness(fmaxf(0.0, -outs[1] / 5.0), 10);

    lights[OUT3_POS_LIGHT].setSmoothBrightness(fmaxf(0.0, outs[2] / 5.0), 10);
    lights[OUT3_NEG_LIGHT].setSmoothBrightness(fmaxf(0.0, -outs[2] / 5.0), 10);

    lights[OUT4_POS_LIGHT].setSmoothBrightness(fmaxf(0.0, outs[3] / 5.0), 10);
    lights[OUT4_NEG_LIGHT].setSmoothBrightness(fmaxf(0.0, -outs[3] / 5.0), 10);
}

struct TwoToFourWidget : ModuleWidget { TwoToFourWidget(TwoToFour *module); };

TwoToFourWidget::TwoToFourWidget(TwoToFour *module)
{
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		auto *panel = new SvgPanel();
		panel->box.size = box.size;
		panel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/2To4.svg")));
		addChild(panel);
	}

	addChild(createWidget<ScrewSilver>(Vec(15, 0)));
	addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 0)));
	addChild(createWidget<ScrewSilver>(Vec(15, 365)));
	addChild(createWidget<ScrewSilver>(Vec(box.size.x - 30, 365)));

    //////PARAMS//////

    //////INPUTS//////
    addInput(createInput<PJ301MPort>(Vec(10, 100), module, TwoToFour::INA_INPUT));
    addInput(createInput<PJ301MPort>(Vec(55, 100), module, TwoToFour::INB_INPUT));

    for(int i = 0; i < 4; i++)
    {
        const int yPos = i*45;
        addOutput(createOutput<PJ301MPort>(Vec(33, 150 + yPos), module, TwoToFour::OUT1_OUTPUT + i));
        addChild(createLight<SmallLight<GreenRedLight>>(Vec(70, 158 + yPos), module, TwoToFour::OUT1_POS_LIGHT + i*2));
    }
}

Model *modelTwoToFour = createModel<TwoToFour, TwoToFourWidget>("2To4");

