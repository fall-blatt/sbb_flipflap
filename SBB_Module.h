#ifndef SBB_Module_h
#define SBB_Module_h

#include "Arduino.h"

class SBB_Module{
    public:
        //CONSTRUCTORS
        SBB_Module();
        SBB_Module::SBB_Module(int addr, int numPositions, int typeModule);
        
        //GLOBAL FUNCTIONS
        void init(int addr, int numPositions, int typeModule);
        void updateMode(long clock);

    
    
        void setPosition(int pos, long clock);
        void setPosition(int pos, int countdown,int delayAfter,long clock);
    
    
        //HELPER FUNCTIONS
        int rotationDuration(int start, int target);
        int distFlap(int start, int target);

            
        // GETTER
        int position();
        int moduleType();       
        int targetPosition();
        int address();
        int numPositions();

        bool isOpen(); 
        bool isWaiting();
        bool isTrigger();
        bool isRotating();
        bool isDelay();
            
            
        //SETTER
        void trigger();            
        void calibrateRotationTime(int timePerRevolutuion);
    
        

    private:
        int _position;
        int _mode = MODE_OPEN;
        int _moduleType;
       
        int _targetPosition;
        long _triggerTime;
        long _endRotationTime;
        long _endDelayTime;
    
        int TIME_PER_REVOLUTION = 3800;
    
        int _address;
        int _numPositions;
        const int MODE_OPEN = 0;
        const int MODE_WAITING = 1;
        const int MODE_TRIGGER = 2;
        const int MODE_ROTATING = 3;
        const int MODE_DELAY = 4;
    
};

#endif