#ifndef SBB_SplitFlap_h
#define SBB_SplitFlap_h

#include "Arduino.h"
#include "SBB_Module.h"

class SBB_FlipFlap
{
  public:
    //CONSTRUCTORS
    /** Brief description which ends at this dot. Details follow
     *  here.
     */
    SBB_FlipFlap(int txPin, int startAddr, int numModules   ,HardwareSerial& serial = Serial);
    SBB_FlipFlap(int txPin, int startAddr, int numModules,int moduleType[],HardwareSerial& serial = Serial);
    SBB_FlipFlap(int txPin, int startAddr, int numModules, int moduleType, HardwareSerial& serial = Serial);

    //GLOBAL FUNCTIONS
    void initModules(int del = 4000);
    void update();
    void updateBlock(int _blocksize = 18);



    //SINGLE MODULE FUNCTIONS
    /** Brief description which ends at this dot. Details follow
     *  here.
     */
    void setPosition(int p, int m,  int delayAfter = 0, int countdown = 0);
    void setValue(int val, int m,  int delayAfter = 0,int countdown = 0);
    void setValue(char val, int m, int delayAfter = 0,int countdown = 0 );

    void step(int stepSize, int m,  int delayAfter = 0, int countdown = 0);

    void randomize(int m,  int delayAfter = 0, int countdown = 0);
    void zero(int m, int delayAfter = 0, int countdown = 0);


    //MULTIPLE MODULE FUNCTIONS
    void setWord(String w, int delayBetween = 0, int m = 0, int numModules = -1,  int delayAfter = 0, int countdown = 0);

    void setText(String &text, int &indexLetter, bool lineBreak = false, int delayBetween = 0, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);

    void setWordTimed(String w, int delayBetween = 0, int m = 0, int numModules = -1,  int delayAfter = 0, int countdown = 0);

    void randomizeAll( int delayBetween = 0,  int delayAfter = 0,  int countdown = 0);
    void setPositions(int p[], int pSize, int delayBetween, int m = 0, int delayAfter = 0, int countdown = 0);
    void stepAll(int stepSize = 1, int delayBetween = 0, int delayAfter = 0, int countdown = 0);
    void zeroAll(int delayBetween = 0, int delayAfter = 0, int countdown = 0);
    void zeroAllValue(int delayBetween = 0, int delayAfter = 0, int countdown = 0);



    //void fillRow(int p, int num, int dir, int m, int delayBetween,int countdown, int delayAfter);
    void fillRow(int p, int dir, int delayBetween, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);
    void fillRowForward(int p, int delayBetween, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);
    void fillRowBackward(int p, int delayBetween, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);

    void fillRow(char c, int dir, int delayBetween, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);
    void fillRowForward(char c, int delayBetween, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);
    void fillRowBackward(char c, int delayBetween, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);

    //void fillRowStepsize(int p, int flapLetters, int dir, int m, int numModules, int countdown, int delayAfter);
    void fillRowStepsize(int p, int dir, int flapLetters, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);
    void fillRowStepsize(char c, int dir, int flapLetters, int m = 0, int numModules = -1, int delayAfter = 0, int countdown = 0);

    //OTHER FUNCTIONS
    void changeAddress(int oldAddr, int newAddr);



    //GETTER
    int numPositions(int m);

    bool isOpen();
    bool isOpen(int m);
    bool isOpen(int m, int num);

    //SETTER
    void calibrateDelayBetweenSend(int del);

    //HELPER FUNCTIONS

    void sendPosition(int p, int a);
    void brk();


    void startCommand(byte cmd);
    void startCommand(byte cmd, int addr);
    void setAddress(int addr);
    void sendValue(int val);
    
    int findSpace(String &s, int pos, int index);
    
    int getDuration(int indexModule, int targetPos);
    int getDuration(int indexModule, char targetValue);

    // LOOKUP FUNCTIONS
    int valueToPosition(char l, int m);
    int valueToPosition(int l, int m);

    int lookupAlphaNum(char l);  
    int lookupAlphaNum(int n);  
    int lookupHour(int n);
    int lookupHour(char l);
    int lookupMinute(int n);
    int lookupMinute(char l);

    
  private:
  
    HardwareSerial&  serial;
    int _startAddr;
    int _numModules;
    int _txPin;
    
    long _clock = 0;
    
    SBB_Module *_modules;


    int DELAY_BETWEEN_SEND = 10;
    const int BAUD_RATE = 19200;
    const byte CMD_POSITION = 0xc0;
    const byte CMD_ZERO = 0xc5;
    const byte CMD_STEP = 0xc6;
    const byte CMD_PULSE = 0xc7;
    const byte CMD_CHANGE_ADDR = 0xce;
    
    
};


const int FORWARD = 1;
const int BACKWARD = -1;

const int MODULE_OTHER_40 = 0;
const int MODULE_OTHER_62 = 1;
const int MODULE_ALPHANUM = 2;
const int MODULE_HOUR = 3;
const int MODULE_MINUTE = 4;





#endif
