#ifndef MQTT_HAL_H
#define MQTT_HAL_H

#ifdef ELO_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include "global.h"
#include "net.h"
#include "device_hal.h"

void mqtt_initialize(StateProvider *, IndicatorProvider *ip, NetworkSettings* networkSettings);
void mqtt_monitorQueue();
void mqtt_loop();
void mqtt_broadcastState();
void mqtt_connect();
void mqtt_broadcastConfig();
void mqtt_processMessage(char *topic, char *msg);
#endif
