//tracks plants
#include "Plant.h"
#include "Arduino.h"
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"
#include "Waterer.h"
#include "Adafruit_seesaw.h"

Waterer waterer;
Adafruit_seesaw plant0;
Adafruit_seesaw plant1;
Adafruit_seesaw plant2;
Adafruit_seesaw plant3;
Adafruit_seesaw plant4;
// Adafruit_seesaw plant5;
// Adafruit_seesaw plant6;
// Adafruit_seesaw plant7;
Adafruit_seesaw reservoir;

void Plant::changeActiveSensor(uint8_t channel) {
    if (channel > 7) return;
    Wire.beginTransmission(0x70);
    Wire.write(1 << channel);
    Wire.endTransmission();
}

int Plant::readWater(uint8_t sensor) {
    switch (sensor) {
        case 0:
            changeActiveSensor(0);
            return plant0.touchRead(0);
            break;
        case 1:
            changeActiveSensor(1);
            return plant1.touchRead(0);
            break;
        case 2:
            changeActiveSensor(2);
            return plant2.touchRead(0);
            break;
        case 3:
            changeActiveSensor(3);
            return plant3.touchRead(0);
            break;
        case 4:
            changeActiveSensor(4);
            return plant4.touchRead(0);
            break;
        // case 5:
        //     changeActiveSensor(5);
        //     return plant5.touchRead(0);
        //     break;
        // case 6:
        //     changeActiveSensor(6);
        //     return plant6.touchRead(0);
        //     break;
        // case 7:
        //     changeActiveSensor(7);
        //     return plant7.touchRead(0);
        //     break;
    }  
    return -1;
}

Plant::Plant() {
    pinMode(WIO_BUZZER, OUTPUT);
    analogWrite(WIO_BUZZER, 128);
    delay(100);
    analogWrite(WIO_BUZZER, 0);
    delay(1000);
}

bool Plant::begin() {
    SD.begin();
    File file = SD.open("Plant Data.csv", FILE_WRITE);
    file.println("time, reservoirWater, plantWaterAvg, didWater, didDrain");
    file.close();
    reservoir.begin(0x36);
    Wire.begin();
    changeActiveSensor(0);
    plant0.begin(0x36);
    changeActiveSensor(1);
    plant1.begin(0x36);
    changeActiveSensor(2);
    plant2.begin(0x36);
    changeActiveSensor(3);
    plant3.begin(0x36);
    changeActiveSensor(4);
    plant4.begin(0x36);

    // changeActiveSensor(5);
    // plant5.begin(0x36);
    // changeActiveSensor(6);
    // plant6.begin(0x36);
    // changeActiveSensor(7);
    // plant7.begin(0x36);
    return true;
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
        if (!reservoirLow()) {
            water();
        } else {
            notifyReservoirLow();
        }
    }
}

void Plant::saveData(String dataString) {
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
    return false;
}

void Plant::notifyReservoirLow() {

}

bool Plant::reservoirLow(){
    return false;
}
