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

      //Set all Modules to positon 0
      //parameters: (otpional) delaybetween, (optional) delayAfter, (optional) countdown

      //sbb.zeroAll();
      sbb.zeroAll(0, 1000);
      //sbb.zeroAll(20,1000,0);
      //sbb.zeroAll(40,1000,500);

      state = 1;
    } else if (state == 1) {

      //Set all Modules to positon that displays a '0'
      //parameters: (otpional) delaybetween, (optional) delayAfter, (optional) countdown

      //sbb.zeroAll();
      sbb.zeroAll(0, 1000);
      //sbb.zeroAll(20,1000,0);
      //sbb.zeroAll(40,1000,500);


      state = 2;
    } else if (state == 2) {

      //Set all Modules to a random Position
      //parameters: (otpional) delaybetween, (optional) delayAfter, (optional) countdown

      //sbb.randomizeAll();
      sbb.randomizeAll(0, 1000);
      //sbb.randomizeAll(20,1000,0);
      //sbb.randomizeAll(40,1000,500);

      state = 3;
    } else if (state == 3) {

      //Move all Modules a given stepsize forward
      //parameters: (optional) stepsize, (otpional) delaybetween, (optional) delayAfter, (optional) countdown

      //sbb.stepAll();
      //sbb.stepAll(4, 0, 1000, 0);
      sbb.stepAll(12, 200, 1000, 0);
      //sbb.stepAll(2, 20, 1000, 500);

      state = 0;
    }
  }

  //IMPORTANT: ALWAYS INCLUDE AT THE END OF setup() AND loop()!
  //updates all the modules and the clock, send the actual commands to the modules.
  sbb.update();
}
