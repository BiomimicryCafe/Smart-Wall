#include <Arduino.h>
#include "Sleep.h"
#include "Plant.h"
#include "Interface.h"
#include <Adafruit_APDS9960.h>
#include "SD/Seeed_SD.h"
Adafruit_APDS9960 gesture;
Interface interface;
Sleep sleep;
Plant plant;

bool isAsleep = false;
bool trackSleep = true;
bool carePlants = true;
int waterIntervaldays = 7;
bool drainPlants = true;
int currentScreen = 0;
unsigned long timeStartMillis = 0;
unsigned int sleepTimer = 0;

char checkInputs(){
  uint8_t gestureReading = gesture.readGesture();
  if(digitalRead(WIO_KEY_C) == LOW) {
    return 'A';
  } else if(digitalRead(WIO_KEY_B) == LOW) {
    return 'B';
  } else if(digitalRead(WIO_KEY_A) == LOW) {
    return 'C';
  } else if(digitalRead(WIO_5S_UP) == LOW || gestureReading == APDS9960_UP) {
    return 'U';
  } else if(digitalRead(WIO_5S_DOWN) == LOW || gestureReading == APDS9960_DOWN) {
    return 'D';
  } else if(digitalRead(WIO_5S_LEFT) == LOW || gestureReading == APDS9960_RIGHT) {
    return 'L';
  } else if(digitalRead(WIO_5S_RIGHT) == LOW || gestureReading == APDS9960_LEFT) {
    return 'R';
  } else if(digitalRead(WIO_5S_PRESS) == LOW) {
    return 'P';
  } else {
    return 'N';
  }
}

void changeParameter(int currentScreen, bool UpDown) {
  switch (currentScreen) {
    case 0:
      break;
    case 1:
      if (UpDown == true) {
        plant.setWaterIntervalDays(++waterIntervaldays);
      } else {
        plant.setWaterIntervalDays(--waterIntervaldays);
      }
      break;
    case 2:
      drainPlants = !drainPlants;
      plant.setDrainBetweenWatering(drainPlants);
      break;
    case 3:
      break;
    case 4:
      trackSleep = !trackSleep;
      break;
    case 5:
      carePlants = !carePlants;
      break;
  } 
}

void resetParameter(int currentScreen) {
  switch (currentScreen) {
    case 0:
      break;
    case 1:
      plant.setWaterIntervalDays(7);
      break;
    case 2:
      plant.setDrainBetweenWatering(true);
      break;
    case 3:
      break;
    case 4:
      trackSleep = true;
      break;
    case 5:
      carePlants = true;
      break;
  }
}

void showScreen(int screen) {
  switch (screen) {
    case 0:
      interface.showPlantStatsAll();
      break;
    case 1:
      interface.showPlantControlsWaterInterval(waterIntervaldays);
      break;
    case 2:
      interface.showPlantControlsDrainage(drainPlants);
      break;
    case 3:
      interface.showSleepStatsAll();
      break;
    case 4:
      interface.showSettingsTrackSleepYN(trackSleep, carePlants);
      break;
    case 5:
      interface.showSettingsTrackPlantsYN(trackSleep, carePlants);
      break;
  }
}
void setup() {
  // put your setup code here, to run once:
  //init buttons/joystick

  pinMode(WIO_BUZZER, OUTPUT);
  analogWrite(WIO_BUZZER, 220);
  delay(100);
  analogWrite(WIO_BUZZER, 0);
  delay(100);
  pinMode(WIO_KEY_A, INPUT);
  pinMode(WIO_KEY_B, INPUT);
  pinMode(WIO_KEY_C, INPUT);
  pinMode(WIO_5S_UP, INPUT);
  pinMode(WIO_5S_DOWN, INPUT);
  pinMode(WIO_5S_LEFT, INPUT);
  pinMode(WIO_5S_RIGHT, INPUT);
  pinMode(WIO_5S_PRESS, INPUT);
  pinMode(LCD_BACKLIGHT, OUTPUT);

  timeStartMillis = millis();
  isAsleep = false; 
  if (!interface.begin()) {

  }

  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
    interface.showError("SD Card Error");
  }

  if (!plant.begin()) {
    interface.showError("Plant Library Error");
  }

  if (!sleep.begin()) {
    interface.showError("Sleep Library Error");
  }

  if (!gesture.begin()) {
    interface.showError("Gesture Sensor Error");
  }

  gesture.enableProximity(true);
  gesture.enableGesture(true);
 


  digitalWrite(LCD_BACKLIGHT, LOW);
  delay(1000);
}



void loop() {
  char input = checkInputs();
  if (input != 'n') {
    if (isAsleep) {
      digitalWrite(LCD_BACKLIGHT, LOW);
      showScreen(currentScreen);
      isAsleep = false;
    }
    sleepTimer = 0;
  }
  //if function 1 pressed, go to plant screen, else, pass
  if (input == 'A') {
    currentScreen = 0;
    showScreen(currentScreen);
  }
  //if function 2 pressed, go to sleep screen, else, pass
  if (input == 'B') {
    currentScreen = 3;
    showScreen(currentScreen);
  }
  //if function 3 pressed, go to settings screen, else, pass
  if (input == 'C') {
    currentScreen = 4;
    showScreen(currentScreen);
  }
  //if left registered, if at min parameter/screen, wrap around, else, go to prev parameter/screen, else, pass
  if (input == 'L') {
    if (currentScreen == 0) {
      currentScreen = 5;
      showScreen(currentScreen);
    } else {
      currentScreen--;
      showScreen(currentScreen);
    }
  }
  //if right registered, if at max parameter/screen, wrap around, else, go to next parameter/screen, else, pass
  if (input == 'R') {
    if (currentScreen == 5) {
      currentScreen = 0;
      showScreen(currentScreen);
    } else {
      currentScreen++;
      showScreen(currentScreen);
    }
  }
  //if up registered, increment current parameter, else, pass
  if (input == 'U') {
    changeParameter(currentScreen, true);
    showScreen(currentScreen);
  }
  //if down registered, decrement current parameter, else, pass
  if (input == 'D') {
    changeParameter(currentScreen, false);
    showScreen(currentScreen);
  }
  //if joystick pressed, reset current parameter, else, pass
  if (input == 'P') {
    resetParameter(currentScreen);
    showScreen(currentScreen);
  }

  //if sleep tracking is on
  if(trackSleep) {
    
    //if time is between 11pm and 7:30am (placeholder: every twelve hours)
    if (millis() - timeStartMillis >= 12*60*60*1000) {
      timeStartMillis = millis();
      sleep.update();
    }
  } 
  //if plant care is turned on
  if (carePlants) {
    //if time is divisible by 6 hours, check plant water, if low, if water storage low, add to notifs, else, water plants, else, pass
    if (millis()%21600000 == 0) {
      plant.update();
    }  
  }
  //if time is divisible by 1 second, if sleeping, pass, else, increment sleep timer, else, pass
  if (millis()%1000 == 0) {
    //if sleep timer is equal to one minute and we are not asleep, sleep, else, pass
    if (sleepTimer <= 60000 && isAsleep == false) {
      sleepTimer++;
    } else {
      digitalWrite(LCD_BACKLIGHT, HIGH);
      isAsleep = true;
      sleepTimer = 0;
    }
  }
}