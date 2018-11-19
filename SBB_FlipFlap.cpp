#include "Arduino.h"
#include "SBB_FlipFlap.h"


/*******************************************

  CONSTRUCTORS

********************************************/

SBB_FlipFlap::SBB_FlipFlap(int txPin, int startAddr, int numModules) {
  _txPin = txPin;
  _startAddr = startAddr;
  _numModules = numModules;
  _modules = new SBB_Module[_numModules];
  for (int i = 0; i < _numModules; i++) {
    //SBB_Module tempModule(_startAddr+i,40);
    _modules[i].init(_startAddr + i, 40, MODULE_ALPHANUM);
  }

  Serial.begin(BAUD_RATE);

}



SBB_FlipFlap::SBB_FlipFlap(int txPin, int startAddr, int numModules, int moduleType[]) {
  _txPin = txPin;
  _startAddr = startAddr;
  _numModules = numModules;
  _modules = new SBB_Module[_numModules];
  for (int i = 0; i < _numModules; i++) {
    //SBB_Module tempModule(_startAddr+i,40);
    int numPosTemp = 62;
    int moduleTypeTemp = moduleType[i];
    if (moduleTypeTemp == MODULE_OTHER_40 ||
        moduleTypeTemp == MODULE_HOUR ||
        moduleTypeTemp == MODULE_ALPHANUM) {
      numPosTemp = 40;
    }
    _modules[i].init(_startAddr + i, numPosTemp, moduleTypeTemp);
  }

  Serial.begin(BAUD_RATE);

}

SBB_FlipFlap::SBB_FlipFlap(int txPin, int startAddr, int numModules, int moduleType) {
  _txPin = txPin;
  _startAddr = startAddr;
  _numModules = numModules;
  _modules = new SBB_Module[_numModules];
  for (int i = 0; i < _numModules; i++) {
    //SBB_Module tempModule(_startAddr+i,40);
    int numPosTemp = 62;
    int moduleTypeTemp = moduleType;
    if (moduleTypeTemp == MODULE_OTHER_40 ||
        moduleTypeTemp == MODULE_HOUR ||
        moduleTypeTemp == MODULE_ALPHANUM) {
      numPosTemp = 40;
    }
    _modules[i].init(_startAddr + i, numPosTemp, moduleType);
  }

  Serial.begin(BAUD_RATE);

}



/*******************************************

  GLOBAL FUNCTIONS

********************************************/



void SBB_FlipFlap::initModules(int del) {
  delay(500);
  startCommand(CMD_ZERO, 0);
  delay(del);
}


void SBB_FlipFlap::update() {
  _clock = millis();

  bool isSending = false;
  for (int  i = 0; i < _numModules; i++) {
    _modules[i].updateMode(_clock);

    if (_modules[i].isTrigger()) {
      if (!isSending) {
        sendPosition(_modules[i].targetPosition(), _modules[i].address());
      } else {
        sendValue(_modules[i].targetPosition());
      }
      _modules[i].trigger();
      isSending = true;
    } else {
      isSending = false;
    }
  }
  _clock = millis();
}




/*******************************************

  SINGLE MODULE FUNCTIONS

********************************************/


void SBB_FlipFlap::setPosition(int p, int m, int delayAfter, int countdown) {
  if (_modules[m].isOpen()) {
    _modules[m].setPosition(p, countdown, delayAfter, _clock);
  }
}


void SBB_FlipFlap::setValue(int val, int m, int delayAfter, int countdown) {
  if (_modules[m].isOpen()) {
    //lookup Value depending on Module Type
    int pos = valueToPosition(val,m);
      setPosition(pos, m, countdown, delayAfter);

  }
}


void SBB_FlipFlap::setValue(char val, int m, int delayAfter, int countdown) {
  if (_modules[m].isOpen()) {
    //lookup Value depending on Module Type
      int pos = valueToPosition(val,m);
      setPosition(pos, m, countdown, delayAfter);

  }
}


void SBB_FlipFlap::step(int stepSize, int m, int delayAfter, int countdown) {
  int pos = (_modules[m].position() + stepSize) % _modules[m].numPositions();
  setPosition(pos, m, countdown, delayAfter);
}


  void SBB_FlipFlap::randomize(int m, int delayAfter, int countdown){
    int pos = floor(random(_modules[m].numPositions()));
    setPosition(pos,m,countdown,delayAfter);
  }


  void SBB_FlipFlap::zero(int m, int delayAfter, int countdown){
    setPosition(0,m,countdown,delayAfter);
  }



/*******************************************

  MULTIPLE MODULE FUNCTIONS

********************************************/

  
void SBB_FlipFlap::setWord(String w,  int delayBetween, int m, int numModules, int delayAfter,int countdown) {
    int worldLength = w.length();
    int addrLength = numModules;
    if(addrLength == -1){
        addrLength = _numModules;
    }

    for (int i = 0; i < addrLength; i++) {
        if (worldLength > i) {
            setValue(w.charAt(i),i+m,countdown+i*delayBetween,delayAfter);
        } else {
            char filler = ' ';
            setValue(filler,i+m,countdown+i*delayBetween,delayAfter);
        }

    }
}


void SBB_FlipFlap::setText(String text, int part, bool lineBreak, int delayBetween,  int m, int numModules, int delayAfter, int countdown){
    int textLength = text.length();

    String subString = text.substring(part*numModules,(part+1)*numModules);
    //String subString = text.substring(0,18);
    setWord(subString, m,numModules,delayBetween,countdown,delayAfter);
    
    int nextPart = part+1;
    
    if(nextPart*numModules>textLength){
        nextPart = 0;
    }
    
    
}

  
void SBB_FlipFlap::randomizeAll( int delayBetween, int delayAfter, int countdown) {

    for (int i = 0; i < _numModules; i++) {
        int pos = floor(random(_modules[i].numPositions()));
        setPosition(pos,i,countdown+i*delayBetween,delayAfter);
    }
}



  void SBB_FlipFlap::setPositions(int p[], int pSize, int delayBetween, int m, int delayAfter,int countdown){
    for(int i = 0; i< pSize ;i++){
        int index = i+ m;
        if(index<_numModules){
            setPosition(p[i],index,countdown+i*delayBetween,delayAfter);
        }       
    }
  }

  void SBB_FlipFlap::stepAll(int stepSize, int delayBetween, int delayAfter, int countdown){
    for (int i = 0; i < _numModules; i++) {
        step(stepSize,i,countdown+i*delayBetween,delayAfter);
    }
  }

  void SBB_FlipFlap::zeroAll(int delayBetween, int delayAfter, int countdown){
    for (int i = 0; i < _numModules; i++) {
        setPosition(0,i,countdown+i*delayBetween,delayAfter);
    }
  }

    void SBB_FlipFlap::zeroAllValue(int delayBetween, int delayAfter, int countdown){
    for (int i = 0; i < _numModules; i++) {
        setValue(0,i,countdown+i*delayBetween,delayAfter);
    }
  }


  void SBB_FlipFlap::fillRow(int p, int num, int dir, int m, int delayBetween,int countdown, int delayAfter){
    if(dir >=0){ //FORWARD
        for(int i = 0; i<num;i++){
            if(m + i < _numModules){
            setPosition(p,m+i,countdown+i*delayBetween,delayAfter);
            }
        }
    } else {
        for(int i = 0; i<num;i++){
            if(m + i < _numModules){
                setPosition(p,m+i,countdown+(num-i)*delayBetween,delayAfter);
            }
        }
    }
  }

  void SBB_FlipFlap::fillRowForward(int p, int num, int m, int delayBetween,int countdown, int delayAfter){
    fillRow(p,num,FORWARD,m,delayBetween,countdown,delayAfter);
  }
  void SBB_FlipFlap::fillRowBackward(int p, int num, int m, int delayBetween,int countdown, int delayAfter){
    fillRow(p,num,BACKWARD,m,delayBetween,countdown,delayAfter);
  }

void SBB_FlipFlap::fillRow(char c, int num, int dir, int m, int delayBetween,int countdown, int delayAfter){
    if(dir >=0){ //FORWARD
        for(int i = 0; i<num;i++){
            if(m + i < _numModules){
            setValue(c,m+i,countdown+i*delayBetween,delayAfter);
            }
        }
    } else {
        for(int i = 0; i<num;i++){
            if(m + i < _numModules){
                setValue(c,m+i,countdown+(num-i)*delayBetween,delayAfter);
            }
        }
    }
  }

  void SBB_FlipFlap::fillRowForward(char c, int num, int m, int delayBetween,int countdown, int delayAfter){
    fillRow(c,num,FORWARD,m,delayBetween,countdown,delayAfter);
  }
  void SBB_FlipFlap::fillRowBackward(char c, int num, int m, int delayBetween,int countdown, int delayAfter){
    fillRow(c,num,BACKWARD,m,delayBetween,countdown,delayAfter);
  }


    void SBB_FlipFlap::fillRowStepsize(char c, int flapLetters, int dir, int m, int numModules, int countdown, int delayAfter){
        int previousLetterPos = _modules[m].position();
        int nextLetter = c;
        int nextLetterPos = valueToPosition(c,m);
        int del = _modules[m].rotationDuration(previousLetterPos,nextLetterPos);
        del = floor(del/(float)flapLetters);
        fillRow(c,numModules,dir,m,del,countdown,delayAfter);
    }


  /********************************************

  OTHER FUNCTIONS

**********************************************/

  void SBB_FlipFlap::changeAddress(int oldAddr, int newAddr){
    startCommand(CMD_CHANGE_ADDR);
    setAddress(oldAddr);
    setAddress(newAddr);
  }




  /********************************************

  GETTER

**********************************************/
int SBB_FlipFlap::numPositions(int m) {
  return _modules[m].numPositions();
}

bool SBB_FlipFlap::isOpen() {
  for (int i = 0; i < _numModules; i++) {
    if (!_modules[i].isOpen()) {
      return false;
    }
  }
  return true;
}

bool SBB_FlipFlap::isOpen(int m) {
  return _modules[m].isOpen();
}

bool SBB_FlipFlap::isOpen(int m, int num) {
  int n = m + num;
  if (n > _numModules) n = _numModules;
  for (int i = m; i < n; i++) {
    if (!_modules[i].isOpen()) {
      return false;
    }
  }
  return true;
}




/********************************************

  HELPER FUNCTIONS

**********************************************/


void SBB_FlipFlap::sendPosition(int p, int a) {
  startCommand(CMD_POSITION, a);
  Serial.write(p);
  delay(DELAY_BETWEEN_SEND);
}


void SBB_FlipFlap::startCommand(byte cmd) {
  brk();
  Serial.write(0xff);
  Serial.write(cmd);
}

void SBB_FlipFlap::startCommand(byte cmd, int addr) {
  brk();
  Serial.write(0xff);
  Serial.write(cmd);
  setAddress(addr);
}

void SBB_FlipFlap::setAddress(int addr) {
  Serial.write(addr);
}

void SBB_FlipFlap::sendValue(int val) {
  Serial.write(val);
  delay(DELAY_BETWEEN_SEND);

}

void SBB_FlipFlap::brk() {
  Serial.flush();
  Serial.end();
  pinMode(_txPin, OUTPUT);
  digitalWrite(_txPin, LOW);
  delay(10);
  Serial.begin(BAUD_RATE);
}


/********************************************

  LOOKUP FUNCTIONS

**********************************************/


int SBB_FlipFlap::valueToPosition(char l, int m){
    int pos = 0;
    if(_modules[m].moduleType() == MODULE_ALPHANUM){
        pos = lookupAlphaNum(l);
    } else if(_modules[m].moduleType() == MODULE_HOUR){
        pos = lookupHour(l);
    } else if(_modules[m].moduleType() == MODULE_MINUTE){
        pos = lookupMinute(l);
    }
    return pos;
}


int SBB_FlipFlap::valueToPosition(int l, int m){
    int pos = 0;
    if(_modules[m].moduleType() == MODULE_ALPHANUM){
        pos = lookupAlphaNum(l);
    } else if(_modules[m].moduleType() == MODULE_HOUR){
        pos = lookupHour(l);
    } else if(_modules[m].moduleType() == MODULE_MINUTE){
        pos = lookupMinute(l);
    }
    return pos;
}

int SBB_FlipFlap::lookupAlphaNum(char l) {
  int pos = 39; //default to space
  if ((l >= 65 && l <= 90)) { //uppercase Letters
    pos = l - 65;
  } else if ((l >= 97 && l <= 122)) { //lowercase Letter
    pos = l - 97;
  } else if ((l >= 49 && l <= 57) ) { //numbers 1-9
    pos = l - 21;
  } else if (l == 45) { // -
    pos = 27;
  } else if (l == 46) { // .
    pos = 38;
  } else if (l == 47) { // /
    pos = 26;
  } else if (l == 48) { // 0
    pos = 37;
  }
  return pos;
}

int SBB_FlipFlap::lookupAlphaNum(int n) {
  int pos = 39;   //default to space
  if (n >= 1 && n <= 9) {
    pos = n + 27;
  } else if (n == 0) {
    pos = 37;
  }
  return pos;
}

int SBB_FlipFlap::lookupHour(int n) {
  int pos = 29 ;
  if (n <= 23 && n >= 0 ) {
    pos = n;
  }
  return pos;
}

int SBB_FlipFlap::lookupHour(char l) {
  int pos = -1;
if (l >= 48 && l <= 57) {
        pos = lookupHour(l - 48);
      }
  return pos;
}

int SBB_FlipFlap::lookupMinute(int n) {
  int pos = 29 ;
  if (n < 31 && n >= 0) {
    pos = n + 30;

  } else if (n < 59 && n >= 0) {
    pos = n - 31;
  }
  return pos;
}

int SBB_FlipFlap::lookupMinute(char l) {
  int pos = -1 ;
   if (l >= 48 && l <= 57) {
        pos = lookupMinute(l - 48);
      }
  return pos;
}



