//#ifdef ELO_PERSIST

#include <ArduinoJson.h>
#include "FS.h"
#include "persist_hal.h"

#ifdef ELO_INDICATOR
const int IND_VERSION = 1;
#define IND_FILENAME "/ind.buf"
#endif

void persist_initialize()
{
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
  //SPIFFS.format();
  #ifdef ELO_INDICATOR
  #endif
}

#ifdef ELO_INDICATOR

bool loadIndicators(LightingProvider *lp)
{
    File configFile = SPIFFS.open(F(IND_FILENAME), "r");
    if (!configFile)
    {
        Serial.println(F("Failed to open indicator config file"));
        return false;
    }

    size_t size = configFile.size();
    if (size > 1024)
    {
        Serial.println(F("Indicator config file size is too large"));
        return false;
    }

    Serial.println(F("loading indicators..."));

    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    char *bufPtr = buf.get();

    int version = *((int *)bufPtr);
    bufPtr += sizeof(int);

    Serial.print(F("v="));
    Serial.print(version);
      
    lp->loadIndicators((byte *)bufPtr);

    configFile.close();    
}


bool saveIndicators(LightingProvider *lp)
{

    File file = SPIFFS.open(F(IND_FILENAME), "w");
    if (!file)
    {
        Serial.println(F("Failed to open config file for writing"));
        return false;
    }

    Serial.println(F("writing indicators..."));
    int version = IND_VERSION;
    int indicatorCount = lp->getIndicatorCount();

    file.write((uint8_t *)&version, sizeof(version));
    file.write((uint8_t *)&indicatorCount, sizeof(indicatorCount));

    if (indicatorCount > 0) {
      byte *indBuf = lp->getIndicatorBuffer();
      file.write((uint8_t *)indBuf, sizeof(Indicator) * indicatorCount);
    }
    file.close();
}
#endif

bool loadConfig(StateProvider *sp)
{
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile)
  {
    Serial.println(F("Failed to open config file"));
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024)
  {
    Serial.println(F("Config file size is too large"));
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<250> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(buf.get());

  if (!root.success())
  {
    Serial.println(F("Failed to parse config file"));
    return false;
  }

if (root.containsKey("s"))
    sp->setState(root.get<unsigned char>("s"));
  if (root.containsKey("l"))
    sp->setBrightness(root.get<unsigned char>("l"));
  if (root.containsKey("a"))
    sp->setAnimation(root.get<unsigned char>("a"));
  if (root.containsKey("c"))
  {
    JsonObject &color = root.get<JsonObject>("c");
    EloColor c =
        {
            color.get<unsigned char>("r"),
            color.get<unsigned char>("g"),
            color.get<unsigned char>("b"),
        };

    sp->setColor(c);
  }

  if (root.containsKey("p1"))
    sp->setParam1(root.get<unsigned char>("p1"));
  if (root.containsKey("p2"))
    sp->setParam2(root.get<unsigned char>("p2"));
  if (root.containsKey("p3"))
    sp->setParam3(root.get<unsigned char>("p3"));
  if (root.containsKey("p4"))
    sp->setParam4(root.get<unsigned char>("p4"));
  if (root.containsKey("p5"))
    sp->setParam5(root.get<unsigned char>("p5"));

  if (root.containsKey("c1"))
    sp->setCal1(root.get<int>("c1"));
  if (root.containsKey("c2"))
    sp->setCal2(root.get<int>("c2"));
  if (root.containsKey("c3"))
    sp->setCal3(root.get<int>("c3"));
  if (root.containsKey("c4"))
    sp->setCal4(root.get<int>("c4"));
  if (root.containsKey("c5"))
    sp->setCal5(root.get<int>("c5"));

  return true;
}

bool saveConfig(StateProvider *sp)
{
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();

  root["s"] = sp->getState();

  EloColor c = sp->getColor();
  JsonObject &color = root.createNestedObject("c");
  color["r"] = c.r;
  color["g"] = c.g;
  color["b"] = c.b;

  root["a"] = sp->getAnimation();
  root["l"] = sp->getBrightness();
  root["p1"] = sp->getParam1();
  root["p2"] = sp->getParam2();
  root["p3"] = sp->getParam3();
  root["p4"] = sp->getParam4();
  root["p5"] = sp->getParam5();

  root["c1"] = sp->getCal1();
  root["c2"] = sp->getCal2();
  root["c3"] = sp->getCal3();
  root["c4"] = sp->getCal4();
  root["c5"] = sp->getCal5();

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile)
  {
    Serial.println(F("Failed to open config file for writing"));
    return false;
  }

  root.printTo(configFile);
  return true;
}


//#endif