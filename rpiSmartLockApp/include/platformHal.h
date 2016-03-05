// Name:        platformHal.h
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#ifdef PLATFORM_RPI
 #include <platform_rpi.h>
#else
 //TODO: include other cpu platform specific headers
#endif

// ###########################################################
// Global Definitions
// ###########################################################

// platform API
typedef struct {
 void (*init)();
} ty_platform_api;

ty_platform_api platform;

// ###########################################################
// Function Prototypes
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPI_platform(void);
