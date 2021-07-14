#ifndef GLOBAL_H
#define GLOBAL_H

#include "build.h"
#include "device_type.h"

// Set where debug messages will be printed.
#define DEBUG_PRINTER Serial
// If using something like Zero or Due, change the above to SerialUSB

// Define actual debug output functions when necessary.
#ifdef MQTT_DEBUG
#define DEBUG_PRINT(...)                  \
    {                                     \
        DEBUG_PRINTER.print(__VA_ARGS__); \
    }
#define DEBUG_PRINTLN(...)                  \
    {                                       \
        DEBUG_PRINTER.println(__VA_ARGS__); \
    }
#define DEBUG_PRINTBUFFER(buffer, len) \
    {                                  \
        printBuffer(buffer, len);      \
    }
#else
#define DEBUG_PRINT(...) \
    {                    \
    }
#define DEBUG_PRINTLN(...) \
    {                      \
    }
#define DEBUG_PRINTBUFFER(buffer, len) \
    {                                  \
    }
#endif

typedef unsigned char byte;

typedef struct EloColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} EloColor;

extern EloColor Black;
extern EloColor White;

class StateWriter
{
  public:
    void (*write)(int, unsigned char);
    unsigned char (*read)(int);
};

class Logger {
    public:
        void (*print)(const char *);
        void (*println)(const char *);
};

class StateProvider
{

  public:
    int (*getState)();
    void (*setState)(int);
    EloColor (*getColor)();
    void (*setColor)(EloColor);

    int (*stateHasChanged)();
    void (*resetVersion)();

    void (*setAnimation)(int);
    int (*getAnimation)();

    void (*setBrightness)(int);
    int (*getBrightness)();

    int (*getParam1)();
    void (*setParam1)(int);
    int (*getParam2)();
    void (*setParam2)(int);
    int (*getParam3)();
    void (*setParam3)(int);
    int (*getParam4)();
    void (*setParam4)(int);
    int (*getParam5)();
    void (*setParam5)(int);

    int (*getVar1)();
    void (*setVar1)(int);
    int (*getVar2)();
    void (*setVar2)(int);
    int (*getVar3)();
    void (*setVar3)(int);
    int (*getVar4)();
    void (*setVar4)(int);
    int (*getVar5)();
    void (*setVar5)(int);

    int (*getCal1)();
    void (*setCal1)(int);
    int (*getCal2)();
    void (*setCal2)(int);
    int (*getCal3)();
    void (*setCal3)(int);
    int (*getCal4)();
    void (*setCal4)(int);
    int (*getCal5)();
    void (*setCal5)(int);

    int (*getVersion)();
};

class LightingProvider
{
  public:
    void (*setAll)(EloColor);
    void (*show)();
    int (*stateHasChanged)();
    void (*resetVersion)();

    void (*loadIndicators)(byte *);
    int (*getIndicatorCount)();
    byte *(*getIndicatorBuffer)();
};

class IndicatorProvider
{
  public:
    void (*setIndicator)(int, int, int);
    void (*configIndicator)(int, int, int);
};

class DeviceState
{
  public:
    int state;
};

class CommsProvider
{
  public:
    void (*broadcastState)();
};

// factories
LightingProvider *getLightingProvider();
StateProvider *getStateProvider();
CommsProvider *getCommsProvider();
IndicatorProvider *getIndicatorProvider();

#endif