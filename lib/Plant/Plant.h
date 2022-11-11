#ifndef Plant_h
#define Plant_h

#include <Arduino.h>

class Plant {
    public:
        Plant();
        void update();
        void setWaterIntervalDays(int interval);
        void setDrainBetweenWatering(bool YN);
    private:
        int waterIntervalDays;
        bool drainBetweenWatering;
        void water();
        void drain();
        void logWater();
        void notifyResivoirLow();
        bool plantWaterLow();
        bool resivoirLow();
};
#endif