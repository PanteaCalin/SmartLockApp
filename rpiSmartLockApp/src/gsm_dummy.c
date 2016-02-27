// Name:        gsm_dummy.c
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Includes 
// ###########################################################
#include <gsm_dummy.h>

// ###########################################################
// Function Implementation 
// ###########################################################

// global functions 
// -----------------------------------------------------------
int gsm_dummy_init(void) {
 GSMDUMMY_DGB_PRINT_MSG("GSMDUMMY.init\n");
 return 0;
}
int gsm_dummy_hw_check(void) {
 GSMDUMMY_DGB_PRINT_MSG("GSMDUMMY.hw_check\n");
 return 0;
}
int gsm_dummy_call(char* phoneNumber) {
 GSMDUMMY_DGB_PRINT_MSG("GSMDUMMY.call\n");
 phoneNumber = phoneNumber;
 return 0;
}
int gsm_dummy_sendSMS(char* phoneNumber, char* sms) {
 phoneNumber = phoneNumber;
 GSMDUMMY_DGB_PRINT_MSG("GSMDUMMY.sendSMS\n");
 sms = sms;
 return 0;
}
int gsm_dummy_getStatus(char *status) {
 status = status;
 GSMDUMMY_DGB_PRINT_MSG("GSMDUMMY.getStatus\n");
 return 0;
}

