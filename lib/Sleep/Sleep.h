#ifndef Sleep_h
#define Sleep_h
#include <Arduino.h>
class Sleep {
    public:
        Sleep();
        void update();
    private:
        bool track;
        int silenceLevel;
        bool movementDetected();
        int getTemperature();
        int getLightLevel();
        int getSoundLevel();
        void log();
        void logMovement();
        void logSound();
        void LogManualWakeEvent();
        void saveData(String dataString);
};

#endif