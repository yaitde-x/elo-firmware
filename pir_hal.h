#ifndef PIR_HAL
#define PIR_HAL

#include "global.h"

#define NO_MOTION LOW
#define MOTION_DETECTED HIGH
#define PIR_DEFAULT_SAMPLE 1000

void pir_setup(StateProvider *s, CommsProvider *c, int dataPin);
void pir_loop();

#endif
