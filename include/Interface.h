#ifndef Interface_h
#define Interface_h

#include <Arduino.h>
#include "TFT_eSPI.h"
class Interface {
    public:
        Interface();
        void showPlantStatsAll();
        void showPlantControlsWaterInterval(int interval);
        void showPlantControlsDrainage(bool YN);
        void showSleepStatsAll();
        void showSettingsTrackSleepYN(bool sleepYN, bool plantYN);
        void showSettingsTrackPlantsYN(bool sleepYN, bool plantYN);
};

#endif