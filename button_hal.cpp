
#include "Arduino.h"
#include "global.h"
#include "button_hal.h"

#ifdef ELO_BUTTON
enum ButtonTypes {
    Momentary = 1, Toggle = 2, Timer = 3
};

StateProvider *buttonStateProvider;
CommsProvider *buttonCommsProvider;

byte buttonPin;
byte buttonType;

void button_initialize(StateProvider *sp, CommsProvider *c)
{
    buttonStateProvider = sp;
    buttonCommsProvider = c;

    buttonPin = buttonStateProvider->getParam1();
    buttonType = buttonStateProvider->getParam2();

    if (buttonPin == 0)
        buttonPin = ELO_BUTTON_PIN;
    if (buttonType == 0)
        buttonType = ELO_BUTTON_TYPE;

    if (buttonStateProvider->getParam3() == 0)
        buttonStateProvider->setParam3(100);

    pinMode(buttonPin, INPUT);
}

long touchDetected = 0;
byte tempState = 0;

void button_loop()
{
    byte pinState = digitalRead(buttonPin);
    long currentMillis = millis();
    int detectDelay = buttonStateProvider->getParam3();
    
    if (pinState == HIGH && touchDetected == 0){
        touchDetected = millis();
        Serial.println(F("touch detected"));
    } else if (pinState == HIGH && touchDetected > 0 && currentMillis - touchDetected > detectDelay && tempState == 0)
    {
        Serial.println(F("sending button touch"));
        buttonStateProvider->setVar1(1);
        buttonCommsProvider->broadcastState();
        tempState = 1;
    } else if (pinState == LOW && touchDetected > 0 && tempState == 1){
        touchDetected = 0;
        tempState = 0;
        buttonStateProvider->setVar1(0);
        buttonCommsProvider->broadcastState();
        Serial.println(F("sending button no touched"));
    }

}
#endif