#ifndef Plant_h
#define Plant_h

#include <Arduino.h>

class Plant {
    public:
        Plant();
        void update();
        void setWaterIntervalDays(int interval);
        void setDrainBetweenWatering(bool YN);
        void water();
        void drain();
    private:
        int waterIntervalDays;
        bool drainBetweenWatering;
        void saveData(string dataString);
        void logWater();
        void notifyResivoirLow();
        bool plantWaterLow();
        bool resivoirLow();
};
#endif
