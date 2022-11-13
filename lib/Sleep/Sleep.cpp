//tracks sleep
#include "Arduino.h"
#include "Sleep.h"
#include <SPI.h>
#include <Seeed_FS.h>
#include "SD/Seeed_SD.h"
#include <TimeLib.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_SHT4x.h>
Adafruit_TSL2591 light = Adafruit_TSL2591(2591);
Adafruit_SHT4x temp = Adafruit_SHT4x();

Sleep::Sleep() {
    // pinMode(WIO_MIC, INPUT);
    // silenceLevel = getSoundLevel();
    // File file = SD.open("Sleep Data.csv", FILE_WRITE);
    // file.println("time, temp, light, movement, sound");
    // file.close();
    // light.begin();
    // temp.begin();
    // temp.setPrecision(SHT4X_MED_PRECISION);
    // light.setTiming(TSL2591_INTEGRATIONTIME_600MS);
    // light.setGain(TSL2591_GAIN_MED);
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
    sensors_event_t humidity, temperature;
    temp.getEvent(&humidity, &temperature);
    return temperature.temperature;
}

int Sleep::getLightLevel() {
    return light.getLuminosity(TSL2591_VISIBLE);
}

int Sleep::getSoundLevel() {
    return analogRead(WIO_MIC);
}

void Sleep::saveData(String dataString){
    File file = SD.open("Sleep Data.csv", FILE_APPEND);
    file.println(dataString);
    file.close();
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

