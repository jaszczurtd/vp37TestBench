
#ifndef DRAW_7_SEGMENT_CPP
#define DRAW_7_SEGMENT_CPP

#include <Arduino.h>
#include <tools.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S.h>

#include "peripherials.h"
#include "config.h"

class MyDisplay : public Adafruit_ST7796S {
public:
  MyDisplay(int8_t cs, int8_t dc, int8_t rst)
    : Adafruit_ST7796S(cs, dc, rst) {}  

  void draw7SegString(const String& str, int x, int y, int digitWidth, int digitHeight, float thickness, uint16_t color);
  int get7SegStringWidth(const String& str, int digitWidth, float thickness);

private:
  void drawHorizontalSegment(int x, int y, int length, float thickness, uint16_t color);
  void drawVerticalSegment(int x, int y, int length, float thickness, uint16_t color);
  void drawDigit(char ch, int x, int y, int width, int height, float thickness, uint16_t color);
  float getCharProportionalFactor(char c);
};


#endif
