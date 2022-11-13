#ifndef Plant_h
#define Plant_h

#include <Arduino.h>
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"
#include "Waterer.h"
#include "Adafruit_seesaw.h"

class Plant {
    public:
        Plant();
        bool begin();
        void update();
        void setWaterIntervalDays(int interval);
        void setDrainBetweenWatering(bool YN);
        void water();
        void drain();
    private:
        int waterIntervalDays;
        bool drainBetweenWatering;
        void saveData(String dataString);
        void logWater();
        void notifyReservoirLow();
        bool plantWaterLow();
        bool reservoirLow();
        void changeActiveSensor(uint8_t channel);
        int readWater(uint8_t sensor);
};
#endif
