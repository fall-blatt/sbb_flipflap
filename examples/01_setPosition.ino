#include <SBB_FlipFlap.h>

//set txPin
int txPin = 1;

//set lowest addr of module. note: the addresses of the module must be sequential
int addr = 4;

//set number of Modules
int numModules = 54;

//set moduleType if all Modules are the same
//Avaiable Modules:  MODULE_ALPHANUM, MODULE_HOUR, MOUDLE_MINUTE, MODULE_OTHER_40, MODULE_OTHER_62
int moduleType = MODULE_ALPHANUM;

//set moduleType if modules have different types
// int[] moduleType = {MODULE_HOUR, MODULE_MINUTE, MODULE_OTHER_42}

//initalize SBB Object
SBB_FlipFlap sbb(txPin, addr, numModules, moduleType);


void setup() {

  // IMPORTANT always include before sending commands to modules
  //this sets all Modules to Position 0,
  sbb.initModules();

  //IMPORTANT: ALWAYS INCLUDE AT THE END OF setup() AND loop()!
  //updates all the modules and the clock, send the actual commands to the modules.
  sbb.update();
}

void loop() {
  //get a new position every quarter second
  int pos = (millis() / 250) % 40;

  //set Position of first Module to pos
  sbb.setPosition(pos, 0);
  //set Position of second Module to pos, and show it for 2s (2000ms)
  sbb.setPosition(pos, 1, 2000);
  //set Position of third Module to pos, and show it for 2s (2000ms), also wait 500ms until sending command
  sbb.setPosition(pos, 2, 2000, 500);

  //IMPORTANT: ALWAYS INCLUDE AT THE END OF setup() AND loop()!
  //updates all the modules and the clock, send the actual commands to the modules.
  sbb.update();
}