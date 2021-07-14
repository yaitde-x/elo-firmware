#ifndef PERSIST_HAL_H_
#define PERSIST_HAL_H_

#include "device_hal.h"

#ifdef ELO_LED
#include "led_hal.h"
#endif


void persist_initialize();
bool loadConfig(StateProvider *sp);
bool saveConfig(StateProvider *sp);
bool loadIndicators(LightingProvider *lp);
bool saveIndicators(LightingProvider *lp);
#endif