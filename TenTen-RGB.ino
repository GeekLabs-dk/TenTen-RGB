
// TenTen-RGB
// Some Arduino code for fooling around with a NeoPixel compatible 10x10 RGB
// matrix display.
// Copyright (C) 2014 Alex Donatelli
// Copyright (C) 2015 Mikkel Kirkgaard Nielsen
//
// Based in part on circlergb.ino by Alex Donatelli
// (http://www.instructables.com/id/A-Multicolor-LED-Lamp/?ALLSTEPS).
//
// This program is free software: you can redistribute it and/or modify
// it  under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include <Adafruit_NeoPixel.h>

#define PIXELPIN     2
#define BUTTONPIN    0
#define PIXELCOUNT  100
#define MAXFUN      11
int fun = MAXFUN-1; 

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800);

// Structure representing a single pixel
// This is for future use.
struct pixel
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t brightness; // Brightness is not part of ws281x protocol, must be a NeoPixel library scaling factor
};

struct character
{
  char ascii;
  bool pixels[5][5];
};

const uint8_t font_elems = 'Z' - 'A' + 3 + 7; // Allocate memory for Danish alphabet + " ,.!?-+"
const struct character font[font_elems] =
{
  { 'A', {
      { 0, 0, 1, 0, 0 },
      { 0, 1, 0, 1, 0 },
      { 0, 1, 1, 1, 0 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 }
    }
  },
  { 'B', {
      { 1, 1, 1, 0, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 1, 1, 0, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 1, 1, 0, 0 }
    }
  },
  { 'C', {
      { 0, 1, 1, 0, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 0, 0, 1, 0 },
      { 0, 1, 1, 0, 0 }
    }
  },
  { 'D', {
      { 1, 1, 1, 0, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 1, 1, 0, 0 }
    }
  },
  { 'E', {
      { 1, 1, 1, 1, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 1, 1, 0, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 1, 1, 1, 0 }
    }
  },
  { 'G', {
      { 0, 1, 1, 0, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 0, 1, 1, 0 },
      { 0, 1, 1, 0, 0 }
    }
  },
  { 'K', {
      { 1, 0, 0, 1, 0 },
      { 1, 0, 1, 0, 0 },
      { 1, 1, 0, 0, 0 },
      { 1, 0, 1, 0, 0 },
      { 1, 0, 0, 1, 0 }
    }
  },
  { 'L', {
      { 1, 0, 0, 0, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 0, 0, 0, 0 },
      { 1, 1, 1, 1, 0 }
    }
  },
  { 'M', {
      { 1, 0, 0, 0, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 0, 1, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 }
    }
  },
  { 'O', {
      { 0, 1, 1, 1, 0 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 0, 1, 1, 1, 0 }
    }
  },
  { 'M', {
      { 1, 0, 0, 0, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 0, 1, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 }
    }
  },
  { 'S', {
      { 0, 1, 1, 1, 0 },
      { 1, 0, 0, 0, 0 },
      { 0, 1, 1, 0, 0 },
      { 0, 0, 0, 1, 0 },
      { 1, 1, 1, 0, 0 }
    }
  },
  { 'U', {
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1 },
      { 0, 1, 1, 1, 0 }
    }
  },
  { 'I', {
      { 0, 1, 1, 1, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 0, 1, 0, 0 },
      { 0, 1, 1, 1, 0 }
    }
  },
  { ' ', {
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    }
  }
};

void scrolltext(char *text, uint8_t x = 0, uint8_t y = 0, uint16_t wait = 2000)
{
  uint16_t tick = 0;
  uint8_t off_col[3] = {0x0a, 0x0a, 0x0a}; // initialized with static startup colors
  uint8_t on_col[3] = {0x50, 0x05, 0x05};

  uint16_t matrix_length = strlen(text) * (5 + 1) * 5; // 5x5 pixs + 1 pix for spacing pr. char
  bool *matrix = (bool*)malloc(matrix_length);
  memset(matrix, 0, matrix_length);

  // Assemble matrix representing string in pixels according to font definition
  for (int c = 0; c < strlen(text); c++)
  {
    uint8_t cur_elem = 0;
    while (font[cur_elem].ascii != text[c])
      if (++cur_elem >= sizeof(font) / sizeof(struct character))
      {
        cur_elem--; // use last char in font for unknown chars
        break;
      }

    for (int row = 0; row < 5; row++)
      for (int col = 0; col < 5; col++)
        matrix[ c * 6 + row * strlen(text) * 6  + col] = font[cur_elem].pixels[row][col];
  }

  //  write pixels from matrix to leds according to current offset (tick)
  while (!buttonPressed())
  {
    static bool forward = true;
    for (int pix = 0; pix < strip.numPixels(); pix++)
    {
      //              (  row   ) * row pixel len
      uint32_t pick = (pix / 10) * strlen(text) * 6 + pix % 10 + tick;
      if (
        pick < matrix_length
        && pick < (1+pix / 10) * strlen(text) * 6
        && matrix[pick]
      )
        strip.setPixelColor(pix, on_col[0], on_col[1], on_col[2]);
      //        strip.setPixelColor(pix, rand(), rand(), rand());
      else
        strip.setPixelColor(pix, off_col[0], off_col[1], off_col[2]);
    }

    strip.show();

    // did we reach either end?
    if (tick >= strlen(text) * 6 || tick == 0)
    {
      forward = !forward;
      if (forward) {
        off_col[0] = 0x00 + rand() & 0x0f;
        off_col[1] = 0x00 + rand() & 0x0f;
        off_col[2] = 0x00 + rand() & 0x0f;
      } else {
        on_col[0] = 0x00 + rand() & 0xff;
        on_col[1] = 0x00 + rand() & 0xff;
        on_col[2] = 0x00 + rand() & 0xff;
      }
    }
    if (forward)
      tick++;
    else
      tick--;
    delay(wait);
  }
}

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(BUTTONPIN, INPUT);
}

void loop() {
  strip.setBrightness(50);

  switch (fun) {
    case 0:
      rainbow(20);
      break;
    case 1:
      rainbowCycle(20);
      break;
    case 2:
      theaterChaseRainbow(50);
      break;
    case 3:
      movingColor(10);
      break;
    case 4:
      bouncingColor(15u);
      break;
    case 5:
      fadingColor(5);
      break;
    case 6:
      psycho(20);
      break;
    case 7:
      white();
      break;
    case 8:
      threelinefun(true);
      break;
    case 9:
      threelinefun(false);
      break;
    case 10:
      scrolltext("GEEKLABS",0,0,25);// OG IB OG MMM OG ", 0, 0, 40);
      //      scrolltext("MALOU MAIKA", 0, 0, 50);
    case 11:
      black();
      break;
  }
}


void threelinefun(boolean go_random)
{
  uint32_t tick = 0;
  uint8_t rowPixels = 10;
  uint8_t count = strip.numPixels();
  uint8_t spacing = 2;
  uint8_t increment = +1;
  uint8_t delaytime = 100;
  uint8_t colorarray[count / rowPixels][3];

  while (true)
  {

    if (go_random)
    {
      for (int pix = 0; pix < count; pix++)
      {
        if (pix >= rowPixels * 0 && pix < rowPixels * 1 && !((pix + tick + 0 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 1 && pix < rowPixels * 2 && !((pix + tick + 1 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 2 && pix < rowPixels * 3 && !((pix + tick + 2 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 3 && pix < rowPixels * 4 && !((pix + tick + 3 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 4 && pix < rowPixels * 5 && !((pix + tick + 4 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 5 && pix < rowPixels * 6 && !((pix + tick + 5 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 6 && pix < rowPixels * 7 && !((pix + tick + 6 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 7 && pix < rowPixels * 8 && !((pix + tick + 7 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 8 && pix < rowPixels * 9 && !((pix + tick + 8 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else if (pix >= rowPixels * 9 && pix < rowPixels * 10 && !((pix + tick + 9 * increment) % spacing))
          strip.setPixelColor(pix, rand() % 0xff, rand() % 0xff, rand() % 0xff);
        else
          strip.setPixelColor(pix, 0, 0, 0);
      }
    }
    else
    {
      for (int pix = 0; pix < count; pix++)
      {
        if (pix >= rowPixels * 0 && pix < rowPixels * 1 && !((pix + tick + 0 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[0][0], colorarray[0][1], colorarray[0][2]);
        else if (pix >= rowPixels * 1 && pix < rowPixels * 2 && !((pix + tick + 1 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[1][0], colorarray[1][1], colorarray[1][2]);
        else if (pix >= rowPixels * 2 && pix < rowPixels * 3 && !((pix + tick + 2 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[2][0], colorarray[2][1], colorarray[2][2]);
        else if (pix >= rowPixels * 3 && pix < rowPixels * 4 && !((pix + tick + 3 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[3][0], colorarray[3][1], colorarray[3][2]);
        else if (pix >= rowPixels * 4 && pix < rowPixels * 5 && !((pix + tick + 4 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[4][0], colorarray[4][1], colorarray[4][2]);
        else if (pix >= rowPixels * 5 && pix < rowPixels * 6 && !((pix + tick + 5 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[5][0], colorarray[5][1], colorarray[5][2]);
        else if (pix >= rowPixels * 6 && pix < rowPixels * 7 && !((pix + tick + 5 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[6][0], colorarray[6][1], colorarray[6][2]);
        else if (pix >= rowPixels * 7 && pix < rowPixels * 8 && !((pix + tick + 5 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[7][0], colorarray[7][1], colorarray[7][2]);
        else if (pix >= rowPixels * 9 && pix < rowPixels * 9 && !((pix + tick + 5 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[8][0], colorarray[8][1], colorarray[8][2]);
        else if (pix >= rowPixels * 9 && pix < rowPixels * 10 && !((pix + tick + 5 * increment) % spacing))
          strip.setPixelColor(pix, colorarray[9][0], colorarray[9][1], colorarray[9][2]);
        else
          strip.setPixelColor(pix, 0, 0, 0);
      }
    }


    strip.show();
    tick++;

    if (!go_random && (tick == 1 || tick % 35 == 0))
    {
      for (int row = 0; row < count / rowPixels; row++)
        for (int rgb = 0; rgb < 3; rgb++)
          colorarray[row][rgb] = (rand() + 27) % 0xff;
    }

    if (tick % 40 == 0)
      spacing = 2 + rand() % 5;

    if (tick % 25 == 0)
    {
      if (rand() % 2)
        increment = +1;
      else
        increment = -1;

      if (rand() % 3)
        increment *= 2;
    }
    if (tick % 90 == 0)
      delaytime = 50 + rand() % 100;
    delay(delaytime);

    if (tick % 30 == 0)
      strip.setBrightness(5 + rand() % 75);

    if (buttonPressed()) return;
  }

  /*strip.setPixelColor();
  strip.Color();
  strip.setBrightness(i);
  */
}


uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
    if (buttonPressed()) return;
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    if (buttonPressed()) return;
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
    if (buttonPressed()) return;
  }
}

void movingColor(uint8_t wait) {
  uint16_t i, j;

  colorWipe(strip.Color(0, 0, 0), 0);
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      //      strip.setPixelColor(i, Wheel((i+j) & 255));
      strip.setPixelColor(i, Wheel((j * 4) % 256));
      strip.show();
      delay(wait);
      if (buttonPressed()) return;
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
}

void psycho(uint8_t wait) {
  colorWipe(Wheel(random(255)), 0);
  strip.show();
  delay(wait);
  buttonPressed();
}

void fadingColor(uint8_t wait) {
  uint16_t i;
  uint32_t c;

  c = Wheel(random(255));
  for (i = 0; i < 255; i++) {
    strip.setBrightness(i);
    colorWipe(c, 0);
    strip.show();
    delay(wait);
    if (buttonPressed()) {
      strip.setBrightness(255);
      return;
    }
  }
  for (i = 255; i > 1; i--) {
    strip.setBrightness(i);
    colorWipe(c, 0);
    strip.show();
    delay(wait);
    if (buttonPressed()) {
      strip.setBrightness(255);
      return;
    }
  }
}

void bouncingColor(uint8_t wait) {
  uint16_t i, j;
  uint32_t c;

  colorWipe(strip.Color(0, 0, 0), 0);
  c = strip.Color(0, 0, 0);
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.setPixelColor(strip.numPixels() - i, c);
      strip.show();
      delay(wait);
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.setPixelColor(strip.numPixels() - i, strip.Color(0, 0, 0));
      if (i % (strip.numPixels() / 2) == 0)
        c = Wheel( random(255) % 255);
      if (buttonPressed()) return;
    }
  }
}

void white(void) {
  colorWipe(strip.Color(255, 255, 255), 0);
  strip.show();
  buttonPressed();
}

void black(void) {
  colorWipe(strip.Color(0, 0, 0), 0);
  strip.show();
  while (!buttonPressed());
}

boolean buttonPressed() {
  boolean pressed = !digitalRead(BUTTONPIN);
  if (pressed) {
    delay(10);
    pressed = !digitalRead(BUTTONPIN);
    if (pressed) {
      fun++;
      if (fun > MAXFUN) fun = 0;
      while (!digitalRead(BUTTONPIN));
    }
  }
  return (pressed);
}
