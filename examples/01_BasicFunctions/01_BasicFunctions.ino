#include <SBB_FlipFlap.h>

//set txPin
int txPin = 1;

//set lowest addr of module. note: the addresses of the module must be sequential
int addr = 13;

//set number of Modules
int numModules = 9;

//set moduleType if all Modules are the same
//Avaiable Modules:  MODULE_ALPHANUM, MODULE_HOUR, MOUDLE_MINUTE, MODULE_OTHER_40, MODULE_OTHER_62
int moduleType = MODULE_ALPHANUM;

//set moduleType if modules have different types
// int[] moduleType = {MODULE_HOUR, MODULE_MINUTE, MODULE_OTHER_42}

//initalize SBB Object
SBB_FlipFlap sbb(txPin, addr, numModules, moduleType);

int state = 0;

void setup() {

  // IMPORTANT always include before sending commands to modules
  //this sets all Modules to Position 0,
  sbb.initModules();

  //IMPORTANT: ALWAYS INCLUDE AT THE END OF setup() AND loop()!
  //updates all the modules and the clock, send the actual commands to the modules.
  sbb.update();
}

void loop() {
  //Test if all Modules are ready and availabe for new command
  if (sbb.isOpen()) {
    if (state == 0) {

      //Set Position of one Module 
      //parameters: position, module, (optional) delayAfter, (optional) countdown

      sbb.setPosition(1,0);
      sbb.setPosition(2,1,1000);
      sbb.setPosition(3,2,500,500);
      
      state = 1;
    } else if (state == 1) {

      //Set one Module to display a value. Can be a char or a digit
      //parameters: value, module, (optional) delayAfter, (optional) countdown

      sbb.setValue('E',0);
      sbb.setValue(9,1, 1000);
      sbb.setValue('4',2,500,500);
      
      state = 2;
    } else if (state == 2) {

      //Set one Module to random Position
      //parameters: module, (optional) delayAfter, (optional) countdown

      sbb.randomize(0);
      sbb.randomize(1, 1000);
      sbb.randomize(2,500,500);

      state = 3;
    } else if (state == 3) {

      //Move one Module a given stepSize
      //parameters: stepsize, module, (optional) delayAfter, (optional) countdown

      sbb.step(1,0);
      sbb.step(5,1,1000);
      sbb.step(3,2,500,500);

      state = 4;
    } else if (state == 4) {

      //Move one Module to index 0
      //parameters: module, (optional) delayAfter, (optional) countdown

      sbb.zero(0);
      sbb.zero(1,1000);
      sbb.zero(2,500,500);

      state = 0;
    }
  }

  //IMPORTANT: ALWAYS INCLUDE AT THE END OF setup() AND loop()!
  //updates all the modules and the clock, send the actual commands to the modules.
  sbb.update();
}
