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

// global function declarations
// -----------------------------------------------------------
int gsm_fibocomg510_init(void);
int gsm_fibocomg510_hw_check(void);
int gsm_fibocomg510_call(char* phoneNumber);
int gsm_fibocomg510_sendSMS(char* phoneNumber, char* sms); 
int gsm_fibocomg510_getStatus(char *status);

// static function declarations
// -----------------------------------------------------------
static int gsm_fibocomg510_sendATCmd(char* atCmdAction, char* atCmd);
static int gsm_fibocomg510_getATResp(char* expectedATResp);
static void gsm_fibocomg510_turnOn();
static void gsm_fibocomg510_turnOff(ty_turnOff_mode mode);
static void gsm_fibocomg510_reset();
static void gsm_fibocomg510_wakeUp();
static ty_vdd_power_status gsm_fibocomg510_getPowerState();
static int gsm_fibocomg510_getOperatingMode();
