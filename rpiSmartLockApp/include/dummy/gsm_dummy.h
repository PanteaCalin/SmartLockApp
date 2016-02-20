// Name:        gsm_dummy.h
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Includes 
// ###########################################################
#include <gsm_dummy_defines.h>

// ###########################################################
// Function Prototypes 
// ###########################################################

// global functions 
// -----------------------------------------------------------
int gsm_dummy_init(void);
int gsm_dummy_hw_check(void);
int gsm_dummy_call(char* phoneNumber);
int gsm_dummy_sendSMS(char* phoneNumber, char* sms); 
int gsm_dummy_getStatus(char *status);

