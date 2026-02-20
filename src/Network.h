#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class Network
{
    WiFiClient espClient;
    PubSubClient mqtt;

    const char *ssid = "";
    const char *password = "";
    const char *mqttServer = "";
    const int mqttPort = 1883;

public:
    Network() : mqtt(espClient) {}

    void setCredentials(const char *ssid, const char *password, const char *mqttServer)
    {
        this->ssid = ssid;
        this->password = password;
        this->mqttServer = mqttServer;
    }

    bool connectWiFi()
    {
        WiFi.begin(ssid, password);
        int attempts = 0;

        while (WiFi.status() != WL_CONNECTED && attempts < 20)
        {
            delay(500);
            Serial.print(".");
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
            return true;
        }

        Serial.println("\nWiFi failed");
        return false;
    }

    bool connectMQTT()
    {
        mqtt.setServer(mqttServer, mqttPort);

        if (mqtt.connect("WeatherRock"))
        {
            Serial.println("MQTT connected");
            return true;
        }

        Serial.println("MQTT failed");
        return false;
    }

    void publishData(float temp, float pressure, float light, bool rain, bool wind, Rock_States state)
    {
        if (!mqtt.connected())
            connectMQTT();

        mqtt.loop();

        mqtt.publish("rock/temperature", String(temp, 1).c_str());
        mqtt.publish("rock/pressure", String(pressure, 0).c_str());
        mqtt.publish("rock/light", String(light, 1).c_str());
        mqtt.publish("rock/rain", rain ? "true" : "false");
        mqtt.publish("rock/wind", wind ? "true" : "false");
        mqtt.publish("rock/state", String(static_cast<int>(state)).c_str());
    }

    bool isConnected()
    {
        return WiFi.status() == WL_CONNECTED && mqtt.connected();
    }
};