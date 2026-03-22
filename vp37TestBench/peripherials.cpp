#include "Arduino.h"

#include "peripherials.h"

NOINIT volatile float valueFields[F_LAST];

static hal_pwm_freq_channel_t pwmVp37 = NULL;
static hal_pwm_freq_channel_t pwmAngle = NULL;

void setupOnboardLed(void) {
  hal_gpio_set_mode(LED_BUILTIN, HAL_GPIO_OUTPUT);
}

void initBasicPIO(void) {
  hal_adc_set_resolution(ADC_BITS);

  pwmVp37 = hal_pwm_freq_create(PIO_VP37_RPM, VP37_PWM_FREQUENCY_HZ, PWM_RESOLUTION);
  pwmAngle = hal_pwm_freq_create(PIO_VP37_ANGLE, ANGLE_PWM_FREQUENCY_HZ, PWM_RESOLUTION);

  setupOnboardLed();
  hal_ext_adc_init(ADS1115_ADDR, ADC_RANGE);

  for(int a = 0; a < F_LAST; a++) {
    valueFields[a] = 0.0;
  }
}

void initI2C(void) {
  hal_i2c_init(PIN_SDA, PIN_SCL, I2C_SPEED_HZ);
}

void initSPI(void) {
  hal_spi_init(0, PIN_MISO, PIN_MOSI, PIN_SCK);

  hal_display_init(TFT_CS, TFT_DC, TFT_RST);
  hal_display_configure(SCREEN_WIDTH, SCREEN_HEIGHT, 3, true, true);
  hal_display_fill_screen(0);
}

float getSystemSupplyVoltage(void) {
  float val = hal_ext_adc_read_scaled(ADS1115_PIN_1) / (R2 / (R1 + R2));
  return roundfWithPrecisionTo(val, 1);
}

float getVP37FuelTemperature(void) {
  float val = hal_ext_adc_read_scaled(ADS1115_PIN_0);
  val = steinhart(val, R_VP37_FUEL_A, R_VP37_FUEL_B, false);
  return roundfWithPrecisionTo(val, 1);
}

void valToPWM(unsigned char pin, int val) {

  hal_pwm_freq_channel_t ch = NULL;
  switch(pin) {
    case PIO_VP37_RPM:
      ch = pwmVp37;
      break;
    case PIO_VP37_ANGLE:
      ch = pwmAngle;
      break;
    default:
      break;
  }
  if(ch != NULL) {
    hal_pwm_freq_write(ch, PWM_RESOLUTION - val);
  } else {
    derr("config for this pwm is not initialized!");
  }
}

int getVP37Adjustometer(void) {
  float val = hal_ext_adc_read_scaled(ADS1115_PIN_2);
  return (int)(roundfWithPrecisionTo(val, 3) * 1000);
}

int getThrottlePercentage(void) {
  int currentVal = int(valueFields[F_THROTTLE_POS]);
  float percent = (currentVal * 100) / PWM_RESOLUTION;
  return percentToGivenVal(percent, 100);
}
