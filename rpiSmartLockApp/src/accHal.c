// Name:        accHal.c
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#include <accHal.h>

// ###########################################################
// Function Definition
// ###########################################################

// select Accelerometer Module API
void selectAPI_acc(void) {

 #ifdef ACCELEROMETER_NXP_MMA7660FCT
  //TODO: add here API functions for NXP_MMA7660FCT accelerometer module
 #else
  acc.init      = acc_dummy_init;
  acc.hw_check  = acc_dummy_hw_check;
  acc.getStatus = acc_dummy_getStatus;
 #endif
}
