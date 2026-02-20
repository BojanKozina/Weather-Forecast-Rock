#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Rock_States.h"
#include "Sensors.h"
#include "Network.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class Rock
{
    Sensors theSensors;
    Adafruit_SSD1306 theDisplay;
    Network theNetwork;

    int displayCycles{0};

public:
    Rock() : theDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

    void begin(const char *ssid, const char *password, const char *mqttServer)
    {

        Wire.begin();

        theSensors.begin();
        theNetwork.setCredentials(ssid, password, mqttServer);

        while (!theDisplay.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
        {
            Serial.println(F("SSD1306 allocation failed"));
            delay(2000);
        }

        randomSeed(esp_random());

        theSensors.setAll();

        theDisplay.clearDisplay();
        theDisplay.display();

        if (theNetwork.connectWiFi())
            theNetwork.connectMQTT();
        else
        {
            showFace(Rock_States::NO_WIFI); // show the dino!
            delay(5000);
        }

        theNetwork.publishData(theSensors.getTemperature(), theSensors.getPressure(), theSensors.getLightLevel(), theSensors.getIsRaining(), theSensors.getIsWindy(), determineState());
    }

    // Based on the enum in Rock_States.h its going to return one of the bitmaps (ie the face of the rock)
    const unsigned char *getFaceBitmap(Rock_States state)
    {

        switch (state)
        {
        case Rock_States::SLEEPING:
            return Rock_Sleeping;
        case Rock_States::HOT:
            return Rock_Hot;
        case Rock_States::COLD:
            return Rock_Cold;
        case Rock_States::RAINING:
            return Rock_Raining;
        case Rock_States::HAPPY:
            return Rock_Happy;
        case Rock_States::NEUTRAL:
            return Rock_Neutral;
        case Rock_States::SNOWING:
            return Rock_Snowing;
        case Rock_States::WINDY:
            return Rock_Windy;
        case Rock_States::GLASSES:
            return Rock_Glasses;
        case Rock_States::WINDY_RAIN:
            return Rock_Windy_Rain;
        case Rock_States::COLD_RAIN:
            return Rock_Cold_Rain;
        case Rock_States::NO_WIFI:
            return Rock_No_Wifi;

        // Joke faces
        case Rock_States::POU:
            return Rock_Pou;
        case Rock_States::ALIEN:
            return Rock_Alien;
        case Rock_States::ROBLOX:
            return Rock_Roblox;
        case Rock_States::ROCK_ON:
            return Rock_Rock_On;
        case Rock_States::NERD:
            return Rock_Nerd;
        case Rock_States::DOG:
            return Rock_Dog;
        case Rock_States::COMPLIMENT:
            return Rock_Compliment;
        case Rock_States::BSOD:
            return Rock_BSOD;
        case Rock_States::MARIO:
            return Rock_Mario;
        case Rock_States::AMOGUS:
            return Rock_Amogus;
        case Rock_States::ZEN:
            return Rock_Zen;

        default:
            return Rock_Neutral;
        }
    }

    Rock_States determineState()
    {

        // Error state
        if (theSensors.getTemperature() == -999)
            return Rock_States::NEUTRAL;

        // Night time - sleeping
        if (theSensors.getLightLevel() < 5)
            return Rock_States::SLEEPING;

        // Snow: rain + freezing
        if (theSensors.getIsRaining() && theSensors.getTemperature() < -5)
            return Rock_States::SNOWING;

        // Wind + rain combo
        if (theSensors.getIsWindy() && theSensors.getIsRaining())
            return Rock_States::WINDY_RAIN;

        // Cold + rain combo
        if (theSensors.getIsRaining() && theSensors.getTemperature() < 10)
            return Rock_States::COLD_RAIN;

        // Just raining
        if (theSensors.getIsRaining())
            return Rock_States::RAINING;

        // Just windy
        if (theSensors.getIsWindy())
            return Rock_States::WINDY;

        // Very hot
        if (theSensors.getTemperature() > 32)
            return Rock_States::HOT;

        // Warm and sunny
        if (theSensors.getTemperature() > 25 && theSensors.getLightLevel() > 80)
            return Rock_States::GLASSES;

        // Cold
        if (theSensors.getTemperature() < 10)
            return Rock_States::COLD;

        // Nice weather
        if (theSensors.getLightLevel() > 70 && theSensors.getTemperature() > 20)
            return Rock_States::HAPPY;

        // Default
        return Rock_States::NEUTRAL;
    }

    void update()
    {
        if (displayCycles >= 2)
        {
            theSensors.setAll();
            theSensors.printAll();
            displayCycles = 0;
        }

        Rock_States currentState;

        // This part is totally uncessary and its just used to show the joke faces randomly
        int easterEggChance = random(10);
        if (easterEggChance == 0)
        {
            int randomJokeFace = random(static_cast<int>(Rock_States::JOKE_START+1), static_cast<int>(Rock_States::COUNT));
            currentState = static_cast<Rock_States>(randomJokeFace);
        }
        else
        {
            currentState = determineState();
        }

        showFace(currentState);
        theNetwork.publishData(theSensors.getTemperature(), theSensors.getPressure(), theSensors.getLightLevel(), theSensors.getIsRaining(), theSensors.getIsWindy(), currentState);
        delay(4000);

        showData(theSensors.getTemperature(), theSensors.getPressure(), theSensors.getLightLevel(), theSensors.getIsRaining());
        displayCycles++;
        delay(5000);
    }

    void showFace(Rock_States state)
    {
        const unsigned char *face = getFaceBitmap(state);

        theDisplay.clearDisplay();

        theDisplay.drawBitmap(0, 0, face, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

        theDisplay.display();
    }

    void shuffleStates()
    {
        int randomFace = random(static_cast<int>(Rock_States::COUNT));
        showFace(static_cast<Rock_States>(randomFace));
        delay(3000);
    }

    void shuffleJokeFaces()
    {
        int randomFace = random(static_cast<int>(Rock_States::JOKE_START+1), static_cast<int>(Rock_States::COUNT));
        showFace(static_cast<Rock_States>(randomFace));
        delay(3000);
    }

    void showData(float temperature, float pressure, float lightLevel, bool isRaining)
    {
        theDisplay.clearDisplay();
        theDisplay.setTextSize(1);

        theDisplay.drawBitmap(0, 0, Data_Screen, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
        theDisplay.setTextColor(SSD1306_WHITE);

        theDisplay.setCursor(45, 5);
        theDisplay.print(temperature, 1);
        theDisplay.print("  C");

        theDisplay.setCursor(45, 21);
        theDisplay.print(pressure, 0);
        theDisplay.print(" hPa");

        theDisplay.setCursor(45, 37);
        theDisplay.print(lightLevel, 1);
        theDisplay.print("  %");

        theDisplay.setCursor(45, 53);
        theDisplay.print(isRaining ? "Raining" : "No Rain");

        theDisplay.display();
    }
};