// Name:        gsmHal.c
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#include <gsmHal.h>

// ###########################################################
// Function Definition
// ###########################################################

// select GSM Module API
void selectAPI_gsm(void) {

 #ifdef GSM_MODULE_FIBOCOM_G510
  gsm.init      = gsm_fibocomg510_init;
  gsm.hw_check  = gsm_fibocomg510_hw_check;
  gsm.call      = gsm_fibocomg510_call;
  gsm.sendSMS   = gsm_fibocomg510_sendSMS;
  gsm.getStatus = gsm_fibocomg510_getStatus;
 #else
  gsm.init      = gsm_dummy_init;
  gsm.hw_check  = gsm_dummy_hw_check;
  gsm.call      = gsm_dummy_call;
  gsm.sendSMS   = gsm_dummy_sendSMS;
  gsm.getStatus = gsm_dummy_getStatus;
 #endif
}
