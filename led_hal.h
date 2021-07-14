#ifndef LED_HAL_H
#define LED_HAL_H

#include <FastLED.h>
#include "global.h"

void led_initialize(StateProvider *s);
void led_animateFrame();

void led_setAll(EloColor color);
void led_clear();
void led_showStrip();
void led_FillLEDsFromPaletteColors(uint8_t colorIndex);
void led_changeAnimation(int anim, int animationSpeed);
void led_applyIndicatorState();

void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();

#ifdef ELO_INDICATOR

typedef struct
{
    byte state;
    CRGB color;
    byte level;
    byte start;
    byte length;
    int phase;
} Indicator;

#define INDICATOR_LEVEL_NONE 0
#define INDICATOR_LEVEL_NORMAL 1
#define INDICATOR_LEVEL_ELEVATED 2
#define INDICATOR_LEVEL_CRITICAL 3

#define INDICATOR_STATE_NONE 0
#define INDICATOR_STATE_1 1
#define INDICATOR_STATE_2 2
#define INDICATOR_STATE_3 3
#define INDICATOR_STATE_4 4
#define INDICATOR_STATE_5 5
#define INDICATOR_STATE_6 6

void led_loadIndicators(byte *bufPtr);
int led_getIndicatorCount();
byte *led_getIndicatorBuffer();
void led_setIndicator(int indicatorIndex, int state, int level);
void led_configIndicator(int indicatorIndex, int start, int length);
int led_stateHasChanged();
void led_resetVersion();
#endif

#endif