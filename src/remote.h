#ifndef REMOTE_H_
#define REMOTE_H_

#define IR_UP     0xFDA05F
#define IR_DOWN   0xFDB04F
#define IR_LEFT   0xFD10EF
#define IR_RIGHT  0xFD50AF
#define IR_ENTER  0xFD906F
#define IR_VOL_UP 0xFD40BF
#define IR_VOL_DN 0xFD00FF
#define IR_PLAY   0xFD807F
#define IR_SETUP  0xFD20DF
#define IR_MODE   0xFD609F
#define IR_BACK   0xFD708F
#define IR_0      0xFD30CF
#define IR_1      0xFD08F7
#define IR_2      0xFD8877
#define IR_3      0xFD48B7
#define IR_4      0xFD28D7
#define IR_5      0xFDA857
#define IR_6      0xFD6897
#define IR_7      0xFD18E7
#define IR_8      0xFD9867
#define IR_9      0xFD58A7
#define IR_NULL   0xFFFFFFFF

bool settingMode = false;

void remoteControl(uint32_t command) {

      // End of IR transmission, not a command
      if (command == IR_NULL) { return; }

      // Change modes
      if (command == IR_MODE) {
        Serial.println(F("MODE\n"));
        settingMode = !settingMode;

        if (settingMode) { Serial.println(F("settingMode = true\n")); }
        else { Serial.println(F("settingMode = false\n")); }
        return;
      }

      if (settingMode) {
        Serial.println(F("Setting mode to:"));
        if (command == IR_1) { mode = MODE_PATTERN; Serial.println(F("PATTERN\n")); }
        else if (command == IR_2) { mode = MODE_IMAGE; Serial.println(F("IMAGE\n")); }
        else if (command == IR_0) {
          cycleByTen = !cycleByTen;
          if (cycleByTen) { setOverlay(2, CRGB::Pink, 100); }
          else { setOverlay(2, CRGB::Pink, 50); }
        }

        // Done setting mode whether it was successful or not
        settingMode = false;
        return;
      }

      // Toggle auto cycle
      if (command == IR_PLAY) {
        autoCycle = !autoCycle;
        if (autoCycle) {
          setOverlay(2, CRGB::DarkGreen, 10);
        } else {
          setOverlay(2, CRGB::Red, 10);
        }
        return;
      }

      // Pattern Mode Controls
      if (mode == MODE_PATTERN) {

        if (command == IR_LEFT) {
          if(--patternNumber < 0) patternNumber = NUM_PATTERNS - 1;
          return;
        }
        if (command == IR_RIGHT) {
          if(++patternNumber >= NUM_PATTERNS) patternNumber = 0;
          return;
        }

        // Switch Background Color
        if (command == IR_4) {
          if ((colorIndex-1)<0) { colorIndex=NUM_COLORS-1; }
          else { colorIndex--; }
          // Switch chaser
          if (chaserIndex == colorIndex) {
            if ((chaserIndex+1)>=NUM_COLORS) { chaserIndex=0; }
            else { chaserIndex++; }
          }
          return;
        }
        if (command == IR_5) {
          if ((colorIndex+1)>=NUM_COLORS) { colorIndex=0; }
          else { colorIndex++; }
          // Switch chaser
          if (chaserIndex == colorIndex) {
            if ((chaserIndex+1)>=NUM_COLORS) { chaserIndex=0; }
            else { chaserIndex++; }
          }
          return;
        }

        // Switch Chaser Color
        if (command == IR_1) {
          if ((chaserIndex+1)>=NUM_COLORS) { chaserIndex=0; }
          else { chaserIndex++; }
          // if same as background, once more
          if (chaserIndex == colorIndex) {
            if ((chaserIndex+1)>=NUM_COLORS) { chaserIndex=0; }
            else { chaserIndex++; }
          }
          return;
        }

        // Toggle chaser moving mode
        if (command == IR_3) {
          chaserMoving = !chaserMoving;
          return;
        }

        // Number of Chasers
        if (command == IR_UP) {
          if ((chaserCount+1)> 16) { return; }
          else { chaserCount++; }
          return;
        }
        if (command == IR_DOWN) {
          if ((chaserCount-1)< 0) { return; }
          else { chaserCount--; }
          return;
        }

      }
      else if (mode == MODE_IMAGE) {

        if (command == IR_LEFT) {
          if(--imageNumber < 0) imageNumber = NUM_IMAGES - 1;
          currentBitmap.next();
          return;
        }
        if (command == IR_RIGHT) {
          if(++imageNumber >= NUM_IMAGES) imageNumber = 0;
          currentBitmap.next();
          return;
        }

        // Increase image number by 10
        if (command == IR_0) {
          imageNumber = (imageNumber+10)%NUM_IMAGES;
          currentBitmap.next();
          return;
        }

        // Increase image number by 10
        if (command == IR_BACK) {
          imageNumber = (imageNumber-10)%NUM_IMAGES;
          currentBitmap.next();
          return;
        }

        // Cycle time of 2 seconds
        if (command == IR_1) {
          cycleCount = 0;
          cycleTimeIndex = 0;
          setOverlay(2, CRGB::Green, 25);
          return;
        }

        // Cycle time of 5 seconds
        if (command == IR_2) {
          cycleCount = 0;
          cycleTimeIndex = 1;
          setOverlay(2, CRGB::Green, 50);
          return;
        }

        // Cycle time of 10 & 30 seconds
        if (command == IR_3) {
          cycleCount = 0;
          if (cycleTimeIndex == 2) {
            cycleTimeIndex = 3;
            setOverlay(2, CRGB::Green, 100);
          } else {
            cycleTimeIndex = 2;
            setOverlay(2, CRGB::Green, 75);
          }
          return;
        }

        // Toggle Sync Mode
        if (command == IR_7) {
          if (syncMode == SYNC_OFF) {
            syncMode = SYNC_SECONDARY;
            setOverlay(2, CRGB::Yellow, 66);
          } else if (syncMode == SYNC_SECONDARY) {
            syncMode = SYNC_PRIMARY;
            setOverlay(2, CRGB::Yellow, 100);
          } else if (syncMode == SYNC_PRIMARY) {
            syncMode = SYNC_OFF;
            setOverlay(2, CRGB::Yellow, 33);
          }
          return;
        }

      }

      // Toggle blackOut mode
      if (command == IR_9) {
        blackOut = !blackOut;
        return;
      }

      // Debug moving mode
      if (command == IR_6) {
        tick++;
        return;
      }

      // Brightness controls
      if (command == IR_VOL_UP) {
        Serial.println(F("VOL UP\n"));
        if ((brightness+5) < 200) brightness = brightness+7;
        FastLED.setBrightness(brightness);
        setOverlay(1, CRGB::White, (int)((brightness/200.0)*100.0));
      }
      if (command == IR_VOL_DN) {
        Serial.println(F("VOL DOWN\n"));
        if ((brightness-5) > 0) brightness = brightness-7;
        FastLED.setBrightness(brightness);
        setOverlay(1, CRGB::White, (int)((brightness/200.0)*100.0));
      }

}

#endif
