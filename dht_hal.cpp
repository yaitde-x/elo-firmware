#include "global.h"
#ifdef ELO_DHT // ignore the entire module

#include "DHT.h"
#include "dht_hal.h"

StateProvider *dhtStateProvider;
CommsProvider *dhtCommsProvider;
DHT *dht;

int dhtDataPin;
long lastReadingMillis = 0;
int sampleMs = DHT_DEFAULT_SAMPLE;
int lastTemp = 0;

void dht_setup(StateProvider *s, CommsProvider *c, int dataPin)
{
    dhtStateProvider = s;
    dhtCommsProvider = c;
    dhtDataPin = dataPin;

    dht = new DHT(dhtDataPin, DHTTYPE);
    dht->begin();
}

void dht_loop()
{

    long currentMillis = millis();

    if (currentMillis - lastReadingMillis > sampleMs)
    {

        lastReadingMillis = currentMillis;

        float h = dht->readHumidity();
        float t = dht->readTemperature();
        float f = dht->readTemperature(true);
        float c1 = dhtStateProvider->getCal1();
        float c2 = dhtStateProvider->getCal2();

        if (isnan(h) || isnan(t) || isnan(f)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        int v1 = int((f + c1) * 100);
        int v2 = int((h + c2) * 100);

        //if (v1 != lastTemp) {
            
            lastTemp = v1;

            // Compute heat index in Fahrenheit (the default)
            float hif = dht->computeHeatIndex(f, h);
            // Compute heat index in Celsius (isFahreheit = false)
            float hic = dht->computeHeatIndex(t, h, false);

            int v3 = int(hif * 100);

            dhtStateProvider->setVar1(v1);
            dhtStateProvider->setVar2(v2);
            dhtStateProvider->setVar3(v3);
            dhtCommsProvider->broadcastState();
        //}
    }
}

#endif
