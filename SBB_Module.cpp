#include "Arduino.h"
#include "SBB_Module.h"

/*******************************************

CONSTRUCTORS

********************************************/

SBB_Module::SBB_Module(){

}


SBB_Module::SBB_Module(int addr, int numPositions, int typeModule){
    init(addr,numPositions, typeModule);
}


/********************************************

GLOBAL FUNCTIONS

************************************************/

void SBB_Module::init(int addr, int numPositions, int typeModule){
    _address = addr;
    _numPositions = numPositions;
    _position = 0;
    _moduleType = typeModule;
}

void SBB_Module::updateMode(long clock){
    if(_mode == MODE_WAITING){
        if(clock >= _triggerTime){
            _mode = MODE_TRIGGER;
        }
    } else if(_mode == MODE_ROTATING){
        if(clock >= _endRotationTime){
            _mode = MODE_DELAY;
            _endRotationTime = -1;
            _position = _targetPosition;
            _triggerTime = -1;
        }
    } else if(_mode == MODE_DELAY){
        if(clock >= _endDelayTime){
            _mode = MODE_OPEN;
        }
    }
}




void SBB_Module::setPosition(int pos, int countdown, int delayAfter, long clock){
    _targetPosition = pos;
    _triggerTime = clock+countdown;
    int duration = rotationDuration(_position,_targetPosition);
    _endRotationTime = _triggerTime + duration;
  
    _endDelayTime = _endRotationTime + delayAfter;
    if(delayAfter<0){
        _endRotationTime += delayAfter;
    }
    _mode = MODE_WAITING;
    if(countdown == 0){
        _mode = MODE_TRIGGER;
    }
}

void SBB_Module::setPosition(int pos, long clock){
    setPosition(pos,0,0,clock);
}

/********************************************

HELPER FUNCTIONS

************************************************/

int SBB_Module::rotationDuration(int start, int target){
    int steps = distFlap(start, target);
    int time = floor(steps/(float)_numPositions*TIME_PER_REVOLUTION);
    return time;
  //return max(steps*TIME_PER_FLAP, TIME_MIN);
}

int SBB_Module::distFlap(int start, int target){
   int d;
  if (start<=target) {
    d = target-start;
  } else {
    d = _numPositions-start + target;
  }
  return d; 
}


/********************************************

GETTER

************************************************/
int SBB_Module::position(){
    return _position;
}


int SBB_Module::moduleType(){
    return _moduleType;
}

int SBB_Module::targetPosition(){
    return _targetPosition;
}

int SBB_Module::address(){
    return _address;
}

int SBB_Module::numPositions(){
    return _numPositions;
}


//MODE GETTERS
bool SBB_Module::isOpen(){
    return (_mode == MODE_OPEN);
}

bool SBB_Module::isWaiting(){
    return (_mode==MODE_WAITING); 
}

bool SBB_Module::isTrigger(){
    return (_mode==MODE_TRIGGER);
}

bool SBB_Module::isRotating(){
    return (_mode==MODE_ROTATING);
}
bool SBB_Module::isDelay(){
    return (_mode==MODE_DELAY);
}



/********************************************

SETTER

************************************************/

void SBB_Module::trigger(){
    _mode = MODE_ROTATING;
    
}

void SBB_Module::calibrateRotationTime(int timePerRevolutuion){
    TIME_PER_REVOLUTION = timePerRevolutuion;
}

