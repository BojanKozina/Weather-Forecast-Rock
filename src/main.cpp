#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Rock_States.h"
#include "Rock.h"
#include "Sensors.h"


Rock theRock;


void setup()
{
  Serial.begin(9600);

 theRock.begin("e25d4c","271650987","77.78.200.65");

}



void loop()
{

  theRock.update();


}