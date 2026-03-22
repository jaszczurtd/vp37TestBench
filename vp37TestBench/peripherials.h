#ifndef C_PERIPHERIALS
#define C_PERIPHERIALS

#include <tools.h>
#include <hal/hal_display.h>

#include <libConfig.h>
#include <canDefinitions.h>
#include <draw7Segment.h>

#include "config.h"
#include "hardwareConfig.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

extern volatile float valueFields[];

void initI2C(void);
void initSPI(void);
void initBasicPIO(void);
float getSystemSupplyVoltage(void);
float getVP37FuelTemperature(void);
void valToPWM(unsigned char pin, int val);
int getVP37Adjustometer(void);
int getThrottlePercentage(void);

#endif
