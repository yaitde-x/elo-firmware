#ifndef SERIAL_H
#define SERIAL_H

#include <CmdMessenger.h>

#include "global.h"
#include "device_hal.h"

void serial_initialize(StateProvider *s);
void serial_loop();

#endif