// Name:        rpiSmartLockApp_fibocomG510GSM.c
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Includes 
// ###########################################################
#include <gsm_fibocomG510.h>
#include <gpioHal.h>

// ###########################################################
// Function Definition
// ###########################################################

// global function definition
// -----------------------------------------------------------
int gsm_fibocomg510_init(void) {
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

int gsm_fibocomg510_hw_check(void) {
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

int gsm_fibocomg510_call(char* phoneNumber) {
 phoneNumber = phoneNumber;
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

int gsm_fibocomg510_sendSMS(char* phoneNumber, char* sms) {
 phoneNumber = phoneNumber;
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 sms = sms;
}

int gsm_fibocomg510_getStatus(char *status) {
 status = status;
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

// static function definition
// -----------------------------------------------------------

static int gsm_fibocomg510_sendATcmd(char* atCmdAction, char* atCmd) {
  GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
  atCmdAction = atCmdAction;
  atCmd = atCmd;
}

static ty_vdd_power_status gsm_fibocomg510_getPowerState() {

 gpio.setDir(GSM_FIBOCOMG510_GPIO_VDD, GPIO_DIR_IN);
 return ( (gpio.getLvl(GSM_FIBOCOMG510_GPIO_VDD) == 1) ? VDD_POWER_ON : VDD_POWER_OFF);
}

static void gsm_fibocomg510_turnOff(ty_turnOff_mode mode) {

 switch(mode) {
  case TURN_OFF_HW:
	  gpio.setDir(GSM_FIBOCOMG510_GPIO_POWER_ON, GPIO_DIR_OUT);
      gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_POWER_ON);
      //sleepMs(GSM_HW_POWER_DOWN_DELAY_MS); //TODO: implement a sleep function
      gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_POWER_ON);
      break;

  case TURN_OFF_SW:
	  gsm_fibocomg510_sendATcmd(GSM_AT_CMD_SET, GSM_HW_POWER_DOWN);
	  break;

  default:
	  break;
 }
}

static void gsm_fibocomg510_turnOn() {

 gpio.setDir(GSM_FIBOCOMG510_GPIO_POWER_ON, GPIO_DIR_OUT);
 gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_POWER_ON);
 //sleepMs(GSM_HW_POWER_UP_DELAY_MS); //TODO: impement sleep function
 gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_POWER_ON);
}

static void gsm_fibocomg510_reset() {

 gpio.setDir(GSM_FIBOCOMG510_GPIO_RST, GPIO_DIR_OUT);
 gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_RST);
 //sleepMs(GSM_HW_RESET_DELAY_MS); //TODO: impement sleep function
 gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_RST);
}

static void gsm_fibocomg510_wakeUp() {

 gpio.setDir(GSM_FIBOCOMG510_GPIO_WAKE_UP, GPIO_DIR_OUT);
 gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_WAKE_UP);
 //sleepMs(GSM_HW_WAKEUP_DELAY_MS); //TODO: impement sleep function
 gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_WAKE_UP);
}


