/*
 * tulahoop
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
#define DATA_PIN 4
#define CLOCK_PIN 3
#define NUM_LEDS 123
CRGB leds[NUM_LEDS];
int brightness = 72;

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
uint8_t tick = 0;

// Auto Cycle / Timers
bool autoCycle = false;
#include "timer.h"
int cycleCount = 0;
int cycleTimeIndex = 1;
const int PROGMEM cycleTime[] = { 2, 5, 10, 30 };
#define NUM_TIMES (sizeof(cycleTime) / sizeof(cycleTime[0]))

// Images
#include <Bitmap.h>
Bitmap currentBitmap;

int imageNumber = 0;
#include "images.h"

// IR Remote
#include <IRLibRecv.h>
#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h>
#include "remote.h"

IRrecv irReceiver(2); // Pin 0
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

  // Set up timer
  startTimer(1); // frequency of 1 sec
}


void loop() {
  //Continue looping until you get a complete signal received
  if (irReceiver.getResults()) {
    remoteDecoder.decode();

    // Debug remote codes:
    //remoteDecoder.dumpResults(true);  //Now print results. Use false for less detail

    remoteControl(remoteDecoder.value);

    // Debug output
#ifdef HOOP_DEBUG
      Serial.print(F("Image: "));
      Serial.println(imageNumber);
      Serial.print(F("Mode: "));
      Serial.println(mode);
      Serial.print(F("Brightness: "));
      Serial.println(brightness);
      Serial.print("\n");
#endif

    irReceiver.enableIRIn();      //Restart receiver
  }

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
}

// Clock interrupt handler
void TC3_Handler() {
  TcCount16* TC = (TcCount16*) TC3;
  if (TC->INTFLAG.bit.MC0 == 1) {
    TC->INTFLAG.bit.MC0 = 1;

    // Write callback here!!!
    Serial.print("cycleCount: ");
    Serial.println(cycleCount);

    if (cycleCount >= cycleTime[cycleTimeIndex]-1) {
      cycleCount = 0;
      if(++imageNumber >= NUM_IMAGES) imageNumber = 0;
      currentBitmap.next();

      Serial.println("auto incrementing image");
    } else {
      cycleCount = cycleCount + 1;
    }

  }
}
