#include <jled.h>
#include <Chrono.h>

const int MODE_OK = 0;
const int MODE_WARN = 1;
const int MODE_FLASH = 2;

const int WARN_TIME = 2 * 24 * 60 * 60;
const int FLASH_TIME = 3 * 24 * 60 * 60;

const int PIN_BUTTON = 2;
const int PIN_LED =  9;

int buttonState;
int mode = MODE_OK;

Chrono chronoSeconds(Chrono::SECONDS);
JLed led = JLed(PIN_LED);

void setup() {
  pinMode(PIN_LED, OUTPUT); 
  pinMode(PIN_BUTTON, INPUT);
  led.On();
}

void loop() {
  buttonState = digitalRead(PIN_BUTTON);
  if (buttonState == LOW) {
    chronoSeconds.restart();
    mode = MODE_OK;
    led.Off();
  }

  if (mode == MODE_OK and chronoSeconds.hasPassed(WARN_TIME)) {
    led.On();
    mode = MODE_WARN;
  }

  if (mode == MODE_WARN and chronoSeconds.hasPassed(FLASH_TIME)) {
    led.Breathe(5000).DelayAfter(500).Forever();
    mode = MODE_FLASH;
  }

    led.Update();
}
