#include <jled.h>
#include <Chrono.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Uncomment if the Neopixels are connected for debugging
// #define ENABLE_NEOPIXELS

const int MODE_OK = 0;
const int MODE_WARN = 1;
const int MODE_FLASH = 2;

const int WARN_HOURS = 2 * 24;
const int FLASH_HOURS = 3 * 24;
const int HOUR_SECONDS = 60 * 60;

const int PIN_BUTTON = 2;
const int PIN_LED = 3;

#ifdef ENABLE_NEOPIXELS
const int PIN_NEOPIXEL = 6;
const int NUM_PIXELS = 8;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
#endif

int buttonState;
int mode = MODE_WARN;
int hoursPassed = 0;

Chrono chronoSeconds(Chrono::SECONDS);
JLed led = JLed(PIN_LED);

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  led.Blink(100, 100).Forever();

#ifdef ENABLE_NEOPIXELS
  pixels.begin();
  hoursPassed = 255;
  setPixels();
  hoursPassed = 0;
#endif
}

void setPixels()
{
#ifdef ENABLE_NEOPIXELS
  for (int i = 0; i < NUM_PIXELS; i += 1)
  {
    bool isPixelOn = bitRead(hoursPassed, i) == 1;
    pixels.setPixelColor(i, isPixelOn ? pixels.Color(5, 5, 5) : pixels.Color(0, 0, 0));
  }
  pixels.show();
#endif ENABLE_NEOPIXELS
}

void resetTimer()
{
  chronoSeconds.restart();
  hoursPassed = 0;
  mode = MODE_OK;
  led.Off();
  setPixels();
}

void markHour()
{
  hoursPassed += 1;
  chronoSeconds.restart();
  setPixels();
}

void loop()
{
  buttonState = digitalRead(PIN_BUTTON);
  if (buttonState == HIGH)
  {
    resetTimer();
  }

  if (chronoSeconds.hasPassed(HOUR_SECONDS))
  {
    markHour();
  }

  if (mode == MODE_OK and hoursPassed >= WARN_HOURS)
  {
    led.On();
    mode = MODE_WARN;
  }

  if (mode == MODE_WARN and hoursPassed >= FLASH_HOURS)
  {
    led.Breathe(5000).DelayAfter(500).Forever();
    mode = MODE_FLASH;
  }

  led.Update();
}
