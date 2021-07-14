#include "global.h"

#ifdef ELO_MQTT

#include <ArduinoJson.h>

#include "mqtt_hal.h"
#include "net.h"

WiFiClient *client;
Adafruit_MQTT_Client *mqtt;

//Setup a feed for publishing.
Adafruit_MQTT_Publish *configPub;
Adafruit_MQTT_Publish *statePub;
Adafruit_MQTT_Subscribe *updateSub;
StateProvider *mqttState;
IndicatorProvider *mqttIndicator;

const char CONFIG_FEED[] = "elo/" DEVICE_NAME "/config";
const char STATE_FEED[] = "elo/" DEVICE_NAME "/state";
const char UPDATE_FEED[] = "elo/" DEVICE_NAME "/update";
const char INDICATOR_FEED[] = "elo/indicator";

void mqtt_processUpdateStateMessage(char *msg);
void mqtt_processUpdateConfigMessage(char *msg);

void mqtt_initialize(StateProvider *s, IndicatorProvider *ip, NetworkSettings* networkSettings)
{

  mqttState = s;
  mqttIndicator = ip;

#ifdef MQTT_NET
  // Connect to WiFi access point.
  DEBUG_PRINTLN();
  DEBUG_PRINTLN();
  DEBUG_PRINTLN("Connecting to ");
  DEBUG_PRINTLN(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    DEBUG_PRINT(F("."));
  }

  DEBUG_PRINTLN();
  DEBUG_PRINTLN(F("WiFi connected"));
  DEBUG_PRINTLN(F("IP address: "));
  DEBUG_PRINTLN(WiFi.localIP());
  #endif

  client = new WiFiClient();
  mqtt = new Adafruit_MQTT_Client(client, networkSettings->mqttServer, networkSettings->mqttPort, networkSettings->mqttUserName, networkSettings->mqttPassword);

  // publishing state
  configPub = new Adafruit_MQTT_Publish(mqtt, CONFIG_FEED);
  statePub = new Adafruit_MQTT_Publish(mqtt, STATE_FEED);
  updateSub = new Adafruit_MQTT_Subscribe(mqtt, UPDATE_FEED);

  mqtt->subscribe(updateSub);
  mqtt_connect();
}

void mqtt_loop()
{

  mqtt_monitorQueue();

  mqtt_broadcastConfig();
}

void mqtt_monitorQueue()
{

  mqtt_connect();
  //DEBUG_PRINT(F("check for updates "));
  Adafruit_MQTT_Subscribe *subscription = mqtt->readSubscription(1);
  /*//Serial.print((int)subscription, 16);
  //DEBUG_PRINT(F(" = "));
  if (subscription)
  {
    DEBUG_PRINT((char *)subscription->lastread);
    mqtt_processMessage((char *)subscription->topic, (char *)subscription->lastread);
  }
  //} else
  //  DEBUG_PRINT(F("."));
  */
}

void mqtt_processMessage(char *topic, char *msg)
{

  DEBUG_PRINT(topic);
  DEBUG_PRINT(F(" "));
  DEBUG_PRINTLN(msg);

  StaticJsonBuffer<JSON_BUFSIZE> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(msg);

  // device networkSettings
  #ifdef ELO_UPDATE
  if (root.containsKey("hs")) {
    const char *hs = root["hs"];
    int port = root.get<int>("hp");
    net_setUpdateServer(hs, port);
  }
  #endif
  
  if (root.containsKey("s"))
    mqttState->setState(root.get<unsigned char>("s"));
  if (root.containsKey("l"))
    mqttState->setBrightness(root.get<unsigned char>("l"));
  if (root.containsKey("a"))
    mqttState->setAnimation(root.get<unsigned char>("a"));
  if (root.containsKey("c"))
  {
    JsonObject &color = root.get<JsonObject>("c");
    EloColor c =
        {
            color.get<unsigned char>("r"),
            color.get<unsigned char>("g"),
            color.get<unsigned char>("b"),
        };

    mqttState->setColor(c);
  }

  if (root.containsKey("p1"))
    mqttState->setParam1(root.get<unsigned char>("p1"));
  if (root.containsKey("p2"))
    mqttState->setParam2(root.get<unsigned char>("p2"));
  if (root.containsKey("p3"))
    mqttState->setParam3(root.get<unsigned char>("p3"));
  if (root.containsKey("p4"))
    mqttState->setParam4(root.get<unsigned char>("p4"));
  if (root.containsKey("p5"))
    mqttState->setParam5(root.get<unsigned char>("p5"));

  if (root.containsKey("v1"))
    mqttState->setVar1(root.get<int>("v1"));
  if (root.containsKey("v2"))
    mqttState->setVar2(root.get<int>("v2"));
  if (root.containsKey("v3"))
    mqttState->setVar3(root.get<int>("v3"));
  if (root.containsKey("v4"))
    mqttState->setVar4(root.get<int>("v4"));
  if (root.containsKey("v5"))
    mqttState->setVar5(root.get<int>("v5"));

  if (root.containsKey("c1"))
    mqttState->setCal1(root.get<int>("c1"));
  if (root.containsKey("c2"))
    mqttState->setCal2(root.get<int>("c2"));
  if (root.containsKey("c3"))
    mqttState->setCal3(root.get<int>("c3"));
  if (root.containsKey("c4"))
    mqttState->setCal4(root.get<int>("c4"));
  if (root.containsKey("c5"))
    mqttState->setCal5(root.get<int>("c5"));

  if (root.containsKey("i"))
  {
    DEBUG_PRINT(F("update->ind_state"));
    byte indicatorIndex = root.get<byte>("i");
    byte indicatorState = 0;
    byte indicatorLevel = 0;
    byte ledIndex = 0;
    byte ledPos = 0;
    byte ledCnt = 0;

    if (root.containsKey("li"))
    {
      ledIndex = root.get<unsigned char>("li");

      if (root.containsKey("lp"))
        ledPos = root.get<unsigned char>("lp");

      if (root.containsKey("lc"))
        ledCnt = root.get<unsigned char>("lc");

      mqttIndicator->configIndicator(indicatorIndex, ledPos, ledCnt);
    }

    if (root.containsKey("is"))
    {
      indicatorState = root.get<unsigned char>("is");

      if (root.containsKey("il"))
        indicatorLevel = root.get<unsigned char>("il");

      mqttIndicator->setIndicator(indicatorIndex, indicatorState, indicatorLevel);
    }
  }
}

void mqtt_processUpdateConfigMessage(char *msg)
{
  DEBUG_PRINT(F(" ->config "));
}

void mqtt_processUpdateStateMessage(char *msg)
{
  DEBUG_PRINT(F(" ->state "));
  StaticJsonBuffer<JSON_BUFSIZE> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(msg);

  if (root.containsKey("v1"))
    mqttState->setVar1(root.get<int>("v1"));
  if (root.containsKey("v2"))
    mqttState->setVar2(root.get<int>("v2"));
  if (root.containsKey("v3"))
    mqttState->setVar3(root.get<int>("v3"));
  if (root.containsKey("v4"))
    mqttState->setVar4(root.get<int>("v4"));
  if (root.containsKey("v5"))
    mqttState->setVar5(root.get<int>("v5"));

  if (root.containsKey("c1"))
    mqttState->setCal1(root.get<int>("c1"));
  if (root.containsKey("c2"))
    mqttState->setCal2(root.get<int>("c2"));
  if (root.containsKey("c3"))
    mqttState->setCal3(root.get<int>("c3"));
  if (root.containsKey("c4"))
    mqttState->setCal4(root.get<int>("c4"));
  if (root.containsKey("c5"))
    mqttState->setCal5(root.get<int>("c5"));
}

void mqtt_broadcastState()
{
  DEBUG_PRINT(F(" state-> "));
  mqtt_connect();

  StaticJsonBuffer<JSON_BUFSIZE> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["n"] = DEVICE_NAME;
  root["v1"] = mqttState->getVar1();
  root["v2"] = mqttState->getVar2();
  root["v3"] = mqttState->getVar3();
  root["v4"] = mqttState->getVar4();
  root["v5"] = mqttState->getVar5();

  char buf[JSON_BUFSIZE];
  root.printTo(buf);
  statePub->publish(buf);
}

long lastMillis = 0L;
char configBuf[400];

void mqtt_broadcastConfig()
{

  long curMillis = millis();

  if (curMillis - lastMillis > 5000)
  {
    DEBUG_PRINT(F(" config-> "));
    mqtt_connect();

    StaticJsonBuffer<400> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["n"] = DEVICE_NAME;
    root["s"] = mqttState->getState();
    root["v"] = mqttState->getVersion();

    EloColor c = mqttState->getColor();
    JsonObject &color = root.createNestedObject("c");
    color["r"] = c.r;
    color["g"] = c.g;
    color["b"] = c.b;

    root["a"] = mqttState->getAnimation();
    root["l"] = mqttState->getBrightness();

    root["c1"] = mqttState->getCal1();
    root["c2"] = mqttState->getCal2();
    root["c3"] = mqttState->getCal3();
    root["c4"] = mqttState->getCal4();
    root["c5"] = mqttState->getCal5();
    
    root["p1"] = mqttState->getParam1();
    root["p2"] = mqttState->getParam2();
    root["p3"] = mqttState->getParam3();
    root["p4"] = mqttState->getParam4();
    root["p5"] = mqttState->getParam5();

    root.printTo(configBuf);
    configPub->publish(configBuf);

    lastMillis = curMillis;
  }
}

void mqtt_connect()
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt->connected())
  {
    return;
  }

  DEBUG_PRINT(F("Connecting to MQTT... "));

  uint8_t retries = 3;
  while ((ret = mqtt->connect()) != 0)
  { // connect will return 0 for connected
    DEBUG_PRINTLN(mqtt->connectErrorString(ret));
    DEBUG_PRINTLN("Retrying MQTT connection in 5 seconds...");
    mqtt->disconnect();
    delay(5000); // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  DEBUG_PRINTLN(F("MQTT Connected!"));
}

#endif