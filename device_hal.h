#ifndef DEVICE_HAL_H
#define DEVICE_HAL_H

//#include <FastLED.h>
#include "global.h"

void device_initialize(LightingProvider *l);
int stateHasChanged();
void resetVersion();

void check_and_increment_version(int oldVal, int newVal);

int device_getVersion();
int device_getState();
void device_setState(int s);
void device_setColor(EloColor c);
EloColor device_getColor();

void device_setAnimation(int a);
int device_getAnimation();

void device_setBrightness(int newBrightness);
int device_getBrightness();

int device_getParam1() ;
void device_setParam1(int p);
int device_getParam2();
void device_setParam2(int p);
int device_getParam3() ;
void device_setParam3(int p) ;
int device_getParam4() ;
void device_setParam4(int p) ;
int device_getParam5() ;
void device_setParam5(int p);

int device_getVar1() ;
void device_setVar1(int p);
int device_getVar2();
void device_setVar2(int p);
int device_getVar3() ;
void device_setVar3(int p) ;
int device_getVar4() ;
void device_setVar4(int p) ;
int device_getVar5() ;
void device_setVar5(int p);

int device_getCal1() ;
void device_setCal1(int c);
int device_getCal2();
void device_setCal2(int c);
int device_getCal3() ;
void device_setCal3(int c) ;
int device_getCal4() ;
void device_setCal4(int c) ;
int device_getCal5() ;
void device_setCal5(int c);

#endif