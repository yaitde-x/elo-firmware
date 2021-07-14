#ifndef DHT_HAL_H
#define DHT_HAL_H


#define DHT_DEFAULT_SAMPLE 30000

void dht_setup(StateProvider *s, CommsProvider *c, int dataPin);
void dht_loop();

#endif
