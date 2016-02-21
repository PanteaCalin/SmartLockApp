// Name:        accHal.h
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#ifdef ACCELEROMETER_NXP_MMA7660FCT
 //TODO: include here the header for NXP_MMA7660FCT acc driver
#else
 #include <acc_dummy.h>
#endif

// ###########################################################
// Global Definitions
// ###########################################################

// Accelerometer Module API
typedef struct {
 int (*init)(void);
 int (*hw_check)(void);
 int (*getStatus)(char *status);
} ty_acc_api;

ty_acc_api acc;

// ###########################################################
// Function Prototypes
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPI_acc(void);
