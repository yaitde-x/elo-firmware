#ifndef _net_ap_h_
#define _net_ap_h_

/*
void dumpNetworkConfig();
bool clearNetworkConfig();
bool loadNetworkConfig();
bool saveNetworkConfig();
*/

typedef struct
{
    char ssid[20];
    char networkPassword[20];
    char mqttServer[20];
    int mqttPort;
    char mqttUserName[20];
    char mqttPassword[20];
    char name[20];
    char hubServer[20];
    int hubPort;
    char deviceType[15];
} NetworkSettings;

void initializeClientMode();
void net_initialize();
void net_loop();
NetworkSettings *net_getNetworkSettings();
bool net_isAPMode();
bool net_isClientMode();
void initStaticNetworkConfig();

#ifdef ELO_UPDATE
void initializeApMode();
void net_setUpdateServer(const char *svr, int port);
bool performUpdateCheck(bool sketch);
#endif

#endif