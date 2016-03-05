// Name:        platformHal.c
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#include <platformHal.h>

// ###########################################################
// Function Definition
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPI_platform(void) {
 #ifdef PLATFORM_RPI
 platform.init = rpi_init;
 #else
  //TODO:
 #endif
}

// static functions
// -----------------------------------------------------------

