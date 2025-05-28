#include "vpTestBenchOperation.h"

static VP37Pump injectionPump;

VP37Pump& getInjectionPump() {
    return injectionPump;
}

VPTestBenchOperation::VPTestBenchOperation() { }

void VPTestBenchOperation::init(void) {

  injectionPump.init();

  benchInitialized = true;
}

void VPTestBenchOperation::process(void) {
  if(!benchInitialized) {
    return;
  }

#ifndef TEST_CYCLIC

  int thr = getThrottlePercentage();

  if(lastThrottle != thr || desiredAdjustometer < 0) {
    lastThrottle = thr;
    desiredAdjustometer = map(thr, 0, 100, injectionPump.getMinVP37ThrottleValue(), 
                                           injectionPump.getMaxVP37ThrottleValue());

    injectionPump.setVP37Throttle(desiredAdjustometer);
  }

#endif

  injectionPump.process();

}

void VPTestBenchOperation::showDebug(void) {
  if(!benchInitialized) {
    return;
  }  
  
  injectionPump.showDebug();
}

