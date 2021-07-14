#include "device_hal.h"
#include "mock.h"

#ifdef ELO_LED
#include "led_hal.h"
#endif

#ifdef ELO_MQTT
#include "mqtt_hal.h"
#endif

StateProvider *getStateProvider()
{
    StateProvider *s = new StateProvider();
    s->getVersion = device_getVersion;
    s->getState = device_getState;
    s->setState = device_setState;
    s->getColor = device_getColor;
    s->setColor = device_setColor;

    s->stateHasChanged = stateHasChanged;
    s->resetVersion = resetVersion;

    s->setAnimation = device_setAnimation;
    s->getAnimation = device_getAnimation;
    s->setBrightness = device_setBrightness;
    s->getBrightness = device_getBrightness;
    s->getParam1 = device_getParam1;
    s->setParam1 = device_setParam1;
    s->getParam2 = device_getParam2;
    s->setParam2 = device_setParam2;
    s->getParam3 = device_getParam3;
    s->setParam3 = device_setParam3;
    s->getParam4 = device_getParam4;
    s->setParam4 = device_setParam4;
    s->getParam5 = device_getParam5;
    s->setParam5 = device_setParam5;

    s->getVar1 = device_getVar1;
    s->setVar1 = device_setVar1;
    s->getVar2 = device_getVar2;
    s->setVar2 = device_setVar2;
    s->getVar3 = device_getVar3;
    s->setVar3 = device_setVar3;
    s->getVar4 = device_getVar4;
    s->setVar4 = device_setVar4;
    s->getVar5 = device_getVar5;
    s->setVar5 = device_setVar5;

    s->getCal1 = device_getCal1;
    s->setCal1 = device_setCal1;
    s->getCal2 = device_getCal2;
    s->setCal2 = device_setCal2;
    s->getCal3 = device_getCal3;
    s->setCal3 = device_setCal3;
    s->getCal4 = device_getCal4;
    s->setCal4 = device_setCal4;
    s->getCal5 = device_getCal5;
    s->setCal5 = device_setCal5;

    return s;
}

LightingProvider *getLightingProvider()
{
    LightingProvider *l = new LightingProvider();

#ifdef ELO_LED
    l->setAll = led_setAll;
    l->show = led_showStrip;
    l->loadIndicators = led_loadIndicators;
    l->getIndicatorCount = led_getIndicatorCount;
    l->getIndicatorBuffer = led_getIndicatorBuffer;
    l->stateHasChanged = led_stateHasChanged;
    l->resetVersion = led_resetVersion;

#else
    l->setAll = mock_setAll;
    l->show = mock_show;
    l->loadIndicators = mock_loadIndicators;
    l->getIndicatorCount = mock_getIndicatorCount;
    l->getIndicatorBuffer = mock_getIndicatorBuffer;
    l->stateHasChanged = mock_stateHasChanged;
    l->resetVersion = mock_resetVersion;

#endif

    return l;
}

CommsProvider *getCommsProvider()
{
    CommsProvider *c = new CommsProvider();

#ifdef ELO_MQTT
    c->broadcastState = mqtt_broadcastState;
#else
    c->broadcastState = mock_broadcastState;
#endif

    return c;
}

IndicatorProvider *getIndicatorProvider()
{
    IndicatorProvider *c = new IndicatorProvider();

#ifdef ELO_LED
    #ifdef ELO_INDICATOR
        c->setIndicator = led_setIndicator;
        c->configIndicator = led_configIndicator;
    #else
        c->setIndicator = mock_setIndicator;
        c->configIndicator = mock_configIndicator;
    #endif
#else
    c->setIndicator = mock_setIndicator;
    c->configIndicator = mock_configIndicator;
#endif

    return c;
}
