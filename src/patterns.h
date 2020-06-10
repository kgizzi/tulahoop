
void normal() {
  fill_solid(leds, FastLED.size(), colors[colorIndex]);
  for (int c=0; c<chaserCount; c++) {
    int space = NUM_LEDS / chaserCount;
    leds[((c*space)+(tick/5))%NUM_LEDS] = chaserColors[chaserIndex];
  }
}

void normalMuliColor() {
  fill_solid(leds, FastLED.size(), colors[colorIndex]);
  for (int c=0; c<chaserCount; c++) {
    int space = NUM_LEDS / chaserCount;
    leds[((c*space)+(tick/5))%NUM_LEDS] = chaserColors[c%NUM_CHASER_COLORS];
  }

}

void reverse() {
  int reverseCount = 3;
  for (int c=0; c<reverseCount; c++) {
    int space = NUM_LEDS / reverseCount;
    leds[NUM_LEDS-(((c*space)+(tick/5))%NUM_LEDS)] = chaserColors[(chaserIndex-1)%NUM_CHASER_COLORS];
  }
}

void normalReverse() {
  normal();
  reverse();
}

void normalMuliColorReverse() {
  normalMuliColor();
  reverse();
}

float pos=0.0;
void sinwaveSingleColor() {
  fill_solid(leds, FastLED.size(), colors[colorIndex]);

  pos = pos+0.01;
  float c1 = sin(pos);

  int space = NUM_LEDS / chaserCount;

  for (int c=0; c<chaserCount; c++) {
    leds[(map((int)(c1 * 100.0), -100, 100, 0, NUM_LEDS)+(c*space))%NUM_LEDS] = chaserColors[chaserIndex];
    leds[(map((int)(c1 * 100.0), -100, 100, NUM_LEDS, 0)+(c*space))%NUM_LEDS] = chaserColors[chaserIndex];
  }
}

void sinwaveMultiColor() {
  fill_solid(leds, FastLED.size(), colors[colorIndex]);

  pos = pos+0.01;
  float c1 = sin(pos);

  int space = NUM_LEDS / chaserCount;

  for (int c=0; c<chaserCount; c++) {
    leds[(map((int)(c1 * 100.0), -100, 100, 0, NUM_LEDS)+(c*space))%NUM_LEDS] = chaserColors[chaserIndex+c%NUM_CHASER_COLORS];
    leds[(map((int)(c1 * 100.0), -100, 100, NUM_LEDS, 0)+(c*space))%NUM_LEDS] = chaserColors[(chaserIndex+c+1)%NUM_CHASER_COLORS];
  }
}

void sinwaveSingleColorTouch() {
  fill_solid(leds, FastLED.size(), colors[colorIndex]);

  pos = pos+0.05;
  float c1 = sin(pos);

  int space = NUM_LEDS / chaserCount;

  for (int c=0; c<chaserCount; c++) {
    leds[(map((int)(c1 * 100.0), -100, 100, 0, NUM_LEDS/chaserCount)+(c*space))] = chaserColors[chaserIndex];
    leds[(map((int)(c1 * 100.0), -100, 100, NUM_LEDS/chaserCount, 0)+(c*space))] = chaserColors[chaserIndex];
  }
}

void twinkleRand() {
  fill_solid(leds, FastLED.size(), colors[colorIndex]);

  for (int c=0; c<chaserCount*3; c++) {
    leds[random(NUM_LEDS)] = chaserColors[random(NUM_CHASER_COLORS)];
  }
}

void (*patterns[])() = {
  sinwaveSingleColorTouch,
  sinwaveSingleColor,
  sinwaveMultiColor,
  normal,
  normalMuliColor,
  normalReverse,
  normalMuliColorReverse,
  twinkleRand
};


#define NUM_PATTERNS (sizeof(patterns) / sizeof(patterns[0]))
