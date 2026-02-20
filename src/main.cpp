#include <Arduino.h>
#include "Rock.h"
#include "Config.h"

Rock theRock;


void setup()
{
  Serial.begin(115200);

 theRock.begin(WIFI_SSID,WIFI_PASSWORD, MQTT_SERVER);

}



void loop()
{

  theRock.update();


}