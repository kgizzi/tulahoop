/*
 * tulahoop
 *
 * Written By: Keenan Gizzi
 */

//#define HOOP_DEBUG
#include <FastLED.h>

/*

## Demo Hoop Pins
DATA_PIN  4
CLOCK_PIN 3
IR_PIN    2

## Production Hoop Pins
DATA_PIN  3
CLOCK_PIN 2
IR_PIN    0

*/

// Config
#define DATA_PIN  4
#define CLOCK_PIN 3
#define IR_PIN    2

#define NUM_LEDS  120

// Breadboard
//#define NUM_LEDS  40


// Normal:
//#define NUM_LEDS  150


// Modes
#define MODE_PATTERN  1
#define MODE_IMAGE    2

int mode = MODE_IMAGE;

#define SYNC_OFF       0
#define SYNC_SECONDARY 1
#define SYNC_PRIMARY   2
int syncMode = SYNC_OFF;

// LEDs
CRGB leds[NUM_LEDS];
CRGB onboard[1];
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
bool autoCycle = true; // default to auto cycle on
#include "timer.h"
int cycleCount = 0;
int cycleTimeIndex = 1;
const int PROGMEM cycleTime[] = { 2, 5, 10, 30 };
#define NUM_TIMES (sizeof(cycleTime) / sizeof(cycleTime[0]))

// Overlay
int overlayTime = 0;
CRGB overlayColor = CRGB::White;
int overlayPercent = 0;

bool blackOut = false;

void setOverlay(int time, CRGB color, int percent) {
  overlayTime = time;
  overlayColor = color;
  overlayPercent = percent;
};

// Images
#include <Bitmap.h>
Bitmap currentBitmap;

int imageNumber = 0;
#include "images.h"

// IR Remote
#include <IRLibSendBase.h>
#include <IRLibRecv.h>
#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLib_P04_RC6.h>
#include <IRLibCombo.h>
#include "remote.h"

IRrecv irReceiver(IR_PIN); // Pin 0
IRdecode remoteDecoder;
IRsend irSender;

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

  // Turn off onboard LED
  onboard[0] = CRGB::Black;
  FastLED.addLeds<APA102, 7, 8, BGR>(onboard, 1);

  // Set up timer
  startTimer(1); // frequency of 1 sec
}


void loop() {

  // If master, send sync IR commands
  if (currentBitmap.sendSync && syncMode == SYNC_PRIMARY) {
    currentBitmap.sendSync = false;
    Serial.println("Sending Sync");

    uint16_t syncCommand = (autoCycle << 12) | (cycleTimeIndex << 8) | imageNumber;

    Serial.print(F("autoCycle: "));
    Serial.println(autoCycle);
    Serial.print(F("cycleTimeIndex: "));
    Serial.println(cycleTimeIndex);
    Serial.print(F("imageNumber: "));
    Serial.println(imageNumber);
    Serial.print(F("syncCommand: "));
    Serial.println(syncCommand);
    Serial.print("\n\n");

    irSender.send(RC6, syncCommand, 0);

    irReceiver.enableIRIn();      //Restart receiver
  }

  //Continue looping until you get a complete signal received
  if (irReceiver.getResults()) {
    remoteDecoder.decode();

    // Debug remote codes:
    //remoteDecoder.dumpResults(true);  //Now print results. Use false for less detail

    if (remoteDecoder.protocolNum == NEC) {
      remoteControl(remoteDecoder.value);
    }

    Serial.println("IR received");
    if (syncMode == SYNC_SECONDARY && remoteDecoder.protocolNum == RC6) {
      // Process sync commands
      //remoteDecoder.dumpResults(true);
      Serial.println("Sync SYNC_SECONDARY");
      remoteDecoder.dumpResults(true);

      int decodedAutoCycle = remoteDecoder.value >> 12;
      int decodedCycleTimeIndex = 0x000F & (remoteDecoder.value >> 8);
      int decodedImageNumber = 0x00FF & remoteDecoder.value;

      Serial.print(F("Received: "));
      Serial.println(remoteDecoder.value);
      Serial.print(F("decodedAutoCycle: "));
      Serial.println(decodedAutoCycle);
      Serial.print(F("decodedCycleTimeIndex: "));
      Serial.println(decodedCycleTimeIndex);
      Serial.print(F("decodedImageNumber: "));
      Serial.println(decodedImageNumber);
      Serial.print("\n\n");

      // Sanity check the signal
      if (decodedAutoCycle >= 0 && decodedAutoCycle <= 1 && decodedAutoCycle >= 0 &&
        decodedAutoCycle <= 3 && decodedImageNumber >= 0 && decodedImageNumber < NUM_IMAGES) {

        Serial.println(F("Valid signal received!"));

        if (decodedAutoCycle == 0) {
          autoCycle = false;
        } else {
          autoCycle = 1;
        }

        cycleTimeIndex = decodedCycleTimeIndex;

        //if (imageNumber != decodedImageNumber) {
          imageNumber = decodedImageNumber;
          cycleCount = 0;
          currentBitmap.next();
          startTimer(1);
          Serial.println(F("Image updated!"));
        //}

      }
    }

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

  if (blackOut) {
    fill_solid(leds, FastLED.size(), CRGB::Black);
  }

  // Overlay
  if (overlayTime > 0) {
    // background
    fill_solid(leds, FastLED.size(), CRGB::Black);
    //fill_solid(leds, (int)((overlayPercent/100)*FastLED.size()), overlayColor);
    //Serial.println((overlayPercent/100.0)*NUM_LEDS);
    for (int i=0; i <= ((overlayPercent/100.0)*NUM_LEDS)-1; i++) {
      leds[i] = overlayColor;
    }
  }

  FastLED.show();
}

// Clock interrupt handler
void TC3_Handler() {
  TcCount16* TC = (TcCount16*) TC3;
  if (TC->INTFLAG.bit.MC0 == 1) {
    TC->INTFLAG.bit.MC0 = 1;

    // Write callback here!!!
    //Serial.print("cycleCount: ");
    //Serial.println(cycleCount);

    if (autoCycle) {
      if (cycleCount >= cycleTime[cycleTimeIndex]-1) {
        cycleCount = 0;
        if(++imageNumber >= NUM_IMAGES) imageNumber = 0;
        currentBitmap.next();

        //Serial.println("auto incrementing image");
      } else {
        cycleCount = cycleCount + 1;
      }
    }

    if (overlayTime > 0) { overlayTime--; };
  }
}
