#ifndef C_HARDWAREC
#define C_HARDWAREC

#define VP37_PWM_FREQUENCY_HZ 150
#define ANGLE_PWM_FREQUENCY_HZ 200

#define I2C_SPEED_HZ 500000

#define PIN_SDA 0
#define PIN_SCL 1

#define PIN_MISO 16
#define PIN_MOSI 19
#define PIN_SCK 18

//LCD / display
#define TFT_CS     17 //CS
#define TFT_RST    20 //reset
#define TFT_DC     15 //A0

#define PWM_WRITE_RESOLUTION 11
#define PWM_RESOLUTION 2047

//ADS1115
//values for ECU voltage 
#define R1 3300.0  // 3.3k / ohm
#define R2 470.0   // 470R / ohm
#define ADC_RANGE 0.1875  

#define ADS1115_ADDR 0x48

#define ADS1115_PIN_0 0 //vp37 fuel temp
#define ADS1115_PIN_1 1 //system supply voltage
#define ADS1115_PIN_2 2 //vp37 adjustometer
#define ADS1115_PIN_3 3 //not used atm

#define R_VP37_FUEL_A 2300 //sensor
#define R_VP37_FUEL_B 3300

#define PIO_VP37_RPM 9
#define PIO_VP37_ANGLE 5

#endif
