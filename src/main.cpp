#include <Arduino.h>

#include "Nextion/Nextion.h"
#include "Device/DeviceInfo.h"

uint16_t prevMillis;
uint16_t deltaMillis;

uint16_t counter;

void setup() {  
  setupNextion();
  prevMillis = millis();
  counter = 0;
  // put your setup code here, to run once:
}

void loop() {
  deltaMillis = millis() - prevMillis;
  counter +=deltaMillis;
  if(counter > 250)
  {
    counter = 0;
    updateVals();
  }
  mots.motorLoop();
  handleNextionData();
  // put your main code here, to run repeatedly:
}