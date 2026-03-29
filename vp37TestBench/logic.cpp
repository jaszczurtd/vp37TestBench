
#include "logic.h"

const char *err = (char*)F("ERR");

static void callAtEverySecond(void);
static void callAtEveryHalfSecond(void);
static void callAtEveryHalfHalfSecond(void);
static void updateValsForDebug(void);

static unsigned long alertsStartSecond = 0;
static unsigned long lastThreadSeconds = 0;

static SmartTimers timerSecond;
static SmartTimers timerHalfSecond;
static SmartTimers timerQuarterSecond;
static SmartTimers timerDebug;

NOINIT int statusVariable0;
NOINIT int statusVariable1;

static VPTestBenchOperation benchOperation;

static int *wValues = NULL;
static int wSize = 0;
void executeByWatchdog(int *values, int size) {
  wValues = values;
  wSize = size;
}

void setupTimers(void) {
  watchdog_feed();
  timerSecond.begin(callAtEverySecond, SECOND);
  watchdog_feed();
  timerHalfSecond.begin(callAtEveryHalfSecond, SECOND / 2);
  watchdog_feed();
  timerQuarterSecond.begin(callAtEveryHalfHalfSecond, SECOND / 4);
  watchdog_feed();
  timerDebug.begin(updateValsForDebug, DEBUG_UPDATE);
}

void setup_a(void) {

  debugInit();

  bool rebooted = hal_watchdog_caused_reboot();
  if(!rebooted) {
    statusVariable0 = statusVariable1 = 0;
  }
  setupWatchdog(executeByWatchdog, WATCHDOG_TIME);

  initSPI();
  initI2C();
  initBasicPIO();

  watchdog_feed();

  #ifdef DEBUG_SCREEN
  debugFunc();
  #else

  #endif

  alertsStartSecond = getSeconds() + SERIOUS_ALERTS_DELAY_TIME;

  callAtEverySecond();
  callAtEveryHalfSecond();
  callAtEveryHalfHalfSecond();
  updateValsForDebug();

  watchdog_feed();
  setupTimers();

  benchOperation.init();

  //just a demo
  hal_display_set_font(HAL_FONT_DEFAULT);
  hal_display_set_text_size(2);

  hal_display_set_cursor((hal_display_get_width() - 144) / 2, 0);
  hal_display_set_text_color(0x0210);
  hal_display_print("Widget Sales");

  float thickness = 2.0;
  int charWidth = 16;
  String str;
  int w, x;

  str = "12:34.56+2-1 102^ 99%";
  w = get7SegStringWidth(str.c_str(), charWidth, thickness);
  x = (SCREEN_WIDTH - w) / 2;
  draw7SegString(str.c_str(), x, 50, charWidth, 24, thickness, ST77XX_WHITE);

  str = "1ABCDEF2";
  w = get7SegStringWidth(str.c_str(), charWidth, thickness);
  x = (SCREEN_WIDTH - w) / 2;
  draw7SegString(str.c_str(), x, 80, charWidth, 24, thickness, ST77XX_WHITE);


  setStartedCore0();
}

void loop_a(void) {

  statusVariable0 = 0;
  updateWatchdogCore0();

  if(!isEnvironmentStarted()) {
    statusVariable0 = -1;
    hal_delay_ms(CORE_OPERATION_DELAY);
    hal_idle();
    return;
  }

  statusVariable0 = 1;

  timerSecond.tick();
  timerHalfSecond.tick();
  timerQuarterSecond.tick();
  timerDebug.tick();

  if(lastThreadSeconds < getSeconds()) {
    lastThreadSeconds = getSeconds() + THREAD_CONTROL_SECONDS;
    deb("thread is alive");
  }
  statusVariable0 = 2;

  benchOperation.showDebug();

  hal_delay_ms(CORE_OPERATION_DELAY);
  hal_idle();
}

static bool alertBlink = false, seriousAlertBlink = false;
bool alertSwitch(void) {
  return alertBlink;
}
bool seriousAlertSwitch(void) {
  return seriousAlertBlink;
}

//timer functions
static void callAtEverySecond(void) {
  alertBlink = !alertBlink;

  hal_gpio_write(LED_BUILTIN, alertBlink);

#if SYSTEM_TEMP
  deb("System temperature: %f", hal_read_chip_temp());
#endif
}

static void callAtEveryHalfSecond(void) {
  seriousAlertBlink = !seriousAlertBlink;
}

static void callAtEveryHalfHalfSecond(void) {
}

void setup_b(void) {
  setStartedCore1();
}

void loop_b(void) {
  updateWatchdogCore1();

  if(!isEnvironmentStarted()) {
    statusVariable1 = -1;
    hal_delay_ms(CORE_OPERATION_DELAY);
    hal_idle();
    return;
  }
  statusVariable1 = 1;

  benchOperation.process();

  hal_delay_ms(CORE_OPERATION_DELAY);
  hal_idle();
}

static void updateValsForDebug(void) {
  deb("alive");
}
