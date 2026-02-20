#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Rock_States.h"
#include <Adafruit_BMP280.h>

#define BMP280_ADDRESS 0x76

#define LIGHT_SENSOR_PIN 34

#define RAIN_PIN 25

#define WIND_PIN 27

class Sensors
{
    Adafruit_BMP280 bmpSensor;

    float temperature{0};
    float pressure{0};
    float lightLevel{0};
    bool isRaining{false};
    bool isWindy{false};

public:
    Sensors() : bmpSensor() {}

    void begin()
    {

        pinMode(RAIN_PIN, INPUT_PULLUP);

        pinMode(WIND_PIN, INPUT_PULLUP);

        while (!bmpSensor.begin(BMP280_ADDRESS))
        {
            Serial.println("BMP280 not found. Retrying...");
            delay(1000);
        }

        Serial.println("BMP280 initialized.");
    }

    float readTemperature()
    {
         return bmpSensor.readTemperature();
    }

    float readPressure()
    {
       return  bmpSensor.readPressure() / 100.0F;
 
    }

    float readLightLevel()
    {
        int raw = analogRead(LIGHT_SENSOR_PIN);

        int darkValue = 2100;
        int brightValue = 200;

        raw = constrain(raw, brightValue, darkValue);
        float percent = (float)(darkValue - raw) / (darkValue - brightValue) * 100.0;

        return percent;
    }

    bool readRainData()
    {
        return digitalRead(RAIN_PIN) == LOW;
    }

    bool readWindData()
    {
        int triggers = 0;
        int lastState = HIGH;

        for (int i = 0; i < 100; i++)
        {
            int current = digitalRead(WIND_PIN);

            if (lastState == HIGH && current == LOW)
                triggers++;

            lastState = current;
            delay(20);
        }

        return triggers >= 3;
    }

    void setWindy()
    {
        isWindy = readWindData();
    }

    void setRaining()
    {
        isRaining = readRainData();
    }

    void setTemperature()
    {
        temperature = readTemperature();
    }
   
    void setPressure()
    {
        pressure = readPressure();
    }

    void setLightLevel()
    {
        lightLevel = readLightLevel();
    }

    void setAll()
    {
        setTemperature();
        setPressure();
        setLightLevel();
        setRaining();
        setWindy();
    }

    float getTemperature()
    {
        return temperature;
    }

    float getPressure()
    {
        return pressure;
    }

    float getLightLevel()
    {
        return lightLevel;
    }

    bool getIsRaining()
    {
        return isRaining;
    }

    bool getIsWindy()
    {
        return isWindy;
    }

    void printBMP280DD()
    {
        Serial.print("Temperature: ");
        Serial.print(getTemperature(), 1);
        Serial.println(" °C");

        Serial.print("Pressure: ");
        Serial.print(getPressure(), 0);
        Serial.println(" hPa");
    }

    void printLightLevel()
    {
        Serial.print("Light Level: ");
        Serial.print(getLightLevel(), 1);
        Serial.println(" %");
    }

    void printRainData()
    {
        Serial.print("Rain Detected: ");
        Serial.println(getIsRaining() ? "Yes" : "No");
    }

    void printWindData()
    {
        Serial.print("Windy: ");
        Serial.println(getIsWindy() ? "Yes" : "No");
    }

    void printAll()
    {
        Serial.println("---- Sensor Readings ----");
        printBMP280DD();
        Serial.println("------------------");
        printLightLevel();
        Serial.println("------------------");
        printRainData();
        Serial.println("------------------");
        printWindData();
        Serial.println("---- Sensor Readings End----");
        Serial.println();
    }
};