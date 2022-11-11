//tracks plants
#include "Plant.h"
#include "Arduino.h"
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"
#include "Waterer.h"

Waterer waterer;

Plant::Plant() {
    File file = SD.open("Plant Data.csv", FILE_WRITE);
    file.println("time, reservoirWater, plantWaterAvg, didWater, didDrain");
    file.close();
}

void Plant::setWaterIntervalDays(int interval) {
    waterIntervalDays = interval;
}

void Plant::setDrainBetweenWatering(bool YN) { 
    drainBetweenWatering = YN;
}

void Plant::update() {
    if (millis()%(60*60*1000) == 0) {
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

void Plant::saveData(String dataString){
    File file = SD.open("Plant Data.csv", FILE_APPEND);
    file.println(dataString);
    file.close();
}

void Plant::water() {
    waterer.startWater();
    delay(30000);
    waterer.stopWater();
}

void Plant::drain() {
    waterer.startDrain();
    delay(30000);
    waterer.stopDrain();
}

void Plant::logWater() {

}

bool Plant::plantWaterLow() {

}

void Plant::notifyReservoirLow() {

}

bool Plant::reservoirLow(){

}
