#include "global.h"
#ifdef ELO_HIGROW

#include <WiFi.h>
#include "higrow_hal.h"

StateProvider *higrowStateProvider;
CommsProvider *higrowCommsProvider;

int moistureDataPin;
long higrowLastReadingMillis = 0;
int higrowSampleMs = HIGROW_DEFAULT_SAMPLE;
int lastMoisture = 0;
int lastLight = 0;

#define SOME_PIN 16

void higrow_setup(StateProvider *s, CommsProvider *c, int moisturePin)
{
    higrowStateProvider = s;
    higrowCommsProvider = c;

    moistureDataPin = moisturePin;

    pinMode(SOME_PIN, OUTPUT);
    pinMode(HIGROW_POWER_PIN, INPUT);
    digitalWrite(SOME_PIN, LOW);
}

void higrow_loop()
{

    long currentMillis = millis();

    if (currentMillis - higrowLastReadingMillis > higrowSampleMs)
    {

        higrowLastReadingMillis = currentMillis;

        int waterlevel = analogRead(moistureDataPin);
        int lightlevel = analogRead(HIGROW_LIGHT_PIN);

        waterlevel = map(waterlevel, 0, 4095, 0, 1023);
        waterlevel = constrain(waterlevel, 0, 1023);
        lightlevel = map(lightlevel, 0, 4095, 0, 1023);
        lightlevel = constrain(lightlevel, 0, 1023);

        //Serial.println(waterlevel);
        //Serial.println(lightlevel);

        if (lastMoisture != waterlevel || lastLight != lightlevel) {
            lastMoisture = waterlevel;
            lastLight = lightlevel;
            
            higrowStateProvider->setVar4(waterlevel + higrowStateProvider->getCal4());
            higrowStateProvider->setVar5(lightlevel + higrowStateProvider->getCal5());
            // we do not need to broadcast state. DHT is purposefully after higrow in loop.. dht will broadcast
        }
    }
}

#endif
