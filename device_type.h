#ifndef _device_type_h_
#define _device_type_h_

#include "build.h"

// these defines include various 'hals'
//#define ELO_SERIAL
//#define ELO_LED
//#define ELO_MQTT
#define MQTT_DEBUG
#define MQTT_NET  // define this to use MQTT as the network initializer
//#define ELO_HX711
//#define ELO_INDICATOR
//#define ELO_PIR
//#define ELO_DHT
//#define ELO_HIGROW
//#define ELO_PERSIST
//#define ELO_UPDATE
//#define ELO_NET

#define DEVICE_TYPE "UNKOWN"

#ifdef ELO_IND1
#define ELO_LED
#define ELO_MQTT
#define ELO_INDICATOR
#define NUM_LEDS 1 //(15*20)
#define DEVICE_NAME "elo_ind1"
#endif

#ifdef ELO_TEST
#define ELO_LED
#define ELO_MQTT
#define ELO_INDICATOR
#define NUM_LEDS 10 //(15*20)
#define DEVICE_NAME "elo_test"
#endif

#ifdef ELO_MQTT_TEST
#define ELO_MQTT
#define DEVICE_NAME "elo_mqtt_test"
#endif

#ifdef ELO_WHITEBOARD
#define ELO_LED
#define ELO_MQTT
#define ELO_INDICATOR
#define NUM_LEDS 218
#define DEVICE_NAME "elo_wb"
#define BLD_IND
#define DF_IND
#define MAX_INDICATORS 10
#define DEVICE_TYPE "LED"
#endif

#ifdef ELO_TREK
#define ELO_LED
#define ELO_MQTT
#define ELO_INDICATOR
#define NUM_LEDS 4
#define DEVICE_NAME "elo_trek"
#define MAX_INDICATORS 10
#define DEVICE_TYPE "LED"
#endif

#ifdef ELO_TREE1
#define DEVICE_NAME "elo_tree1"
#define ELO_LED
#define ELO_MQTT
#define ELO_INDICATOR
#define NUM_LEDS 50
#define MAX_INDICATORS 10
#define DEVICE_TYPE "LED"
#endif

#ifdef ELO_DFMON
#define ELO_HX711
#define ELO_MQTT
#define NUM_LEDS 0
#define DEVICE_NAME "elo_dfmon"
#define DEVICE_TYPE "LOAD"
#endif

#ifdef ELO_PIR1
#define ELO_MQTT
#define ELO_PIR
#define NUM_LEDS 0
#define DEVICE_NAME "elo_pir1"
#define PIR_DATAPIN D7
#define ELO_DHT
#define DHT_DATAPIN D3
#define DHTTYPE DHT11
#define DEVICE_TYPE "MOTION"
#endif

#ifdef ELO_TEMP1
#define ELO_MQTT
#define ELO_DHT
#define DEVICE_NAME "elo_temp1"
#define DHT_DATAPIN D7
#define DHTTYPE DHT11
#define DEVICE_TYPE "TEMP"
#endif

#ifdef ELO_DISP1
#define ELO_LED
#define ELO_MQTT
#define ELO_INDICATOR
#define NUM_LEDS (15 * 20)
#define DEVICE_NAME "elo_disp1"
#define MAX_INDICATORS 10
#define DEVICE_TYPE "LED"
#endif

#ifdef ELO_HIGROW1

#define ELO_MQTT
#define ELO_DHT
#define ELO_HIGROW
#define DEVICE_NAME "elo_higrow1"
#define DHT_DATAPIN 22
#define DHTTYPE DHT11
#define HIGROW_SOIL_PIN 32
#define HIGROW_POWER_PIN 34
#define HIGROW_LIGHT_PIN 33
#define DEVICE_TYPE "HIGROW"
#endif

#ifdef ELO_HIGROW2

#define ELO_MQTT
#define ELO_DHT
#define ELO_HIGROW
#define DEVICE_NAME "elo_higrow2"
#define DHT_DATAPIN 22
#define DHTTYPE DHT11
#define HIGROW_SOIL_PIN 32
#define HIGROW_POWER_PIN 34
#define HIGROW_LIGHT_PIN 33
#define DEVICE_TYPE "HIGROW"
#endif

#ifdef ELO_HIGROW3

#define ELO_MQTT
#define ELO_DHT
#define ELO_HIGROW
#define DEVICE_NAME "elo_higrow3"
#define DHT_DATAPIN 22
#define DHTTYPE DHT11
#define HIGROW_SOIL_PIN 32
#define HIGROW_POWER_PIN 34
#define HIGROW_LIGHT_PIN 33
#define DEVICE_TYPE "HIGROW"
#endif

#ifdef ELO_FRANK1
#define ELO_MQTT
#define ELO_PIR
#define NUM_LEDS 0
#define DEVICE_NAME "elo_frank1"
#define PIR_DATAPIN D7
#define ELO_DHT
#define DHT_DATAPIN D3
#define DHTTYPE DHT11
#define DEVICE_TYPE "FRANK"
#endif

#define BRIGHTNESS 50
//#define UPDATE_CHECK_INTERVAL (60 * 60 * 1000 * 24) // daily
#define UPDATE_CHECK_INTERVAL (60 * 60 * 1000) // hourly
//#define UPDATE_CHECK_INTERVAL (1000 * 60) // minute

#ifdef ELO_LED
#define LED_PIN D3
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

// whether to enable animations.. old stuff
#define ELO_ANIM
#define UPDATES_PER_SECOND 100
#endif

// define these to use MQTT
#ifndef ELO_PERSIST
#define WLAN_SSID "plasma-home"
#define WLAN_PASS "98e9elpjj"

#define MQTT_SERVER "192.168.1.82"
#define MQTT_SERVERPORT 1883 // use 8883 for SSL
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#else
#define WLAN_SSID ""
#define WLAN_PASS ""

#define MQTT_SERVER ""
#define MQTT_SERVERPORT 1883 // use 8883 for SSL
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#endif

#define SERIAL_NUMBER "ELO_DF6D3EAB"

#endif