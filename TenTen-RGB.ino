// TenTen-RGB
// Some Arduino code for fooling around with a NeoPixel compatible 10x10 RGB
// matrix display.
// Copyright (C) 2014 Alex Donatelli
// Copyright (C) 2015 Mikkel Kirkgaard Nielsen
//
// Based in part on circlergb.ino by Alex Donatelli
// (http://www.instructables.com/id/A-Multicolor-LED-Lamp/?ALLSTEPS).
//
// Uses Adafuit's NeoPixel library for the raw LED protocol, get it at
// https://github.com/adafruit/Adafruit_NeoPixel
//
// Also includes code from the WiFiWebServer sketch from the ESP8266
// distribution.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include <Adafruit_NeoPixel.h>

#define PIXELPIN     2
#define BUTTONPIN    12
#define PIXELCOUNT  50

#define MAXFUN      11
int fun = MAXFUN - 1; // Start up with scroller
String deftext="CODING PIRATES!";

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800);

// Structure representing a single pixel
// Not used, yet.
struct pixel
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t brightness; // Brightness is not part of ws281x protocol, must be a NeoPixel library scaling factor
};

const uint8_t FONT_WIDTH   = 4;
const uint8_t FONT_HEIGHT  = 5;
const uint8_t CHAR_SPACING = 1;

struct character
{
  char ascii;
  bool pixels[FONT_WIDTH+1][FONT_HEIGHT];
};

const uint8_t font_elems = /*'Z' - 'A'*/ 28 + 3 + 7; // Reserve storage for Danish alphabet + " ,.!?-_"
const struct character font[font_elems] =
{
  { 'A', {
      { 0, 1, 1, 0, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 1, 1, 1, 0 },
      { 1, 0, 0, 1, 0 },
      { 1, 0, 0, 1, 0 }
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
  { 'F', {
      { 1, 1, 1, 1 },
      { 1, 0, 0, 0 },
      { 1, 1, 1, 0 },
      { 1, 0, 0, 0 },
      { 1, 0, 0, 0 }
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
  { 'H', {
      { 1, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 1, 1, 1, 1 },
      { 1, 0, 0, 1 },
      { 1, 0, 0, 1 }
    }
  },
  { 'I', {
      { 1, 1, 1, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 1, 1, 1, 0 }
    }
  },
  { 'J', {
      { 0, 1, 1, 1 },
      { 0, 0, 0, 1 },
      { 0, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 0, 1, 1, 0 }
    }
  },
  { 'K', {
      { 1, 0, 0, 1 },
      { 1, 0, 1, 0 },
      { 1, 1, 0, 0 },
      { 1, 0, 1, 0 },
      { 1, 0, 0, 1 }
    }
  },
  { 'L', {
      { 1, 0, 0, 0 },
      { 1, 0, 0, 0 },
      { 1, 0, 0, 0 },
      { 1, 0, 0, 0 },
      { 1, 1, 1, 1 }
    }
  },
  { 'M', {
      { 1, 0, 0, 1, 1 },
      { 1, 1, 1, 1, 1 },
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 1, 1 }
    }
  },
  { 'N', {
      { 1, 1, 0, 1, 1 },
      { 1, 1, 0, 1, 1 },
      { 1, 0, 1, 1, 1 },
      { 1, 0, 1, 1, 1 },
      { 1, 0, 0, 1, 1 }
    }
  },
  { 'O', {
      { 0, 1, 1, 0, 0 },
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 1, 1 },
      { 0, 1, 1, 0, 0 }
    }
  },
  { 'P', {
      { 1, 1, 1, 0 },
      { 1, 0, 0, 1 },
      { 1, 1, 1, 0 },
      { 1, 0, 0, 0 },
      { 1, 0, 0, 0 }
    }
  },
  { 'Q', {
      { 0, 1, 1, 0 },
      { 1, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 1, 0, 1, 1 },
      { 0, 1, 1, 1 }
    }
  },
  { 'R', {
      { 1, 1, 1, 0 },
      { 1, 0, 0, 1 },
      { 1, 1, 1, 0 },
      { 1, 0, 1, 0 },
      { 1, 0, 0, 1 }
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
  { 'T', {
      { 1, 1, 1, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 }
    }
  },
  { 'U', {
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 1, 1 },
      { 1, 0, 0, 1, 1 },
      { 1, 1, 1, 1, 0 }
    }
  },
  { 'V', {
      { 1, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 0, 1, 1, 0 }
    }
  },
  { 'W', {
      { 1, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 1, 0, 0, 1 },
      { 0, 1, 1, 0 },
      { 0, 1, 1, 0 }
    }
  },
  { 'X', {
      { 1, 0, 0, 1 },
      { 0, 1, 1, 0 },
      { 0, 1, 1, 0 },
      { 0, 1, 1, 0 },
      { 1, 0, 0, 1 }
    }
  },
  { 'Y', {
      { 1, 0, 1, 0 },
      { 1, 0, 1, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 }
    }
  },
  { 'Z', {
      { 1, 1, 1, 1 },
      { 0, 0, 1, 1 },
      { 0, 1, 1, 0 },
      { 1, 1, 0, 0 },
      { 1, 1, 1, 1 }
    }
  },
  { 'Æ', {
      { 0, 1, 1, 1 },
      { 0, 1, 1, 0 },
      { 1, 0, 1, 1 },
      { 1, 1, 1, 0 },
      { 1, 0, 1, 1 }
    }
  },
  { 'Ø', {
      { 0, 1, 1, 1 },
      { 1, 0, 1, 1 },
      { 1, 1, 1, 1 },
      { 1, 1, 0, 1 },
      { 1, 1, 1, 0 }
    }
  },
  { 'Å', {
      { 0, 1, 1, 0 },
      { 0, 1, 1, 0 },
      { 1, 0, 0, 1 },
      { 1, 1, 1, 1 },
      { 1, 0, 0, 1 }
    }
  },
  { ',', {
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 1, 0 }
    }
  },
  { '.', {
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 1, 0 }
    }
  },
  { '!', {
      { 0, 1, 1, 0 },
      { 0, 1, 1, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 0, 0 },
      { 0, 1, 1, 0 }
    }
  },
  { '?', {
      { 0, 1, 1, 0 },
      { 0, 0, 0, 1 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 1, 0 }
    }
  },
  { '-', {
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    }
  },
  { '_', {
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 1, 1, 1, 1 }
    }
  },
  { ' ', {
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    }
  }
};

/* Scroll a string bouncing off the ends.
   Assumes 10x10 pixel display and a 5x5 pixel font.
*/
#define DISPLAY_WIDTH 5
#define DISPLAY_HEIGHT 5
#define DISPLAY_FIRST_PIXEL_ROW 4
#define DISPLAY_FIRST_PIXEL_COL 0
#define DISPLAY_HORIZONTALLY_CONNECTED false
#define DISPLAY_SNAKE_WRAP false
void scrolltext(const char *text, uint8_t x = 0, uint8_t y = 0, uint16_t wait = 2000, uint8_t rounds=-1)
{
  if (!strlen(text))
  {
    Serial.println("No text to scroll!");
    return;
  }
  int16_t tick = 0;
  bool forward = false;
  uint8_t off_col[3] = {0x0a, 0x0a, 0x0a}; // initialized with static startup colors
  uint8_t on_col[3] = {0x50, 0x05, 0x05};

  uint16_t row_length = strlen(text) * (FONT_WIDTH+CHAR_SPACING);
  uint16_t matrix_length = row_length * FONT_HEIGHT;
  bool *matrix = (bool*)malloc(matrix_length);
  Serial.print("scrolltext: ");
  Serial.println(text);

  if (matrix)
  {
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

      for (int row = 0; row < FONT_HEIGHT; row++)
        for (int col = 0; col < FONT_WIDTH; col++)
          matrix[ c * (FONT_WIDTH+CHAR_SPACING) + row * row_length + col] = font[cur_elem].pixels[row][col];
    }

    // write visible pixels from matrix to leds according to current offset (tick)
    // until button press or requested rounds are done
    while (!buttonPressed() && rounds)
    {
      uint8_t wait_scale = 1;
      for (int pix = 0; pix < strip.numPixels(); pix++)
      {
        uint32_t row_nr = (pix / DISPLAY_WIDTH);
        uint32_t pick = row_nr*row_length + pix % DISPLAY_WIDTH + tick;
        if (
          pick < matrix_length
          && pick < (1 + pix / DISPLAY_WIDTH) * strlen(text) * (FONT_WIDTH+CHAR_SPACING)
          && matrix[pick]
        )
          strip.setPixelColor(pix, on_col[0], on_col[1], on_col[2]);
        //        strip.setPixelColor(pix, rand(), rand(), rand());
        else
          strip.setPixelColor(pix, off_col[0], off_col[1], off_col[2]);
      }

      strip.show();

      // Make it tick and do bounds checking
      forward ? tick++ : tick--;
      if ( tick > strlen(text) * (FONT_WIDTH+CHAR_SPACING)
           || tick < 0 )
      {
        forward = !forward;
        if (forward) {
          wait_scale = 10; // more delay at string start for readability
          tick++;
          off_col[0] = 0x00 + rand() & 0x0f;
          off_col[1] = 0x00 + rand() & 0x0f;
          off_col[2] = 0x00 + rand() & 0x0f;
          rounds--;
        } else {
          tick--;
          on_col[0] = 0x00 + rand() & 0xff;
          on_col[1] = 0x00 + rand() & 0xff;
          on_col[2] = 0x00 + rand() & 0xff;
        }
      }
      delay(wait * wait_scale);
    }
    free(matrix);
  }
  else
    Serial.print("Not enough memory for string!");
}

#ifdef ESP8266
#include <ESP8266WiFi.h>
WiFiServer server(80);
#endif

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(BUTTONPIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("TenTen-RGB");

#ifdef ESP8266
  wifiConnect();
#endif
}

#ifdef ESP8266 // we're exceeding an Uno's memory, for now yank out this string to keep within
const String funnames[MAXFUN+1]={"rainbow","rainbowCycle","theaterChaseRainbow","movingColor","bouncingColor","fadingColor","psycho","white","threelinefun","threelinefun 2","scrolltext","black"};
#endif
int funlastwritten=-1;
void loop() {
  strip.setBrightness(50);

  // Make sure button is checked even if current fun doesn't do it
  buttonPressed();
  if (fun != funlastwritten)
  {
    Serial.println(fun);//names[fun]);
    funlastwritten=fun;
  }

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
//      scrolltext("ABCDEFGHIJKLMNOPQRST", 0, 0, 25*5, 1);
//      scrolltext("UVWXYZ ,.!?-_", 0, 0, 25*5, 1);
//      scrolltext("GEEKLABS RULLER", 0, 0, 25*5, 2);
//      scrolltext("MALOU MAIKA", 0, 0, 50);
      deftext.toUpperCase();
      scrolltext(deftext.c_str(), 0, 0, 150);
      break;
    case 11:
      black();
      break;
  }
}

#ifdef ESP8266
bool wifiConnect(){
  // Connect to WiFi network
  #include "wifis.h" // include user supplied passwords (see wifis_template.h)
  if ( sizeof(wifis_ssids) && (sizeof(wifis_ssids) == sizeof(wifis_passwords)) )
  {
    Serial.println();
    Serial.println();

    int wifis_idx=-1, loops=0;
    while (WiFi.status() != WL_CONNECTED) {
      if (loops++%20==0) {
        if (loops>1) {
          Serial.print(F("\n\Couldn't connect to WiFi: "));
          Serial.println(wifis_ssids[wifis_idx%sizeof(wifis_ssids)]);
        }
        wifis_idx++;
        Serial.print(F("Attempting to connect to WiFi: "));
        Serial.println(wifis_ssids[wifis_idx%sizeof(wifis_ssids)]);
        WiFi.begin(wifis_ssids[wifis_idx%sizeof(wifis_ssids)], wifis_passwords[wifis_idx%sizeof(wifis_ssids)]);
        Serial.print(F("Waiting for connection"));
      }
      Serial.write(".");
      delay(500);
    }
    Serial.println("");
    Serial.println(F("WiFi connected"));

    // Start the server
    server.begin();
    Serial.print(F("Server started on  IP: "));
    Serial.println("My IP is "+WiFi.localIP());
    //deftext = "My IP is "+WiFi.localIP();
  }
  else
    Serial.println("No or invalid wifi information defined, see wifis_template.h");
}

bool receiveHTTP() {
  Serial.print("Server status:");
  Serial.println(server.status());
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return false;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  if (req.indexOf("/fun/text/") != -1)
    deftext = req.substring(req.indexOf("/fun/text/")+10, req.lastIndexOf(' '));
  else if (req.indexOf("/fun/") != -1) {
    String val = req.substring(req.indexOf("/fun/")+5, req.lastIndexOf(' '));
    if (val.toInt() >= 0 && val.toInt() <= MAXFUN)
      fun = val.toInt();
  }
/*  else {
    Serial.println("invalid request");
    client.stop();
    return false;
  }
*/

  client.flush();
  String funString(fun);
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nFun is now ";
  s += funnames[fun];
  s += " (";
  s += funString;
  s += ")<br>Text is now ";
  s += deftext;
  s += "<p><a href='/fun/";
  s += (fun==0?MAXFUN:fun-1);
  s += "'><< previous </a>&nbsp;";
  s += "<a href='/fun/";
  s += (fun==MAXFUN?0:fun+1);
  s += "'> next >></a>";
  s +="</html>\n";
  //s +="<p><input type=text id=text onSubmit='location.href='></html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
  return true;
}
#endif

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
      while (!digitalRead(BUTTONPIN))
      ;
    }
  }
#ifdef ESP8266
  else if (receiveHTTP())
    return true; // makes main loop reevaluate current fun
#endif
  return (pressed);
}
