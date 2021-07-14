#ifndef _button_hal_
#define _button_hal_

#include "global.h"
#include "net.h"
#include "device_hal.h"

void button_initialize(StateProvider *sp, CommsProvider *c);
void button_loop();
#endif