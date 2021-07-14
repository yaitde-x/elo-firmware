#ifndef HIGROW_HAL_H
#define HIGROW_HAL_H

#include "global.h"

#define HIGROW_DEFAULT_SAMPLE 30000

void higrow_setup(StateProvider *s, CommsProvider *c, int moisturePin);
void higrow_loop();


#endif