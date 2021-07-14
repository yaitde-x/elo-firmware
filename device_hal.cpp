#include "device_hal.h"

EloColor White = {255, 255, 255};
EloColor Black = {0, 0, 0};

int state = 1;
struct EloColor color = White;
int ver = 1;
int brightness = BRIGHTNESS;
int animation = 0;
int param1 = 0;
int param2 = 0;
int param3 = 0;
int param4 = 0;
int param5 = 0;

int var1 = 0;
int var2 = 0;
int var3 = 0;
int var4 = 0;
int var5 = 0;

int cal1 = 0;
int cal2 = 0;
int cal3 = 0;
int cal4 = 0;
int cal5 = 0;

int curVersionWrite = 0;

LightingProvider *device_LightingProvider;

void device_initialize(LightingProvider *lp)
{
  device_LightingProvider = lp;
}

int stateHasChanged() {
  return curVersionWrite;
}

void resetVersion() {
  curVersionWrite = 0;
}

void check_and_increment_version(int oldVal, int newVal)
{
  if (oldVal != newVal)
    curVersionWrite++;
}

int device_getVersion()
{
  return ver;
}

int device_getState()
{
  return state;
}

void device_setState(int s)
{

  check_and_increment_version(state, s);
  state = s;

  if (state == 1)
  {
    device_LightingProvider->setAll(color);
  }
  else
  {
    device_LightingProvider->setAll(Black);
  }

  device_LightingProvider->show();
}

void device_setColor(EloColor c)
{

  if (c.b != color.b || c.r != color.r || c.g != color.g)
    curVersionWrite++;

  color = c;

  if (state == 1)
    device_LightingProvider->setAll(color);
}

EloColor device_getColor()
{
  return color;
}

void device_setAnimation(int a)
{
  check_and_increment_version(animation, a);
  animation = a;
}

int device_getAnimation()
{
  return animation;
}

int device_getParam1()
{
  return param1;
}

void device_setParam1(int p)
{
  check_and_increment_version(param1, p);
  param1 = p;
}

int device_getParam2()
{
  return param2;
}

void device_setParam2(int p)
{
  check_and_increment_version(param2, p);
  param2 = p;
}

int device_getParam3()
{
  return param3;
}

void device_setParam3(int p)
{
  check_and_increment_version(param3, p);
  param3 = p;
}

int device_getParam4()
{
  return param4;
}

void device_setParam4(int p)
{
  check_and_increment_version(param4, p);
  param4 = p;
}

int device_getParam5()
{
  return param5;
}

void device_setParam5(int p)
{
  check_and_increment_version(param5, p);
  param5 = p;
}

int device_getVar1()
{
  return var1;
}

void device_setVar1(int v)
{
  var1 = v;
}

int device_getVar2()
{
  return var2;
}

void device_setVar2(int v)
{
  var2 = v;
}

int device_getVar3()
{
  return var3;
}

void device_setVar3(int v)
{
  var3 = v;
}

int device_getVar4()
{
  return var4;
}

void device_setVar4(int v)
{
  var4 = v;
}

int device_getVar5()
{
  return var5;
}

void device_setVar5(int v)
{
  var5 = v;
}

int device_getCal1()
{
  return cal1;
}

void device_setCal1(int v)
{
  check_and_increment_version(cal1, v);
  cal1 = v;
}

int device_getCal2()
{
  return cal2;
}

void device_setCal2(int v)
{
  check_and_increment_version(cal2, v);
  cal2 = v;
}

int device_getCal3()
{
  return cal3;
}

void device_setCal3(int v)
{
  check_and_increment_version(cal3, v);
  cal3 = v;
}

int device_getCal4()
{
  return cal4;
}

void device_setCal4(int v)
{
  check_and_increment_version(cal4, v);
  cal4 = v;
}

int device_getCal5()
{
  return cal5;
}

void device_setCal5(int v)
{
  check_and_increment_version(cal5, v);
  cal5 = v;
}

void device_setBrightness(int newBrightness)
{
  check_and_increment_version(brightness, newBrightness);
  brightness = newBrightness;
  device_LightingProvider->show();
}

int device_getBrightness()
{
  return brightness;
}
