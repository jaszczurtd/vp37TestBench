
#ifndef C_LOGIC
#define C_LOGIC

#include <tools.h>

#include "config.h"

#include "hardwareConfig.h"
#include "peripherials.h"

#include "vpTestBenchOperation.h"

void setup_a(void);
void loop_a(void);
void setup_b(void);
void loop_b(void);

bool alertSwitch(void);
bool seriousAlertSwitch(void);

#endif
