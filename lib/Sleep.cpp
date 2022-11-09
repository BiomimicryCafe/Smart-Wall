//tracks sleep
#include "Arduino.h"
#include "Sleep.h"
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"
#include <TimeLib.h>

Sleep::Sleep() {
    silenceLevel = getSoundLevel();
    pinMode(WIO_MIC, INPUT);
    File file = SD.open("Sleep Data.csv", FILE_WRITE);
    file.println("time, temp, light, movement, sound");
}

void Sleep::update() {
    //if time is divisible by 5 seconds, if significant motion detected, log motion, log time, else, pass, else, pass
    if (millis()%5000 == 0) {
        if (movementDetected()) {
            logMovement();
        }
    }
    //if time is divisible by 30 minutes, log ambient light, log ambient temperature, log time, else, pass
    if (millis()%30*60*1000 == 0) {
        log();
    }
    if (millis()%200 == 0) {
        if (getSoundLevel() - silenceLevel > 20) {
            log();
        }
    }
}

bool Sleep::movementDetected() {
    return false; //PLACEHOLDER VALUE
}

int Sleep::getTemperature() {

}

int Sleep::getLightLevel() {

}

int Sleep::getSoundLevel() {
    return analogRead(WIO_MIC);
}

void Sleep::LogManualWakeEvent() {
    saveData(String(hour()+1) + "," 
    + String(getTemperature()) + "," 
    + String(getLightLevel()) + "," 
    + String(true) + "," 
    + String(getSoundLevel()));
}

void Sleep::log() {
    saveData(
        String(hour()+1) + "," 
        + String(getTemperature()) + "," 
        + String(getLightLevel()) + "," 
        + String(movementDetected()) + "," 
        + String(getSoundLevel())
    );
}

void saveData(String dataString){
    File file = SD.open("Sleep Data.csv", FILE_APPEND);
    file.println(dataString);
    file.close();
}