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

int indexLetter = 0;
String text = "Hallo Zusammen was machen wir";

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

      //Display String (start together)
      //parameters: text, (optional) delayBetween, (optional) startmodule, (optional) textlength, (optional) delayAfter, (optional) countdown
      //textlenght = -1 means unlimited/full display. unused modules get set to empty space ' '
      //if textlenght is limited only the modules in rage get updated, all others stay the same as they were

      sbb.setWord("HelloWorld", 0, 0, -1, 2000);
      //sbb.setWord("HelloWorld", 0, 3, -1, 2000, 0);
      //sbb.setWord("HelloWorld", 300, 0, -1, 2000, 0);
      //sbb.setWord("HelloWorld", 0, 2, 4, 2000, 0);
      //sbb.setWord("HelloWorld", 0, 0, -1, 1500, 500);

      state = 1;
    } else if (state == 1) {
      //Display String (end together) this is the same as setWord, but the modules are timed to start, so they display the result together.
      //parameters: text, (optional) delayBetween, (optional) startmodule, (optional) textlength, (optional) delayAfter, (optional) countdown
      //textlenght = -1 means unlimited/full display. unused modules get set to empty space ' '
      //if textlenght is limited only the modules in rage get updated, all others stay the same as they were

      sbb.setWordTimed("ABCDEFGHI", 0, 0, -1, 2000, 0);
      //sbb.setWordTimed("ABCDEFGHI", 0, 3, -1, 2000, 0);
      //sbb.setWordTimed("ABCDEFGHI", 300, 0, -1, 2000, 0);
      //sbb.setWordTimed("ABCDEFGHI", 0, 2, 4, 2000, 0);
      //sbb.setWordTimed("ABCDEFGHI", 0, 0, -1, 1500, 500);
      
      state = 2;
    } else if (state == 2) {
      //Display long Text in parts. returns position in text in variable indexLetter
      //parameters: text, indexLetter, (optional) linebreak, (optional) delayBetween, (optional) startModule, (optional) textLength, (optional) delayAfter, (optional) countdown
      //indexLetter gets updated with new index after displaying text
      //linebreak: if linebreak = true, setText tries to not cut words in half at the end of the line

      sbb.setText(text, indexLetter, true, 0, 0, -1, 2000);
      //sbb.setText(text, indexLetter, false, 0, 0, -1, 2000);

      state = 0;
    }
  }

  //IMPORTANT: ALWAYS INCLUDE AT THE END OF setup() AND loop()!
  //updates all the modules and the clock, send the actual commands to the modules.
  sbb.update();
}
