#include "global.h"
#ifdef ELO_PIR

#include <Esp.h>

#include "pir_hal.h"

StateProvider *pirStateProvider;
CommsProvider *pirCommsProvider;

int pirDataPin;
int lastReading = NO_MOTION;
int pirSampleMs = PIR_DEFAULT_SAMPLE;
long pirLastReadingMillis = 0;

void pir_setup(StateProvider *s, CommsProvider *c, int dataPin){
    pirStateProvider = s;
    pirCommsProvider = c;
    pirDataPin = dataPin;
    pinMode(pirDataPin, INPUT);
}

void pir_loop() {

    long currentMillis = millis();

    if (currentMillis - pirLastReadingMillis > pirSampleMs) 
    {
        pirLastReadingMillis = currentMillis;
        
        int reading = digitalRead(pirDataPin);

        if (reading != lastReading)
            {

            Serial.print("Transmit reading: ");
            Serial.print(reading);
            Serial.println();

            lastReading = reading;
            pirStateProvider->setVar5(reading);
            pirCommsProvider->broadcastState();
            }
    }
}

#endif
