
#include "net.h"

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "global.h"

#ifdef ELO_UPDATE
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#endif

#define ELO_VERSION "0_0_1"
#define ELO_SPIFFS_VERSION "0_0_1"

#define NET_FILENAME "/dev.bin"

#define MODE_NONE 0
#define MODE_AP 1
#define MODE_CLIENT 2

NetworkSettings *networkSettings = new NetworkSettings();

#ifdef ELO_UPDATE
ESP8266WebServer *server;
#endif

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

const int port = 80;
int writeCount = 0;
int reset = 0;
int mode = MODE_NONE;
long lastUpdateCheck;

NetworkSettings *net_getNetworkSettings() {
    return networkSettings;
}

bool net_isAPMode()
{
    return mode == MODE_AP;
}

bool net_isClientMode()
{
    return mode == MODE_CLIENT;
}

void resetUpdateTimer() {
    lastUpdateCheck = millis() - UPDATE_CHECK_INTERVAL;
}

#ifdef ELO_UPDATE
void apiHandleNotFound()
{
    server->send(404, "text/plain", F("resource not found"));
}

void apiHandleGet()
{
    Serial.println("api get");

    StaticJsonBuffer<JSON_BUFSIZE> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["n"] = networkSettings->name;
    root["ssid"] = networkSettings->ssid;
    root["msvr"] = networkSettings->mqttServer;
    root["mport"] = networkSettings->mqttPort;
    root["musr"] = networkSettings->mqttUserName;
    root["hubsvr"] = networkSettings->hubServer;
    root["hubport"] = networkSettings->hubPort;
    root["dtype"] = networkSettings->deviceType;

    char buf[JSON_BUFSIZE];
    root.printTo(buf);
    server->send(200, "application/json", buf);
}

void apiHandlePost()
{
    Serial.println("api post");

    StaticJsonBuffer<JSON_BUFSIZE> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(server->arg("plain"));

    if (root.containsKey("n"))
        strcpy(networkSettings->name, root["n"]);

    if (root.containsKey("ssid"))
        strcpy(networkSettings->ssid, root["ssid"]);

    if (root.containsKey("pwd"))
        strcpy(networkSettings->networkPassword, root["pwd"]);

    if (root.containsKey("msvr"))
        strcpy(networkSettings->mqttServer, root["msvr"]);

    if (root.containsKey("mport"))
        networkSettings->mqttPort = root.get<int>("mport");

    if (root.containsKey("musr"))
        strcpy(networkSettings->mqttUserName, root["musr"]);

    if (root.containsKey("mpwd"))
        strcpy(networkSettings->mqttPassword, root["mpwd"]);

    if (root.containsKey("hubsvr"))
        strcpy(networkSettings->hubServer, root["hubsvr"]);

    if (root.containsKey("hubport"))
        networkSettings->hubPort = root.get<int>("hubport");

    if (root.containsKey("dtype"))
        strcpy(networkSettings->deviceType, root["dtype"]);

    // write the NetworkSettings
    writeCount++;

    server->send(200, "application/json", "");
}

void apiHandleRestart()
{
    reset++;
}
#endif

void initNetworkConfig()
{
    strcpy(networkSettings->ssid, "");
    strcpy(networkSettings->networkPassword, "");
    strcpy(networkSettings->mqttServer, "");
    strcpy(networkSettings->mqttUserName, "");
    strcpy(networkSettings->mqttPassword, "");
    strcpy(networkSettings->name, DEVICE_NAME);
    strcpy(networkSettings->deviceType, DEVICE_TYPE);
    networkSettings->mqttPort = 1883;
}

void initStaticNetworkConfig()
{
    strcpy(networkSettings->ssid, WLAN_SSID);
    strcpy(networkSettings->networkPassword, WLAN_PASS);
    strcpy(networkSettings->mqttServer, MQTT_SERVER);
    strcpy(networkSettings->name, DEVICE_NAME);
    strcpy(networkSettings->deviceType, DEVICE_TYPE);
    strcpy(networkSettings->mqttUserName, "");
    strcpy(networkSettings->mqttPassword, "");
    networkSettings->mqttPort = MQTT_SERVERPORT;
}

void printField(char *fieldName, char *value)
{
    Serial.print(fieldName);
    Serial.print(" : ");
    Serial.println(value);
}

void dumpNetworkConfig()
{
    printField("ssid", networkSettings->ssid);
    printField("pass", networkSettings->networkPassword);
    printField("mqttServer", networkSettings->mqttServer);
    Serial.print("Port : ");
    Serial.println(networkSettings->mqttPort);
    printField("mqttUser", networkSettings->mqttUserName);
    printField("mqttPass", networkSettings->mqttPassword);
    printField("name", networkSettings->name);
    printField("hubServer", networkSettings->hubServer);
    Serial.print("Port : ");
    Serial.println(networkSettings->hubPort);
    printField("deviceType", networkSettings->deviceType);

}

bool clearNetworkConfig()
{
    if (SPIFFS.exists(F(NET_FILENAME)))
        SPIFFS.remove(F(NET_FILENAME));
}

bool loadNetworkConfig()
{
    File configFile = SPIFFS.open(F(NET_FILENAME), "r");
    if (!configFile)
    {
        Serial.println(F("Failed to open device config file"));
        return false;
    }
    size_t size = configFile.size();

    Serial.print(F("loading "));
    Serial.print(size);
    Serial.println(F(" bytes."));

    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);

    memcpy(networkSettings, buf.get(), sizeof(NetworkSettings));

    configFile.close();
}

bool saveNetworkConfig()
{

    File file = SPIFFS.open(F(NET_FILENAME), "w");
    if (!file)
    {
        Serial.println(F("Failed to open device config file for writing"));
        return false;
    }

    Serial.println(F("writing device config..."));

    file.write((uint8_t *)networkSettings, sizeof(NetworkSettings));
    file.close();

    Serial.print("wrote ");
    Serial.print(sizeof(NetworkSettings));
    Serial.println(" bytes.");
}

void net_setUpdateServer(const char *svr, int port) {
    if (strcmp(svr, networkSettings->hubServer) == 0 && port == networkSettings->hubPort)
        return;
    
    strcpy(networkSettings->hubServer, svr);
    networkSettings->hubPort = port;

    saveNetworkConfig();
    resetUpdateTimer();
}

void net_initialize()
{

    #ifdef ELO_PERSIST
    if (!SPIFFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }

    //clearNetworkConfig();

    if (!SPIFFS.exists(F(NET_FILENAME)))
    {
        initializeApMode();
        //SPIFFS.remove(F(NET_FILENAME));
    }
    else
    {
        loadNetworkConfig();

        dumpNetworkConfig();

        initializeClientMode();
        resetUpdateTimer();
    }

    dumpNetworkConfig();
    #else
    initStaticNetworkConfig();
    initializeClientMode();
    #endif
}

void net_loop()
{
    #ifdef ELO_UPDATE
    if (server)
    {
        server->handleClient();

        #ifdef ELO_PERSIST
        if (writeCount > 0)
        {
            saveNetworkConfig();
            dumpNetworkConfig();
            writeCount = 0;
        }
        #endif
    }
    else
    {
        long curMillis = millis();

        if (curMillis - lastUpdateCheck < 0)
            lastUpdateCheck = curMillis;
        else if ((curMillis - lastUpdateCheck) > UPDATE_CHECK_INTERVAL)
        {
            lastUpdateCheck = curMillis;
            performUpdateCheck(true);
        }
    }
    #endif

    if (reset > 0) 
    {
        Serial.println("restarting");
        ESP.restart();
    }
}

#ifdef ELO_UPDATE
void initializeApMode()
{
    Serial.print("Setting up access point... ");

    mode = MODE_NONE;
    initNetworkConfig();
    strcpy(networkSettings->name, SERIAL_NUMBER);

    if (WiFi.softAPConfig(local_IP, gateway, subnet))
    {
        if (WiFi.softAP(networkSettings->name))
        {
            mode = MODE_AP;
            server = new ESP8266WebServer(port);

            server->on("/device", HTTP_GET, apiHandleGet);
            server->on("/device", HTTP_POST, apiHandlePost);
            server->on("/restart", HTTP_POST, apiHandleRestart);

            server->onNotFound(apiHandleNotFound);
            server->begin();

            Serial.print("server is listening on ");
            Serial.print(WiFi.softAPIP());
            Serial.print(":");
            Serial.println(port);
        }
        else
            Serial.println("failed to create access point!");
    }
    else
        Serial.println("failed to configure access point!");
}
#endif

void initializeClientMode()
{
    char ssid[20];

    strcpy(ssid, networkSettings->ssid);
    Serial.print("Connecting to ");
    Serial.print(ssid);

    mode = MODE_NONE;
    WiFi.begin(ssid, networkSettings->networkPassword);
    int cnt = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(F("."));
        cnt++;

        if (cnt > 120)
        {
            clearNetworkConfig();
            ESP.restart();
        }
    }

    mode = MODE_CLIENT;

    Serial.println(F(" connected."));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());
}

#ifdef ELO_UPDATE
bool performUpdateCheck(bool sketch = true)
{
    t_httpUpdate_return ret;
    char updateUrl[50];
    sprintf(updateUrl, "http://%s:%d/api/update/%s", networkSettings->hubServer, networkSettings->hubPort, networkSettings->deviceType);
    Serial.print(F("checking "));
    Serial.println(updateUrl);

    ESPhttpUpdate.rebootOnUpdate(false);
    if (sketch)
    {
        ret = ESPhttpUpdate.update(updateUrl, ELO_VERSION);
    }
    else
    {
        ret = ESPhttpUpdate.updateSpiffs(updateUrl, ELO_SPIFFS_VERSION);
    }
    if (ret != HTTP_UPDATE_NO_UPDATES)
    {
        if (ret == HTTP_UPDATE_OK)
        {
            Serial.printf("UPDATE SUCCEEDED");
            reset++;
            return true;
        }
        else
        {
            if (ret == HTTP_UPDATE_FAILED)
            {
                Serial.printf("Upgrade Failed");
            }
        }
    }
    else
        Serial.printf("no update");

    return false;
}
#endif
