//tracks plants
#include "Plant.h"
#include "Arduino.h"
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"

Plant::Plant() {
    File file = SD.open("Plant Data.csv", FILE_WRITE);
    file.println("time, resivoirWater, plantWaterAvg, didWater, didDrain");
}

void Plant::setWaterIntervalDays(int interval) {
    waterIntervalDays = interval;
}

void Plant::setDrainBetweenWatering(bool YN) { 
    drainBetweenWatering = YN;
}

void Plant::update() {
    if (millis()%60000 == 0) {
        logWater();
    }
    if (millis()%waterIntervalDays*24*60*60*1000 == 0) {
        if(drainBetweenWatering) {
            drain();
        }
        if (!resivoirLow()) {
            water();
        } else {
            notifyResivoirLow();
        }
    }
}

void Plant::water() {

}

void Plant::drain() {

}

void Plant::logWater() {

}

bool Plant::plantWaterLow() {

}

void Plant::notifyResivoirLow() {

}

bool Plant::resivoirLow(){

}