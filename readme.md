# elo_firmware
---

I have been building this from VS Code. Have no idea if you will be able to build it out of the box. This is a generic firmware I used in conjunction with the elo_hub repo.

## MQTT Pub/Sub Examples
```bash
mosquitto_pub -h pi3_hub -t "elo/elo_trek/update" -m "{ \"n\" : \"elo_trek\", \"i\" : 0, \"li\": 0, \"lp\": 0, \"lc\": 4 }"
```

You will need to create a custom.h header file to define your device specific settings.

```c++
#ifndef CUSTOM_H_
#define CUSTOM_H_

// define these to use MQTT
#define WLAN_SSID ""
#define WLAN_PASS ""

#define MQTT_SERVER "mqtt ip address"
#define MQTT_SERVERPORT 1883 // use 8883 for SSL
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""

//###########################################################
// these are all the different devices i can compile for
//###########################################################

//#define ELO_IND1
//#define ELO_WHITEBOARD
//#define ELO_TREK
//#define ELO_TEST
//#define ELO_DFMON
//#define ELO_PIR1
#define ELO_TEMP1 // master closet, WeMos D1 Mini
//#define ELO_DISP1
//#define ELO_HIGROW1 //kitchen, ESP32
//#define ELO_HIGROW2 //jessie's room, ESP32

//###########################################################
// These are the different HAL layers a device can run
//###########################################################

//#define ELO_SERIAL
//#define ELO_LED
//#define ELO_MQTT
//#define MQTT_DEBUG
//#define ELO_HX711
//#define ELO_INDICATOR
//#define ELO_PIR
//#define ELO_DHT
//#define ELO_HIGROW

//###########################################################
// Here is where you specify the configuration for each device
//###########################################################

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

#ifdef ELO_WHITEBOARD
    #define ELO_LED
    #define ELO_MQTT
    #define ELO_INDICATOR
    #define NUM_LEDS 218
    #define DEVICE_NAME "elo_wb"
    #define BLD_IND
    #define DF_IND
    #define MAX_INDICATORS 10
#endif

#ifdef ELO_TREK
    #define ELO_LED
    #define ELO_MQTT
    #define ELO_INDICATOR
    #define NUM_LEDS 4
    #define DEVICE_NAME "elo_trek"
    #define MAX_INDICATORS 1
#endif

#ifdef ELO_DFMON
    #define ELO_HX711
    #define ELO_MQTT
    #define NUM_LEDS 0
    #define DEVICE_NAME "elo_dfmon"
#endif

#ifdef ELO_PIR1
    #define ELO_MQTT
    #define ELO_PIR
    #define NUM_LEDS 0
    #define DEVICE_NAME "elo_pir1"
    #define PIR_DATAPIN D7
#endif

#ifdef ELO_TEMP1
    #define ELO_MQTT
    #define ELO_DHT
    #define DEVICE_NAME "elo_temp1"
    #define DHT_DATAPIN D7
    #define DHTTYPE DHT11 
#endif

#ifdef ELO_DISP1
    #define ELO_LED
    #define ELO_MQTT
    #define ELO_INDICATOR
    #define NUM_LEDS (15*20)
    #define DEVICE_NAME "elo_disp1"
    #define MAX_INDICATORS 10
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

#endif

#define BRIGHTNESS  50

#ifdef ELO_LED
    #define LED_PIN     D3
    #define LED_TYPE    WS2812
    #define COLOR_ORDER GRB

    // whether to enable animations.. old stuff
    #define ELO_ANIM
    #define UPDATES_PER_SECOND 100
#endif

#endif
```