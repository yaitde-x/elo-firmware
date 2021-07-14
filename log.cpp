#include <Arduino.h>
#include "log.h"

void print(const char *msg){
    Serial.print(msg);
}

void println(const char *msg) {
    Serial.println(msg);
}