// Name:        gpioHal.c
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#include <gpioHal.h>

// ###########################################################
// Function Definition
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPI_gpio(void) {
 #ifdef PLATFORM_RPI
  gpio.setDir     = bcm2835_gpio_fsel;
  gpio.setLvlHigh = bcm2835_gpio_set;
  gpio.setLvlLow  = bcm2835_gpio_clr;
  gpio.getLvl     = bcm2835_gpio_lev;
 #else
  //TODO:
 #endif
}

// static functions
// -----------------------------------------------------------

