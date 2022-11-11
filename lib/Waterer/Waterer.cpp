#include "Waterer.h"
#include "Wire.h"

Waterer::Waterer() {
    Wire.begin();
}

void Waterer::startWater() {
    Wire.beginTransmission(0x0f);
    Wire.write(1);
    Wire.endTransmission(0x0f);
}

void Waterer::stopWater() {
    Wire.beginTransmission(0x0f);
    Wire.write(2);
    Wire.endTransmission(0x0f);
}

void Waterer::startDrain() {
    Wire.beginTransmission(0x0f);
    Wire.write(3);
    Wire.endTransmission(0x0f);
}

void Waterer::stopDrain() {
    Wire.beginTransmission(0x0f);
    Wire.write(4);
    Wire.endTransmission(0x0f);
}