#include "Arduino.h"

#include "peripherials.h"

MyDisplay display(TFT_CS, TFT_DC, TFT_RST);

MyDisplay* get_display(void) {
  return &display;
}

void setupOnboardLed(void) {
}

void initSPI(void) {
  SPI.setRX(PIN_MISO); //MISO
  SPI.setTX(PIN_MOSI); //MOSI
  SPI.setSCK(PIN_SCK); //SCK
  SPI.begin(true);

  display.init(SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0, ST7796S_BGR);
  display.invertDisplay(true);
  display.fillScreen(0); // Clear screen

  display.setRotation(3);

  display.setFont();      // Use default (built-in) font
  display.setTextSize(2); // and 2X size for chart label
  
  display.setCursor((display.width() - 144) / 2, 0);
  display.setTextColor(0x0210);
  display.print(F("Widget Sales")); // F("string") is in program memory, not RAM

  float thickness = 2.0;
  int charWidth = 16;
  String str;
  int w, x;
  
  str = "12:34.56+2-1 102^ 99%"; 
  w = display.get7SegStringWidth(str, charWidth, thickness);
  x = (SCREEN_WIDTH - w) / 2;
  display.draw7SegString(str, x, 50, charWidth, 24, thickness, ST77XX_WHITE);

  str = "1ABCDEF2"; 
  w = display.get7SegStringWidth(str, charWidth, thickness);
  x = (SCREEN_WIDTH - w) / 2;
  display.draw7SegString(str, x, 80, charWidth, 24, thickness, ST77XX_WHITE);

}

void initBasicPIO(void) {
  analogWriteResolution(PWM_WRITE_RESOLUTION);

}

