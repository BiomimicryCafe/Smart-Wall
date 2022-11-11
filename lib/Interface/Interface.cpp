//draws things to the screen
#include <Arduino.h>
#include "Interface.h"
#include "TFT_eSPI.h"
#include "seeed_line_chart.h"
#include "CSV_Parser.h"

//create tft lib object
TFT_eSPI tft;
TFT_eSprite spr = TFT_eSprite(&tft);

#define plant_data_max_size 168 //number of hours in a week

Interface::Interface() {
    tft.begin();
    tft.setRotation(3);
    spr.createSprite(TFT_HEIGHT, TFT_WIDTH);
    spr.setRotation(3);
}

void Interface::showPlantStatsAll() {
    spr.fillSprite(TFT_WHITE);
    CSV_Parser cp(/*format*/ "LLLLL", /*has_header*/ true, /*delimiter*/ ',');
    // The line below (readSDfile) wouldn't work if SD.begin wasn't called before.
    // readSDfile can be used as conditional, it returns 'false' if the file does not exist.
    if (cp.readSDfile("/Plant Data.csv")) {
        int *plantTimeData = (int*)cp["time"];
        int *plantResivoirWaterData = (int*)cp["resivoirWater"];
        int *plantDidWater = (int*)cp["didWater"];
        int *plantDidDrain = (int*)cp["didDrain"];
        int *plantWaterLevel = (int*)cp["plantWaterAvg"];
        doubles plantTime;
        while (plantTime.size() <= plant_data_max_size) {
            plantTime.push(plantTimeData[(sizeof(plantTimeData) - plantTime.size())]); //I mean this might work, it might be backwards
        }

        auto header = text(0, 0)
            .value("Plant Stats")
            .align(center)
            .valign(vcenter)
            .width(spr.width())
            .thickness(2);
        
        header.height(header.font_height(&spr) * 2);
        header.draw(&spr); //headerb height is twice the height of the font

        auto content = line_chart(20, header.height());
        content //x, y where the graph begins
            .height(spr.height() - header.height() * 1.5) // actual height of the line chart
            .width(spr.width() - content.x() * 2)         // actual width of the line chart
            .based_on(0.0)                                // Starting point of y-axis, must be a float
            .show_circle(false)                           // drawing a cirle at each point, default is on.
            .value(plantTime)                             // passing through the data to line graph
            .max_size(plant_data_max_size)
            .color(TFT_GREEN)                             // Setting the color for the line
            .backgroud(TFT_WHITE)
            .draw(&spr);
    }
    spr.pushSprite(0,0);
}

void Interface::showPlantControlsWaterInterval(int interval) {
    
}

void Interface::showPlantControlsDrainage(bool YN) {

}

void Interface::showSleepStatsAll() {

}

void Interface::showSettingsTrackSleepYN(bool sleepYN, bool plantYN) {

}

void Interface::showSettingsTrackPlantsYN(bool sleepYN, bool plantYN) {

}