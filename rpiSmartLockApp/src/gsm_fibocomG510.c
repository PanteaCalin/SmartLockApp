// Name:        rpiSmartLockApp_fibocomG510GSM.c
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Includes 
// ###########################################################
#include <gsm_fibocomG510.h>
#include <platformHal.h>
#include <gpioHal.h>
#include <uartHal.h>

// ###########################################################
// Global Variables
// ###########################################################
int gsmSerialPortFileDescriptor;
extern ty_serialPortConfig serialPortCfg_Default;
static int gsm_fibocomg510_initialized = 0; //TODO: change type to bool

// ###########################################################
// Static Function Prototypes
// ###########################################################
static int gsm_fibocomg510_sendATcmd(char* atCmdAction, char* atCmd);
static int gsm_fibocomg510_getATResp(char* expectedATResp);
static void gsm_fibocomg510_turnOn();
static void gsm_fibocomg510_turnOff(ty_turnOff_mode mode);
static void gsm_fibocomg510_reset();
static void gsm_fibocomg510_wakeUp();
static ty_vdd_power_status gsm_fibocomg510_getPowerState();
static int gsm_fibocomg510_getOperatingMode();

// ###########################################################
// Function Definition
// ###########################################################

// global function definition
// -----------------------------------------------------------
int gsm_fibocomg510_init(void) {
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);

 // init serial port
 gsmSerialPortFileDescriptor = uart.setupPortParams(GSM_UART_PORT, &serialPortCfg_Default);
 if (gsmSerialPortFileDescriptor < 0) {
     GSMFIBOCOMG510_DGB_PRINT_MSG("%s - ERROR: serial port init failed. GSM init will resume.\n",__func__);
     return -1;
 }

 gsm_fibocomg510_turnOn();

 if( gsm_fibocomg510_getPowerState() == VDD_POWER_OFF ) {
     GSMFIBOCOMG510_DGB_PRINT_MSG("%s - ERROR: gsm_fibocomg510_turnOn failed. GSM init will resume.\n",__func__);
     return -2;
 }

 // test-only
 gsm_fibocomg510_sendATcmd(GSM_AT_CMD_READ, GSM_SIGNAL_STRENGHT);

 gsm_fibocomg510_initialized = 1;
 return 0;
}

int gsm_fibocomg510_hw_check(void) {
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

int gsm_fibocomg510_call(char* phoneNumber) {
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

int gsm_fibocomg510_sendSMS(char* phoneNumber, char* sms) {
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

int gsm_fibocomg510_getStatus(char *status) {
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

// static function definition
// -----------------------------------------------------------
static int gsm_fibocomg510_sendATcmd(char* atCmdAction, char* atCmd) {
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);

 int atCmdSize = strlen(GSM_AT_CMD_PREFIX) * sizeof(char) + \
                     strlen(atCmdAction)       * sizeof(char) + \
                     strlen(atCmd)             * sizeof(char);

 // TODO: put a little more thought into this. Is it really safe to use malloc in such an application?
 // allocate memory on the HEAP for the AT command string
 char* atCmdPtr = malloc(atCmdSize);

 // initialize the chunk of memory returned by malloc to 0(ZERO)
 memset(atCmdPtr, 0x00, atCmdSize);

 // compose the AT cmd
 strcat(atCmdPtr, GSM_AT_CMD_PREFIX);
 strcat(atCmdPtr, atCmd);
 strcat(atCmdPtr, atCmdAction);

 GSMFIBOCOMG510_DGB_PRINT_MSG("%s - AT Cmd: \"%s\"; AT Cmd Size: %d\n",__func__, atCmdPtr, atCmdSize);

 // send AT cmd using serial port
 uart.writeData(gsmSerialPortFileDescriptor, atCmdPtr, atCmdSize);

 // free the heap memory assigned by malloc
 free(atCmdPtr);
}

static ty_vdd_power_status gsm_fibocomg510_getPowerState() {

 gpio.setDir(GSM_FIBOCOMG510_GPIO_VDD, GPIO_DIR_IN);
 return ( (gpio.getLvl(GSM_FIBOCOMG510_GPIO_VDD) == GPIO_HIGH) ? VDD_POWER_ON : VDD_POWER_OFF);
}

static void gsm_fibocomg510_turnOff(ty_turnOff_mode mode) {

 switch(mode) {
  case TURN_OFF_HW:
	  gpio.setDir(GSM_FIBOCOMG510_GPIO_POWER_ON, GPIO_DIR_OUT);
      gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_POWER_ON);
      sleepMs(GSM_HW_POWER_DOWN_DELAY_MS);
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
 GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
 gpio.setDir(GSM_FIBOCOMG510_GPIO_POWER_ON, GPIO_DIR_OUT);
 gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_POWER_ON);
 sleepMs(GSM_HW_POWER_UP_DELAY_MS);
 gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_POWER_ON);
}

static void gsm_fibocomg510_reset() {

 gpio.setDir(GSM_FIBOCOMG510_GPIO_RST, GPIO_DIR_OUT);
 gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_RST);
 sleepMs(GSM_HW_RESET_DELAY_MS);
 gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_RST);
}

static void gsm_fibocomg510_wakeUp() {

 gpio.setDir(GSM_FIBOCOMG510_GPIO_WAKE_UP, GPIO_DIR_OUT);
 gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_WAKE_UP);
 sleepMs(GSM_HW_WAKEUP_DELAY_MS);
 gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_WAKE_UP);
}


