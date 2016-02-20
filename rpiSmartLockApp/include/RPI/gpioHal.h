// Name:        gpiohal.h
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#ifdef PLATFORM_RPI
 #include <bcm2835.h>
#else
 //TODO: include other cpu platform specific gpio drivers
#endif

// ###########################################################
// Global Definitions
// ###########################################################

// gpio API
typedef struct {
 void (*setDir)(int gpioNo, int gpioDir);
 void (*setLvlHigh)(int gpioNo);
 void (*setLvlLow)(int gpioNo);
 int (*getLvl)(int gpioNo);
} ty_gpio_api;

ty_gpio_api gpio;

// ###########################################################
// Function Prototypes
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPI_gpio(void);
