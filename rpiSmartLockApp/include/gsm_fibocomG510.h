// Name:        rpiSmartLockApp_fibocomG510GSM.h
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Includes 
// ###########################################################
#include <rpiSmartLockApp_Defines.h>
#include <gsm_fibocomG510_Defines.h>

// ###########################################################
// Function Prototypes 
// ###########################################################

// static function declarations
// -----------------------------------------------------------

// global function declarations
// -----------------------------------------------------------
int gsm_fibocomg510_init(void);
int gsm_fibocomg510_hw_check(void);
int gsm_fibocomg510_call(char* phoneNumber);
int gsm_fibocomg510_sendSMS(char* phoneNumber, char* sms);
int gsm_fibocomg510_getStatus(char *status);
