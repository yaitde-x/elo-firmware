#ifdef ELO_SERIAL

#include "serial_hal.h"
#include "led_hal.h"

CmdMessenger c = CmdMessenger(Serial);
StateProvider *serialState;

/* Define available CmdMessenger commands */
enum {
  dev_status, //0
  dev_on, //1
  dev_off, //2
  dev_level, //3
  dev_color, //4
  dev_anim, //5
  dev_telem, //6
  dev_config, //7
  dev_indicator, //8
  dev_reset //9
};

void emit_status(void) {
  Serial.print(F(","));
  //Serial.print(getState());
  Serial.print(serialState->getState());
  Serial.print(F(","));
  //Serial.print(getBrightness());
  Serial.print(serialState->getBrightness());
  Serial.print(F(","));
  //CRGB cc = getColor();
  EloColor color = serialState->getColor();
  Serial.print(color.r);
  Serial.print(F(","));
  Serial.print(color.g);
  Serial.print(F(","));
  Serial.print(color.b);
  Serial.print(F(","));
  //Serial.print(getAnimation());
  Serial.print(serialState->getAnimation());
  Serial.print(F(","));
  //Serial.print(getParam1());
  Serial.print(serialState->getParam1());
  Serial.print(F(","));
  //Serial.print(getParam2());
  Serial.print(serialState->getParam2());
  Serial.print(F(","));
  //Serial.print(getParam3());
  Serial.print(serialState->getParam3());
  Serial.print(F(","));
  //Serial.print(getParam4());
  Serial.print(serialState->getParam4());
  Serial.print(F(","));
  //Serial.print(getParam5());
  Serial.print(serialState->getParam5());
  Serial.print(F(","));
  //Serial.println(getVersion());
  Serial.println(device_getVersion());
  //Serial.println();
}

void send_dev_status(void) {
  Serial.print(10);
  emit_status();
}

void on_dev_status(void) {
  char* uuid = c.readStringArg();
  Serial.print(uuid);
  emit_status();
}

void on_dev_on(void) {
  char* uuid = c.readStringArg();

  serialState->setState(1);

  Serial.print(uuid);
  emit_status();
}

void on_dev_off(void) {

  char* uuid = c.readStringArg();

  serialState->setState(0);

  Serial.print(uuid);
  emit_status();
}

void on_dev_level(void) {

  char* uuid = c.readStringArg();
  int brightness = atoi(c.readStringArg());

  if (brightness > 255)
    brightness = 255;
  if (brightness < 0)
    brightness = 0;

  serialState->setBrightness(brightness);

  Serial.print(uuid);
  emit_status();
}

void on_dev_color(void) {

  char* uuid = c.readStringArg();

  int r = atoi(c.readStringArg());
  int g = atoi(c.readStringArg());
  int b = atoi(c.readStringArg());

  if (r > 255)
    r = 255;
  if (g > 255)
    g = 255;
  if (b > 255)
    b = 255;

  if (r < 0)
    r = 0;
  if (g < 0)
    g = 0;
  if (b < 0)
    b = 0;

  serialState->setColor({r, g, b});

  Serial.print(uuid);
  emit_status();
}

void on_dev_anim(void) {

  char* uuid = c.readStringArg();

  int anim = atoi(c.readStringArg());
  int p1 = atoi(c.readStringArg());
  int p2 = atoi(c.readStringArg());
  int p3 = atoi(c.readStringArg());
  int p4 = atoi(c.readStringArg());
  int p5 = atoi(c.readStringArg());

  if ((anim != serialState->getAnimation())
      || (p1 != serialState->getParam1())
      || (p2 != serialState->getParam2())
      || (p3 != serialState->getParam3())
      || (p4 != serialState->getParam4())
      || (p5 != serialState->getParam5())) {

    serialState->setAnimation ( anim);
    serialState->setParam1 ( p1);
    serialState->setParam2 ( p2);
    serialState->setParam3 ( p3);
    serialState->setParam4 ( p4);
    serialState->setParam5 ( p5);

    led_changeAnimation(serialState->getAnimation(), serialState->getParam1());
  }

  Serial.print(uuid);
  emit_status();
}

void on_dev_telem(void) {

  char* uuid = c.readStringArg();
  int audio = 0;
  int frameCount = 0;
  int frameMillis = 0;

  Serial.print(uuid);
  Serial.print(F(","));
  Serial.print(frameCount);
  Serial.print(F(","));
  Serial.print(frameMillis);
  Serial.print(F(","));

  Serial.print(audio);
  Serial.print(F(","));
  Serial.println(device_getVersion());
}

void on_dev_config(void) {

  char* uuid = c.readStringArg();

  //state: 0, brightness: 255, r: 255, g: 255, b: 255, p1: 0, p2:0, p3:0, p4:0, p5:0, anim: 0
  serialState->setState(c.readInt16Arg());

  serialState->setBrightness(c.readInt16Arg());

  int r = c.readInt16Arg();
  int g = c.readInt16Arg();
  int b = c.readInt16Arg();

  serialState->setColor({r, g, b});

  serialState->setParam1 ( c.readInt16Arg());
  serialState->setParam2 ( c.readInt16Arg());
  serialState->setParam3 ( c.readInt16Arg());
  serialState->setParam4 ( c.readInt16Arg());
  serialState->setParam5 ( c.readInt16Arg());
  serialState->setAnimation ( c.readInt16Arg());

  Serial.print(uuid);
  emit_status();
}

void on_dev_indicator(void) {

  char* uuid = c.readStringArg();

  int indicatorIndex = c.readInt16Arg(); 
  int state = c.readInt16Arg();
  int level = c.readInt16Arg();

  led_setIndicator(indicatorIndex, state, level);

  Serial.println(uuid);
}

void on_unknown_command(void) {
  //c.sendCmd(dev_error,"ER1");
}

void on_dev_reset(void) {

  char* uuid = c.readStringArg();

  serialState->setState(1);

  serialState->setBrightness(BRIGHTNESS);

  serialState->setAnimation (0);
  serialState->setParam1 (0);
  serialState->setParam2 (0);
  serialState->setParam3 (0);
  serialState->setParam4 (0);
  serialState->setParam5 (0);
  serialState->setColor(White);

  Serial.print(uuid);
  emit_status();
}

void serial_initialize(StateProvider *s) {
  serialState = s;

  c.attach(dev_status, on_dev_status);
  c.attach(dev_on, on_dev_on);
  c.attach(dev_off, on_dev_off);
  c.attach(dev_level, on_dev_level);
  c.attach(dev_color, on_dev_color);
  c.attach(dev_anim, on_dev_anim);
  c.attach(dev_telem, on_dev_telem);
  c.attach(dev_config, on_dev_config);
  c.attach(dev_indicator, on_dev_indicator);
  c.attach(dev_reset, on_dev_reset);
  c.attach(on_unknown_command);
}

void serial_loop() {
    c.feedinSerialData();
}

#endif