#ifndef C_PERIPHERIALS
#define C_PERIPHERIALS

#include <Arduino.h>
#include <tools.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7796S.h>
#include <Fonts/FreeSansBold18pt7b.h> // A custom font

#include <libConfig.h>
#include <canDefinitions.h>
#include <ADS1X15.h>

#include "config.h"
#include "hardwareConfig.h"
#include "draw7Segment.h"

#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

extern volatile float valueFields[];

typedef struct {
  uint32_t pwmInitted;
  uint16_t analogWritePseudoScale;
  uint16_t analogWriteSlowScale;
  uint32_t pin;
  uint32_t analogFreq; 
  uint32_t analogScale;
} pwmConfig;

class MyDisplay;

MyDisplay* get_display(void);
void initI2C(void);
void initSPI(void);
void initBasicPIO(void);
float calculateVoltage(int adcValue);
float getSystemSupplyVoltage(void);
float getVP37FuelTemperature(void);
void pwm_init(void);
void valToPWM(unsigned char pin, int val);
int getVP37Adjustometer(void);
int getThrottlePercentage(void);

#endif
