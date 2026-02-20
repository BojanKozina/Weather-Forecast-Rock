#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Rock_States.h"
#include "Rock.h"
#include "Sensors.h"
#include "Config.h"

Rock theRock;


void setup()
{
  Serial.begin(9600);

 theRock.begin(WIFI_SSID,WIFI_PASSWORD, MQTT_SERVER);

}



void loop()
{

  theRock.update();


}