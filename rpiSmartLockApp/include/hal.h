// Name:        hal.h
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#ifdef PLATFORM_RPI
 #include <stdio.h>
 #include <unistd.h> // needed for usleep(); TODO: review this
#else
 //TODO: other platform here
#endif

#include <gpioHal.h>
#include <uartHal.h>
#include <gsmHal.h>
#include <accHal.h>

// ###########################################################
// Global Definitions
// ###########################################################

#ifdef PLATFORM_RPI
 #define sleepMs(x) usleep(x*1000)
#else
 //TODO: other platform here
#endif

// ###########################################################
// Function Prototypes
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPIs(void);

// static functions
// -----------------------------------------------------------
