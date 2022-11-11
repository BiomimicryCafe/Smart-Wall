#ifndef Waterer_h
#define Waterer_h
#include <Arduino.h>

class Waterer {
    public:
        Waterer();
        void startWater();
        void stopWater();
        void startDrain();
        void stopDrain();
};

#endif