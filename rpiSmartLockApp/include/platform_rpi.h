// Name:        platform_rpi.h
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#include <stdio.h>
#include <unistd.h> // needed for usleep(); TODO: review this
#include <bcm2835.h>

// ###########################################################
// Defines
// ###########################################################
#define PLATFORMRPI_DGB_PRINT_MSG_ENABLE
#ifdef PLATFORMRPI_DGB_PRINT_MSG_ENABLE
 #define PLATFORMRPI_DGB_PRINT_MSG printf
#else
 #define PLATFORMRPI_DGB_PRINT_MSG
#endif

#define sleepMs(x) usleep(x*1000)

// ###########################################################
// Function Prototypes
// ###########################################################

// static function prototypes
// -----------------------------------------------------------

// global function prototypes
// -----------------------------------------------------------
void rpi_init(void);
