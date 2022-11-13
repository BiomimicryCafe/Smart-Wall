#ifndef Interface_h
#define Interface_h

#include <Arduino.h>
class Interface {
    public:
        Interface();
        bool begin();
        void showPlantStatsAll();
        void showPlantControlsWaterInterval(int interval);
        void showPlantControlsDrainage(bool YN);
        void showSleepStatsAll();
        void showSettingsTrackSleepYN(bool sleepYN, bool plantYN);
        void showSettingsTrackPlantsYN(bool sleepYN, bool plantYN);
        void showError(String error);
};

#endif