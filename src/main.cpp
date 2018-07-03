/*
 * Hoop v3
 *
 * Written By: Keenan Gizzi
 */

//#define HOOP_DEBUG

// Modes
#define MODE_PATTERN  1
#define MODE_IMAGE    2
int mode = MODE_IMAGE;

// LEDs
#include <FastLED.h>
#define DATA_PIN 3
#define CLOCK_PIN 2
#define NUM_LEDS 123
CRGB leds[NUM_LEDS];
int brightness = 100;

// Solid Colors
const CRGB PROGMEM colors[] = {
  CRGB::White, CRGB::Red, CRGB::Purple, CRGB::Blue, CRGB::Green,
  CRGB::Yellow, CRGB::Orange, CRGB::Pink, CRGB::Black,
  CRGB::Magenta, CRGB::LimeGreen, CRGB::Teal, CRGB::SeaGreen
};
#define NUM_COLORS (sizeof(colors) / sizeof(colors[0]))
int colorIndex = 0;
int chaserIndex = 1;
int chaserCount = 0;

bool chaserMoving = false;
uint32_t lastLineTime  = 0L; // Time last scan line was output
uint8_t tick = 0;

// Images
#include <Bitmap.h>
Bitmap currentBitmap;

int imageNumber = 0;
#include "images.h"

// Microseconds per line for various speed settings
const uint16_t PROGMEM lineTable[] = { // 375 * 2^(n/3)
  1000000L /  3,
  1000000L /  375, // 375 lines/sec = slowest
  1000000L /  472,
  1000000L /  595,
  1000000L /  750, // 750 lines/sec = mid
  1000000L /  945,
  1000000L / 1191,
  1000000L / 1500  // 1500 lines/sec = fastest
};
#define NUM_SPEEDS (sizeof(lineTable) / sizeof(lineTable[0]))
uint8_t  lineIntervalIndex = 3;

// IR Remote
#include <IRLibRecv.h>
#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h>
#include "remote.h"

IRrecv irReceiver(0); // Pin 0
IRdecodeNEC remoteDecoder;

void setup() {
  Serial.begin(9600);

  // Set up IR remote
  irReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));

  // Set up LEDs
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  FastLED.setMaxRefreshRate(3000);
  FastLED.clear();
  FastLED.setBrightness(brightness);
}


void loop() {
  //Continue looping until you get a complete signal received
  if (irReceiver.getResults()) {
    remoteDecoder.decode();

    // Debug remote codes:
    //remoteDecoder.dumpResults(true);  //Now print results. Use false for less detail

    remoteControl(remoteDecoder.value);

    // Debug output
    if (HOOP_DEBUG) {
      Serial.print(F("Image: "));
      Serial.println(imageNumber);
      Serial.print(F("Mode: "));
      Serial.println(mode);
      Serial.print(F("Brightness: "));
      Serial.println(brightness);
      Serial.print("\n");
    }

    irReceiver.enableIRIn();      //Restart receiver
  }

  uint32_t t = millis(); // Current time, milliseconds

  // This is if we want to regulate how fast each scan line gets output
  // Should be calculated from IMU motion
  if (((t = micros()) - lastLineTime) < lineTable[lineIntervalIndex]) return;

  // Display LEDs
  if (mode == MODE_PATTERN) {
    fill_solid(leds, FastLED.size(), colors[colorIndex]);
    for (int c=0; c<chaserCount; c++) {
      int space = NUM_LEDS / chaserCount;
      leds[((c*space)+tick)%NUM_LEDS] = colors[chaserIndex];
      if (chaserMoving) { tick++; }
    }

  }
  else if (mode == MODE_IMAGE) {
    currentBitmap.displayRow(images[imageNumber], tick);
    tick++;
  }

  FastLED.show();
  lastLineTime = t;
}
