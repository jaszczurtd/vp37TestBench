#ifndef T_CONFIG
#define T_CONFIG

#define WATCHDOG_TIME 4000
#define UNSYNCHRONIZE_TIME 15
#define CORE_OPERATION_DELAY 1

//delay time right after start, before first serious alerts will show up (in seconds)
#define SERIOUS_ALERTS_DELAY_TIME 1

//how many main loop cycles between next sensor's read (high importance values)
#define HIGH_READ_CYCLES_AMOUNT 8

//just a general timer tasks info message on main thread
#define THREAD_CONTROL_SECONDS 5

#define MAX_RETRIES 15

//maximum RPM for engine
#define RPM_MAX_EVER 5000

//in miliseconds, print values into serial
#define DEBUG_UPDATE 2000

#endif

