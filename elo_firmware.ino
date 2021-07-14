
const int BAUD_RATE = 115200;

#include "build.h"
#include "device_type.h"
#include "global.h"
#include "device_hal.h"
#include "net.h"

#ifdef ELO_LED
#include "led_hal.h"
#endif

#ifdef ELO_SERIAL
#include "serial_hal.h"
#endif

#ifdef ELO_MQTT
#include "mqtt_hal.h"
#endif

#ifdef ELO_HX711
#include "hx711_hal.h"
#endif

#ifdef ELO_PIR
#include "pir_hal.h"
#endif

#ifdef ELO_DHT
#include "dht_hal.h"
#endif

#ifdef ELO_HIGROW
#include "higrow_hal.h"
#endif

#ifdef ELO_PERSIST
#include "persist_hal.h"
#endif

#ifdef ELO_BUTTON
#include "button_hal.h"
#endif

StateProvider *deviceState = getStateProvider();
LightingProvider *deviceLighting = getLightingProvider();
CommsProvider *commsProvider = getCommsProvider();
IndicatorProvider *indicatorProvider = getIndicatorProvider();

void initializeFirmware()
{
  device_initialize(deviceLighting);

#ifdef ELO_PERSIST
  persist_initialize();
#endif

#ifdef ELO_SERIAL
  Serial.println(F("serial initialize..."));
  serial_initialize(deviceState);
#endif

#ifdef ELO_MQTT
  Serial.println(F("mqtt initialize..."));
  mqtt_initialize(deviceState, indicatorProvider, net_getNetworkSettings());
#endif

#ifdef ELO_LED
  Serial.println(F("led initialize..."));
  led_initialize(deviceState);
#endif

#ifdef ELO_HX711
  hx711_setup(deviceState, commsProvider);
#endif

#ifdef ELO_PIR
  pir_setup(deviceState, commsProvider, PIR_DATAPIN);
#endif

#ifdef ELO_DHT
  dht_setup(deviceState, commsProvider, DHT_DATAPIN);
#endif

#ifdef ELO_HIGROW
  higrow_setup(deviceState, commsProvider, HIGROW_SOIL_PIN);
#endif

  Serial.println(F("calibrate..."));
#ifdef ELO_HIGROW1
  deviceState->setCal1(-5);
#endif

#ifdef ELO_HIGROW2
  deviceState->setCal1(-5);
#endif

#ifdef ELO_TEMP1
  deviceState->setCal1(0);
#endif

#ifdef ELO_PERSIST
  Serial.println(F("load config..."));
  loadConfig(deviceState);
#ifdef ELO_INDICATOR
  Serial.println(F("load indicator config..."));
  loadIndicators(deviceLighting);
#endif
#endif

#ifdef ELO_BUTTON
button_initialize(deviceState, commsProvider);
#endif
}

void setup()
{

  Serial.begin(BAUD_RATE);
  delay(1000);

  Serial.println(F("initialize..."));

  #ifndef MQTT_NET
  net_initialize();

  if (net_isClientMode())
    initializeFirmware();
  #else
    initStaticNetworkConfig();
    initializeFirmware();
  #endif

  Serial.println(F("initialize complete..."));
}

void loop()
{

#ifndef MQTT_NET
  if (net_isClientMode())
  {
#endif

#ifdef ELO_SERIAL
    serial_loop();
#endif

#ifdef ELO_LED
    //led_animateFrame();
#endif

#ifdef ELO_HX711
    hx711_loop();
#endif

#ifdef ELO_PIR
    pir_loop();
#endif

#ifdef ELO_HIGROW
    higrow_loop();
#endif

#ifdef ELO_DHT
    dht_loop();
#endif

#ifdef ELO_BUTTON
    button_loop();
#endif

#ifdef ELO_MQTT
    mqtt_loop();
#endif

#ifdef ELO_PERSIST
    if (deviceState->stateHasChanged() > 0)
    {
      Serial.println(F("state has changed. persisting..."));
      saveConfig(deviceState);
      deviceState->resetVersion();
    }
  #ifdef ELO_INDICATOR
      if (deviceLighting->stateHasChanged() > 0)
      {
        Serial.println(F("indicator state has changed. persisting..."));
        saveIndicators(deviceLighting);
        deviceLighting->resetVersion();
      }
  #endif
#endif

#ifndef MQTT_NET
  }
#endif

//net_loop();

}
