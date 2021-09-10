#pragma once

#include "HetrickUtilities.hpp"

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

using namespace rack;


extern Plugin *pluginInstance;

extern Model *modelTwoToFour;
extern Model *modelAnalogToDigital;
extern Model *modelASR;
extern Model *modelBitshift;
extern Model *modelBlankPanel;
extern Model *modelBoolean3;
extern Model *modelComparator;
extern Model *modelContrast;
extern Model *modelCrackle;
extern Model *modelDelta;
extern Model *modelDigitalToAnalog;
extern Model *modelDust;
extern Model *modelExponent;
extern Model *modelFlipFlop;
extern Model *modelFlipPan;
extern Model *modelGateJunction;
extern Model *modelLogicCombine;
extern Model *modelMinMax;
extern Model *modelRandomGates;
extern Model *modelRotator;
extern Model *modelScanner;
extern Model *modelWaveshape;