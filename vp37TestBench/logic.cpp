
#include "logic.h"

const char *err = (char*)F("ERR");

bool callAtEverySecond(void *arg);
bool callAtEveryHalfSecond(void *arg);
bool callAtEveryHalfHalfSecond(void *arg);
bool updateValsForDebug(void *arg);

static unsigned long alertsStartSecond = 0;
static unsigned long lastThreadSeconds = 0;
static Timer generalTimer;

NOINIT int statusVariable0;
NOINIT int statusVariable1;

static VPTestBenchOperation benchOperation;

void setupTimerWith(unsigned long time, bool(*function)(void *argument)) {
  //watchdog_feed();
  generalTimer.every(time, function);
  m_delay(CORE_OPERATION_DELAY);
}

void setupTimers(void) {

  generalTimer = timer_create_default();

  setupTimerWith(SECOND, callAtEverySecond);
  setupTimerWith(SECOND / 2, callAtEveryHalfSecond);
  setupTimerWith(SECOND / 4, callAtEveryHalfHalfSecond);
  setupTimerWith(DEBUG_UPDATE, updateValsForDebug);
}

static int *wValues = NULL;
static int wSize = 0;
void executeByWatchdog(int *values, int size) {
  wValues = values;
  wSize = size;
}

void setup_a(void) {

  debugInit();

  bool rebooted = setupWatchdog(executeByWatchdog, WATCHDOG_TIME);
  if(!rebooted) {
    statusVariable0 = statusVariable1 = 0;
  }

  initSPI();
  initI2C();
  initBasicPIO();

  watchdog_feed();

  #ifdef DEBUG_SCREEN
  debugFunc();
  #else  

  #endif

  alertsStartSecond = getSeconds() + SERIOUS_ALERTS_DELAY_TIME;

  callAtEverySecond(NULL);
  callAtEveryHalfSecond(NULL);
  callAtEveryHalfHalfSecond(NULL);
  updateValsForDebug(NULL);

  watchdog_feed();
  setupTimers();

  benchOperation.init();

  //just a demo
  MyDisplay *display = get_display();
  display->setFont();      // Use default (built-in) font
  display->setTextSize(2); // and 2X size for chart label
  
  display->setCursor((display->width() - 144) / 2, 0);
  display->setTextColor(0x0210);
  display->print(F("Widget Sales")); // F("string") is in program memory, not RAM

  float thickness = 2.0;
  int charWidth = 16;
  String str;
  int w, x;
  
  str = "12:34.56+2-1 102^ 99%"; 
  w = display->get7SegStringWidth(str, charWidth, thickness);
  x = (SCREEN_WIDTH - w) / 2;
  display->draw7SegString(str, x, 50, charWidth, 24, thickness, ST77XX_WHITE);

  str = "1ABCDEF2"; 
  w = display->get7SegStringWidth(str, charWidth, thickness);
  x = (SCREEN_WIDTH - w) / 2;
  display->draw7SegString(str, x, 80, charWidth, 24, thickness, ST77XX_WHITE);


  setStartedCore0();
}

void loop_a(void) {

  statusVariable0 = 0;
  updateWatchdogCore0();

  if(!isEnvironmentStarted()) {
    statusVariable0 = -1;
    m_delay(CORE_OPERATION_DELAY);  
    tight_loop_contents();
    return;
  }

  statusVariable0 = 1;

  generalTimer.tick();
  if(lastThreadSeconds < getSeconds()) {
    lastThreadSeconds = getSeconds() + THREAD_CONTROL_SECONDS;

    deb("thread is alive, active tasks: %d", generalTimer.size());
  }
  statusVariable0 = 2;

  benchOperation.showDebug();

  m_delay(CORE_OPERATION_DELAY);  
  tight_loop_contents();
}

static bool alertBlink = false, seriousAlertBlink = false;
bool alertSwitch(void) {
  return alertBlink;
}
bool seriousAlertSwitch(void) {
  return seriousAlertBlink;
}

//timer functions
bool callAtEverySecond(void *arg) {
  alertBlink = !alertBlink;

  digitalWrite(LED_BUILTIN, alertBlink);

#if SYSTEM_TEMP
  deb("System temperature: %f", analogReadTemp());
#endif

  return true;
}

bool callAtEveryHalfSecond(void *arg) {
  seriousAlertBlink = !seriousAlertBlink;

  return true;
}

bool callAtEveryHalfHalfSecond(void *arg) {

  return true;
}

void setup_b(void) {
  setStartedCore1();
}

void loop_b(void) {
  updateWatchdogCore1();

  if(!isEnvironmentStarted()) {
    statusVariable1 = -1;
    m_delay(CORE_OPERATION_DELAY);  
    tight_loop_contents();
    return;
  }
  statusVariable1 = 1;

  benchOperation.process();

  m_delay(CORE_OPERATION_DELAY);  
  tight_loop_contents();
}

bool updateValsForDebug(void *arg) {

  deb("alive tasks: %d", generalTimer.size());

  return true;
}