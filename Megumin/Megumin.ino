#include <Wire.h>
#include <Adafruit_ST7735.h>
// #include <lcdgfx.h>
#include "meg.h"
// #include "song.h"

#define TFT_CS   10
#define TFT_RST  8
#define TFT_DC   9

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

// DisplayST7735_128x160x16_SPI display(8, {-1, 10, 9, 0, -1, -1});

void drawBitmapTransparent(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h, uint16_t transparentColor) {
  for (int16_t j = 0; j < h; j++) {
    for (int16_t i = 0; i < w; i++) {
      uint16_t color = bitmap[j * w + i];
      if (color != transparentColor) {           // skip white
        tft.drawPixel(x + i, y + j, color);
      }
    }
  }
}

void setup() {
  // display.begin();
  // display.getInterface().setOffset(2, 1);
  // display.getInterface().setRotation(2);
  // display.fill(0xFFFF);

  // display.drawBitmap16(0,0,27,23,(uint8_t*)megumintest);
  tft.initR();
  tft.setRotation(0);
  tft.fillScreen(0x18A8);
  pinMode(5, OUTPUT);

  drawBitmapTransparent(0, 0, megumintest, 27, 23, ST77XX_WHITE);
  // Logo(69,70,3);
  // playSong(5);
}

void loop() {}