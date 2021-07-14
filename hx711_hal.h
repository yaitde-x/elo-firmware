#ifndef HX711_HAL_H
#define HX711_HAL_H

#include "HX711.h"
#include "global.h"

int scaleReadingHasChanged(double currentReading, double lastReading);

void hx711_setup(StateProvider *s, CommsProvider *commsProvider);
void hx711_loop();

#endif