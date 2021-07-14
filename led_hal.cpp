#include "global.h"
#ifdef ELO_LED // ignore the entire module

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#include "led_hal.h"


CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

#ifdef ELO_INDICATOR
Indicator indicators[MAX_INDICATORS];
int indicatorCount = MAX_INDICATORS;
int indicatorWriteVersion = 0;
#endif

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int oldState;

int refreshFactor = 7;
int refreshCounter = 0;
byte lastAnimation = 0;
byte lastParam1 = 0;

StateProvider *led_stateProvider;

CRGB toCRGB(EloColor ec) {
  return CRGB(ec.r, ec.g, ec.b);
}

EloColor toEloColor(CRGB c) {
  return {c.red, c.green, c.blue};
}

void led_initialize(StateProvider *s) {

  led_stateProvider = s;

  pinMode(LED_PIN, OUTPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(led_stateProvider->getBrightness());

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

#ifdef ELO_INDICATOR

  for (int i = 0; i < MAX_INDICATORS; i++)
  {
    indicators[i].state = INDICATOR_STATE_NONE;
    indicators[i].color = CRGB::Black;
    indicators[i].level = INDICATOR_LEVEL_NONE;
    indicators[i].start = 0;
    indicators[i].length = 0;
    indicators[i].phase = 0;
  }

  #ifdef BLD_IND
  indicatorCount = 1;
  indicators[0].start = 135;
  indicators[0].length = 14;
  #endif

  #ifdef DF_IND
  indicatorCount = 2;
  indicators[1].start = 70;
  indicators[1].length = 14;
  #endif

#endif

  oldState = led_stateProvider->getState();

}

void led_animateFrame() {
  if (refreshFactor <= refreshCounter) {

    int anim = led_stateProvider->getAnimation();
    int param = led_stateProvider->getParam1();

    if (lastAnimation != anim || lastParam1 != param)
      led_changeAnimation(anim, param);

    lastAnimation = anim;
    lastParam1 = param;

    int newState = led_stateProvider->getState();

    refreshCounter = 0;

    if (newState == 1) {

      if (led_stateProvider->getAnimation() == 0) {
        led_setAll(led_stateProvider->getColor());
      } else {

        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */

        led_FillLEDsFromPaletteColors( startIndex);
      }
    } else {
      if (newState != oldState) {
        oldState = newState;
      }

      led_clear();
      for (int led = 0; led < NUM_LEDS; led++) {
        leds[led] = CRGB::Black;
      }

    }
  }

  refreshCounter++;
  #ifdef ELO_INDICATOR
    led_applyIndicatorState();
  #endif

  led_showStrip();
}

#ifdef ELO_INDICATOR

void dumpIndicators()
{
    for (int i = 0; i < indicatorCount; i++)
    {
        Serial.print(i);
        Serial.print(F("="));
        Serial.print(indicators[i].state);
        Serial.print(F(","));
        Serial.print(indicators[i].level);
        Serial.print(F(","));
        Serial.print(indicators[i].start);
        Serial.print(F(","));
        Serial.print(indicators[i].length);
        Serial.println();
    }
}

void led_loadIndicators(byte *bufPtr) {

    indicatorCount = *((int *)bufPtr);
    bufPtr += sizeof(int);

    Serial.print(F(" cnt="));
    Serial.println(indicatorCount);
    indicatorCount = 0;
    for (int i = 0; i < indicatorCount; i++)
    {
        Indicator *ind = (Indicator *)bufPtr;
        indicators[i].state = ind->state;
        indicators[i].level = ind->level;
        indicators[i].start = ind->start;
        indicators[i].length = ind->length;

        bufPtr += sizeof(Indicator);
    }

  dumpIndicators();
}

int led_getIndicatorCount() {
  return indicatorCount;
}

byte *led_getIndicatorBuffer() {
  return (byte *) indicators;
}

void led_setIndicator(int i, int s, int l) {

  if (i >= MAX_INDICATORS)
    return;

  DEBUG_PRINT(F("led_set_ind"));
  indicators[i].state = s;
  indicators[i].level = l;
}

void led_configIndicator(int i, int s, int l) {

  if (i >= MAX_INDICATORS)
    return;

  if (i >= indicatorCount)
    {
      Serial.println(F("increasing indicators"));
      indicatorWriteVersion++;
      indicatorCount = i + 1;
    }

  if (indicators[i].start != s || indicators[i].length != l)
    indicatorWriteVersion++;

  DEBUG_PRINT(F("led_cfg_ind"));
  indicators[i].start = s;
  indicators[i].length = l;
}

int led_stateHasChanged() {
  return indicatorWriteVersion;
}

void led_resetVersion() {
  indicatorWriteVersion = 0;
}
#endif

void setPixel(int pixel, EloColor c) {
  CRGB color = toCRGB(c);
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(pixel, strip.Color(color.r, color.g, color.b));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[pixel] = color;
#endif
}

void led_setAll(EloColor color) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, color);
  }
}

void led_clear() {
  FastLED.clear();
}

void led_showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.setBrightness(led_stateProvider->getBrightness());
  FastLED.show();
#endif
}

void led_FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void led_changeAnimation(int anim, int animationSpeed) {

  refreshFactor = map(animationSpeed, 1, 255, 1, 8);

  if (refreshFactor <= 0)
    refreshFactor = 1;

  if ( anim ==  1)  {
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
  }
  if ( anim == 2)  {
    currentPalette = LavaColors_p;
    currentBlending = LINEARBLEND;
  }
  if ( anim == 3)  {
    SetupPurpleAndGreenPalette();
    currentBlending = LINEARBLEND;
  }
  if ( anim == 4)  {
    SetupTotallyRandomPalette();
    currentBlending = LINEARBLEND;
  }
  if ( anim == 5)  {
    SetupBlackAndWhiteStripedPalette();
    currentBlending = LINEARBLEND;
  }
  if ( anim == 6)  {
    currentPalette = CloudColors_p;
    currentBlending = LINEARBLEND;
  }
  if ( anim == 7)  {
    currentPalette = PartyColors_p;
    currentBlending = LINEARBLEND;
  }
}

void led_applyIndicatorState() {
  DEBUG_PRINT(F(" ."))
  //DEBUG_PRINT(F("max_ind "));
  //DEBUG_PRINT(MAX_INDICATORS);

  for (int i = 0; i < MAX_INDICATORS; i++) {

    Indicator *indicator = &(indicators[i]);
    if (i == 0) {
      DEBUG_PRINT(i);
      DEBUG_PRINT(F(","));
      DEBUG_PRINT(indicator->start);
      DEBUG_PRINT(F(","));
      DEBUG_PRINT(indicator->length);
      DEBUG_PRINT(F(","));
      DEBUG_PRINT(indicator->state);
      DEBUG_PRINT(F(","));
      DEBUG_PRINT(indicator->level);
    }

    byte bpm = 15;

    if (indicator->level == INDICATOR_LEVEL_ELEVATED)
      bpm = 25;
    else if (indicator->level == INDICATOR_LEVEL_CRITICAL)
      bpm = 45;

    byte c;

    if (indicator->level == INDICATOR_LEVEL_NONE)
      c = 255;
    else
      c = beatsin8(bpm, 0, 220);

    if (indicator->state == INDICATOR_STATE_NONE) {
      if (led_stateProvider->getState() == 0) {
        fill_solid( &(leds[indicator->start]), indicator->length, CRGB::Black);
      } else if (led_stateProvider->getAnimation() == 0) {
        CRGB color = toCRGB(led_stateProvider->getColor());
        fill_solid( &(leds[indicator->start]), indicator->length, color);
      }
    } else if (indicator->state == INDICATOR_STATE_1)
      fill_solid( &(leds[indicator->start]), indicator->length, CRGB(0, c, 0));
    else if (indicator->state == INDICATOR_STATE_2)
      fill_solid( &(leds[indicator->start]), indicator->length, CRGB(c, 0, 0));
    else if (indicator->state == INDICATOR_STATE_3)
      fill_solid( &(leds[indicator->start]), indicator->length, CRGB(0, 0, c));
    else if (indicator->state == INDICATOR_STATE_4)
      fill_solid( &(leds[indicator->start]), indicator->length, CRGB(c, c, 0));
    else if (indicator->state == INDICATOR_STATE_5)
      fill_solid( &(leds[indicator->start]), indicator->length, CRGB(c, 0, c));
    else if (indicator->state == INDICATOR_STATE_6)
      fill_solid( &(leds[indicator->start]), indicator->length, CRGB(c, c, c));
  }
    DEBUG_PRINT(F("! "))
}

void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

void SetupBlackAndWhiteStripedPalette()
{
  fill_solid( currentPalette, 16, CRGB::Black);

  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

#endif