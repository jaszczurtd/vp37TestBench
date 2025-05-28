#ifndef VP_TESTBENCH_OPERATION
#define VP_TESTBENCH_OPERATION

#include <tools.h>
#include <arduino-timer.h>
#include <pidController.h>
#include <pico.h>
#include <hardware/timer.h>

#include "config.h"
#include "logic.h"
#include "hardwareConfig.h"
#include "vp37_defs.h"
#include "vp37.h"

#include "EngineController.h"

class VPTestBenchOperation : public EngineController {
private:
  bool benchInitialized;
  int lastThrottle;
  int desiredAdjustometer;

public:
  VPTestBenchOperation();
  void init() override;  
  void process() override;
  void tickPumpTimer(void);

  void showDebug(void);
};

class VP37Pump;

VP37Pump& getInjectionPump();

#endif

