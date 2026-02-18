#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Rock_States.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class Rock
{

    Adafruit_SSD1306 display;

public:
    Rock() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

    void begin()
    {
        if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
        {
            Serial.println(F("SSD1306 allocation failed"));
            for (;;)
                ;
        }

        randomSeed(esp_random());

        display.clearDisplay();
        display.display();
    }


    //Based on the enum in Rock_States.h its going to return one of the bitmaps (ie the face of the rock)
    const unsigned char* getFaceBitmap(Rock_States state)
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

        default: return Rock_Neutral;
        }
    }

    void showFace(Rock_States state)
    {
        const unsigned char *face = getFaceBitmap(state);

        display.clearDisplay();
        
        display.drawBitmap(0, 0, face, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
        
        display.display();
    }

    void shuffleStates()
    {
        int randomFace= random(11);
        showFace(static_cast<Rock_States>(randomFace));
        delay(3000);
    }
};