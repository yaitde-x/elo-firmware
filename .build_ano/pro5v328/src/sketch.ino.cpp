#include <Arduino.h>
#include <CmdMessenger.h>
#include <FastLED.h>
#include "Telemetry.h"
#include "Common.h"
#include "Animator.h"
#include "SinAnimation.h"
void emit_status(void);
void send_dev_status(void);
void on_dev_status(void);
void on_dev_on(void);
void on_dev_off(void);
void on_dev_level(void);
void on_dev_color(void);
void on_dev_anim(void);
void on_dev_telem(void);
void on_dev_config(void);
void on_unknown_command(void);
void attach_callbacks(void);
void setup();
void loop();
#line 1 "src/sketch.ino"
//#include <CmdMessenger.h>
//#include <FastLED.h>
//#include "Telemetry.h"
//#include "Common.h"
//#include "Animator.h"
//#include "SinAnimation.h"

FASTLED_USING_NAMESPACE

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define ELO_ANIM
#define LED_CNT 17

const int BAUD_RATE = 19200;
char retBuf[10];
int v = 1;

CmdMessenger c = CmdMessenger(Serial);
extern void (* animationCallback)();

/* Define available CmdMessenger commands */
enum {
  dev_status,
  dev_on,
  dev_off,
  dev_level,
  dev_color,
  dev_anim,
  dev_telem,
  dev_config
};

/* Create callback functions to deal with incoming messages, hello!*/

void emit_status(void) {
  Serial.print(F(","));
  Serial.print(getState());
  Serial.print(F(","));
  Serial.print(getBrightness());
  Serial.print(F(","));
  CRGB cc = getColor();
  Serial.print(cc.r);
  Serial.print(F(","));
  Serial.print(cc.g);
  Serial.print(F(","));
  Serial.print(cc.b);
  Serial.print(F(","));
  Serial.print(getAnimation());
  Serial.print(F(","));
  Serial.print(getParam1());
  Serial.print(F(","));
  Serial.print(getParam2());
  Serial.print(F(","));
  Serial.print(getParam3());
  Serial.print(F(","));
  Serial.print(getParam4());
  Serial.print(F(","));
  Serial.print(getParam5());
  Serial.print(F(","));
  Serial.println(getVersion());

}

/* callback */
void send_dev_status(void) {
  Serial.print(10);
  emit_status();
}

/* callback */
void on_dev_status(void) {
  char* uuid = c.readStringArg();
  Serial.print(uuid);
  emit_status();
}

/* callback */
void on_dev_on(void) {
  char* uuid = c.readStringArg();
  int brightness = c.readInt16Arg();

  //Serial.print(brightness);
  setBrightness(brightness);
  CRGB cc = getColor();
  setAll(cc.r, cc.g, cc.b);
  digitalWrite(LED_PIN, HIGH);
  setState(1);
  Serial.print(uuid);
  emit_status();
}

/* callback */
void on_dev_off(void) {

  char* uuid = c.readStringArg();

  setAll(0, 0, 0);
  digitalWrite(LED_PIN, LOW);
  setState(0);
  Serial.print(uuid);
  emit_status();
}

/* callback */
void on_dev_level(void) {

  char* uuid = c.readStringArg();
  int brightness = c.readInt16Arg();

  setBrightness(brightness);

  Serial.print(uuid);
  emit_status();
}

/* callback */
void on_dev_color(void) {

  char* uuid = c.readStringArg();

  int r = c.readInt16Arg();
  int g = c.readInt16Arg();
  int b = c.readInt16Arg();

  
   setColor(CRGB(r, g, b));

	if (getState() == 1)
		setAll(r, g, b);
  

  Serial.print(uuid);
  emit_status();
}

/* callback */
void on_dev_anim(void) {
	int oldAnim = getAnimation();
	char* uuid = c.readStringArg();
	int anim = c.readInt16Arg();
	int p1 = c.readInt16Arg();
	int p2 = c.readInt16Arg();
	int p3 = c.readInt16Arg();
	int p4 = c.readInt16Arg(); 
	int p5 = c.readInt16Arg();
  
	setAnimation(anim, p1, p2, p3, p4, p5);

#ifdef ELO_ANIM
	if (anim != oldAnim) {
		
		if (anim == 1) {
			animateSineInit();
			setAnimationCallback(animateSine, animateSineMicro);
		} else if (anim == 2) {
			animateStreakInit();
			setAnimationCallback(animateStreak, animateStreakMicro);
		} else if (anim == 3) {
			animateSlideInit();
			setAnimationCallback(animateSlide, animateSlideMicro);
		} else if (anim == 4) {
			animateRainbowInit();
			setAnimationCallback(animateRainbow, animateRainbowMicro);
		} else if (anim == 5) {
			animateSound1Init();
			setAnimationCallback(animateSound1, animateSound1Micro);
		} else if (anim == 6) {
			animateSound2Init();
			setAnimationCallback(animateSound2, animateSound2Micro);
		}
		telem_reset();
	}
	
	if (p1 > 0)
		setFrameLength(p1);
	else
		setFrameLength(100);
#endif
	
	Serial.print(uuid);
	emit_status();
}

void on_dev_telem(void) {

  char* uuid = c.readStringArg();

  Serial.print(uuid);
  Serial.print(F(","));
  Serial.print(telem_get_frame_count());
  Serial.print(F(","));
  Serial.print(telem_get_frame_millis());
  Serial.print(F(","));
  Serial.print(getAudio());
  Serial.print(F(","));
  Serial.println(getVersion());
}

void on_dev_config(void) {

  char* uuid = c.readStringArg();

  //state: 0, brightness: 255, r: 255, g: 255, b: 255, p1: 0, p2:0, p3:0, p4:0, p5:0, anim: 0
  int state = c.readInt16Arg();
  setState(state);
  
  int brightness = c.readInt16Arg();
  setBrightness(brightness);
  
  int r = c.readInt16Arg();
  int g = c.readInt16Arg();
  int b = c.readInt16Arg();
  setColor(CRGB(r, g, b));
  
  int p1 = c.readInt16Arg();
  int p2 = c.readInt16Arg();
  int p3 = c.readInt16Arg();
  int p4 = c.readInt16Arg(); 
  int p5 = c.readInt16Arg();
  int anim = c.readInt16Arg();
  setAnimation(anim, p1, p2, p3, p4, p5);
  
  Serial.print(uuid);
  Serial.println(getVersion());
}

/* callback */
void on_unknown_command(void) {
  //c.sendCmd(dev_error,"ER1");
}

/* Attach callbacks for CmdMessenger commands */
void attach_callbacks(void) {

  c.attach(dev_status, on_dev_status);
  c.attach(dev_on, on_dev_on);
  c.attach(dev_off, on_dev_off);
  c.attach(dev_level, on_dev_level);
  c.attach(dev_color, on_dev_color);
  c.attach(dev_anim, on_dev_anim);
  c.attach(dev_telem, on_dev_telem);
  c.attach(on_unknown_command);
}

void setup() {
	
	delay(3000); // 3 second delay for recovery
	Serial.begin(BAUD_RATE);
	
	pinMode(LED_PIN, OUTPUT);
	pinMode(A0, INPUT);
	
	attach_callbacks();

	initCommon(LED_CNT);
	
}

void loop() {
  c.feedinSerialData();

  sampleAudio();
  	
#ifdef ELO_ANIM
  if (getAnimation() > 0 && animationCallback != NULL && getState() == 1) {
    animateFrame();
  }
#endif
 
}
