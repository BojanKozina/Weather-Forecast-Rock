#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Rock_States.h"
#include "Rock.h"


Rock theRock;

void setup()
{
  Serial.begin(9600);

  
  theRock.begin();
  
  theRock.showData(25.5, 60.0, 1013.25, false);

}

void loop()
{

  
  
}