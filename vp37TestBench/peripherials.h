#ifndef C_PERIPHERIALS
#define C_PERIPHERIALS

#include <Arduino.h>
#include <tools.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S.h>
#include <Fonts/FreeSansBold18pt7b.h> // A custom font

#include "config.h"
#include "hardwareConfig.h"
#include "draw7Segment.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

class MyDisplay;

MyDisplay* get_display(void);
void setupOnboardLed(void);
void initSPI(void);
void initBasicPIO(void);

#endif
