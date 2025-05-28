#include "Arduino.h"

#include "peripherials.h"

MyDisplay display(TFT_CS, TFT_DC, TFT_RST);
ADS1115 ADS(ADS1115_ADDR);

NOINIT volatile float valueFields[F_LAST];

m_mutex_def(pwmMutex);
m_mutex_def(i2cMutex);

static pwmConfig pwmVp37;
static pwmConfig pwmAngle;

MyDisplay* get_display(void) {
  return &display;
}

void setupOnboardLed(void) {
  pinMode(LED_BUILTIN, OUTPUT);
}

void initBasicPIO(void) {
  analogReadResolution(ADC_BITS);
  pwm_init();
  setupOnboardLed();
  ADS.begin();

  for(int a = 0; a < F_LAST; a++) {
    valueFields[a] = 0.0;
  }
}

void initI2C(void) {
  m_mutex_init(i2cMutex);
  Wire.setSDA(PIN_SDA);
  Wire.setSCL(PIN_SCL);
  Wire.setClock(I2C_SPEED_HZ);
  Wire.begin();
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
}

float calculateVoltage(int adcValue) {
    float voltage = (adcValue * ADC_RANGE) / 1000;
    return voltage / (R2 / (R1 + R2));
}

static int16_t readADC(uint8_t pin) {
  m_mutex_enter_blocking(i2cMutex);
  ADS.setGain(0);
  int16_t v = ADS.readADC(pin);
  m_mutex_exit(i2cMutex);
  return v;
}

float getSystemSupplyVoltage(void) {
  float val = calculateVoltage(readADC(ADS1115_PIN_1));
  return roundfWithPrecisionTo(val, 1);
}

float getVP37FuelTemperature(void) {
  float val = (readADC(ADS1115_PIN_0) * ADC_RANGE) / 1000;
  val = steinhart(val, R_VP37_FUEL_A, R_VP37_FUEL_B, false);
  return roundfWithPrecisionTo(val, 1);
}

void pwm_configure_channel(pwmConfig *cfg) {

  cfg->analogScale = PWM_RESOLUTION;
  cfg->analogWritePseudoScale = 1;
  while (((clock_get_hz(clk_sys) / ((float)cfg->analogScale * cfg->analogFreq)) > 255.0) && 
          (cfg->analogScale < 32678)) {
      cfg->analogWritePseudoScale++;
      cfg->analogScale *= 2;
  }
  cfg->analogWriteSlowScale = 1;
  while (((clock_get_hz(clk_sys) / ((float)cfg->analogScale * cfg->analogFreq)) < 1.0) && 
        (cfg->analogScale >= 6)) {
      cfg->analogWriteSlowScale++;
      cfg->analogScale /= 2;
  }

  if (!(cfg->pwmInitted & (1 << pwm_gpio_to_slice_num(cfg->pin)))) {
    pwm_config c = pwm_get_default_config();
    pwm_config_set_clkdiv(&c, clock_get_hz(clk_sys) / ((float)cfg->analogScale * cfg->analogFreq));
    pwm_config_set_wrap(&c, cfg->analogScale - 1);
    pwm_init(pwm_gpio_to_slice_num(cfg->pin), &c, true);
    cfg->pwmInitted |= 1 << pwm_gpio_to_slice_num(cfg->pin);
  }
  gpio_set_function(cfg->pin, GPIO_FUNC_PWM);  
}

void pwm_init(void) {

  m_mutex_init(pwmMutex);

  pwmVp37.pin = PIO_VP37_RPM;
  pwmVp37.analogFreq = VP37_PWM_FREQUENCY_HZ;
  pwm_configure_channel(&pwmVp37);

  pwmAngle.pin = PIO_VP37_ANGLE;
  pwmAngle.analogFreq = ANGLE_PWM_FREQUENCY_HZ;
  pwm_configure_channel(&pwmAngle);
}

void pwm_write(pwmConfig *cfg, int val) {

  m_mutex_enter_blocking(pwmMutex);

  val <<= cfg->analogWritePseudoScale;
  val >>= cfg->analogWriteSlowScale;

  if (val < 0) {
      val = 0;
  } else if ((uint32_t)val > cfg->analogScale) {
      val = cfg->analogScale;
  }

  pwm_set_gpio_level(cfg->pin, val);

  m_mutex_exit(pwmMutex);
}

void valToPWM(unsigned char pin, int val) {

  pwmConfig *cfg = NULL;
  switch(pin) {
    case PIO_VP37_RPM:
      cfg = &pwmVp37;
      break;
    case PIO_VP37_ANGLE:
      cfg = &pwmAngle;
      break;
    default:
      break;
  }
  if(cfg != NULL) {
    pwm_write(cfg, (PWM_RESOLUTION - val));
  } else {
    derr("config for this pwm is not initialized!");
  }
}

int getVP37Adjustometer(void) {
  float val = (readADC(ADS1115_PIN_2) * ADC_RANGE) / 1000;
  return (int)(roundfWithPrecisionTo(val, 3) * 1000);
}

int getThrottlePercentage(void) {
#warning getThrottlePercentage
//  int currentVal = int(valueFields[F_THROTTLE_POS]);
//  float percent = (currentVal * 100) / PWM_RESOLUTION;
//  return percentToGivenVal(percent, 100);

  return 0;
}
