// Name:        gsmHal.h
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#ifdef GSM_MODULE_FIBOCOM_G510
 #include <gsm_fibocomG510.h>
#else
 #include <gsm_dummy.h>
#endif

// ###########################################################
// Global Definitions
// ###########################################################

// GSM Module API
typedef struct {
 int (*init)(void);
 int (*hw_check)(void);
 int (*call)(char* phoneNumber);
 int (*sendSMS)(char* phoneNumber, char* sms);
 int (*getStatus)(char *status);
} ty_gsm_api;

ty_gsm_api gsm;

// ###########################################################
// Function Prototypes
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPI_gsm(void);
