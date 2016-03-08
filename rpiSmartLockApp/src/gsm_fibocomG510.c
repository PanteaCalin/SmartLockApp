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
int gsmSerialPortFileDescriptor = -1;
extern ty_serialPortConfig serialPortCfg_Default;
static int gsm_fibocomg510_initialized = 0; //TODO: change type to bool

// ###########################################################
// Static Function Prototypes
// ###########################################################
static int gsm_fibocomg510_sendATcmd(char* atCmdAction, char* atCmd);
static int gsm_fibocomg510_getATcmdResp(ty_ATCmdRespAction action, char* resp, char* expectedResp);
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
        GSMFIBOCOMG510_DGB_PRINT_MSG("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -1;
    }
    // turn on GSM module
    gsm_fibocomg510_turnOn();

    // verify GSM module power state
    if( gsm_fibocomg510_getPowerState() == VDD_POWER_OFF ) {
        GSMFIBOCOMG510_DGB_PRINT_MSG("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -2;
    }

    // test-only
    gsm_fibocomg510_sendATcmd(GSM_AT_CMD_READ, GSM_SIGNAL_STRENGHT);
    sleepMs(5000);
    gsm_fibocomg510_getATcmdResp(RETURN_FULL_RESP, NULL, NULL);

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

    int atCmdSize = (strlen(GSM_AT_CMD_PREFIX) + \
                     strlen(atCmdAction)       + \
                     strlen(atCmd)             ) * sizeof(char);

    // TODO: put a little more thought into this. Is it really safe to use malloc in such an application?
    // allocate memory on the HEAP for the AT command string
    char* atCmdPtr = malloc(atCmdSize);

    if(atCmdPtr == NULL) {
        GSMFIBOCOMG510_DGB_PRINT_MSG("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -1;
    }
    // initialize the chunk of memory returned by malloc to 0(ZERO)
    memset(atCmdPtr, 0x00, atCmdSize);

    // compose the AT cmd
    strcat(atCmdPtr, GSM_AT_CMD_PREFIX);
    strcat(atCmdPtr, atCmd);
    strcat(atCmdPtr, atCmdAction);

    GSMFIBOCOMG510_DGB_PRINT_MSG("%s - AT Cmd: \"%s\"; AT Cmd Size: %d\n",__func__, atCmdPtr, atCmdSize);

    if(gsmSerialPortFileDescriptor < 0) {
        GSMFIBOCOMG510_DGB_PRINT_MSG("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -2;
    }

    // send AT cmd using serial port
    int bytesSend = uart.writeData(gsmSerialPortFileDescriptor, atCmdPtr, atCmdSize);

    // free the heap memory assigned by malloc
    free(atCmdPtr);

    if(bytesSend < 0) {
        GSMFIBOCOMG510_DGB_PRINT_MSG("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -3;
    }
    return bytesSend;
}

/* TODO
IMPORTANT: this function's design is very dangerous due to the dynamic allocation nature of
           atCmdRespPtr and the fact that it is passed to the resp argument but by the time
           this function returns, the atCmdRespPtr would have been freed.
           This basically means that the caller function will use resp pointer while it
           is pointing to an un-allocated heap chunk of data.
*/
static int gsm_fibocomg510_getATcmdResp(ty_ATCmdRespAction action, char* resp, char* expectedResp) {

    int uartInputBufferReceivedBytes = uart.getInputBytesAvailable(gsmSerialPortFileDescriptor);
    GSMFIBOCOMG510_DGB_PRINT_MSG("%s - INFO: uart input buffer bytes available: %d\n",__func__, uartInputBufferReceivedBytes);
    if(uartInputBufferReceivedBytes <= 0) {
        GSMFIBOCOMG510_DGB_PRINT_MSG("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -1;
    }

    // allocate heap buffer for AT Cmd Response
    // TODO: reconsider your options here.
    // Maybe there's a safer way of doing this instead of using dynamic buffer allocation
    char* atCmdRespPtr = malloc(uartInputBufferReceivedBytes * sizeof(char));

    if(atCmdRespPtr == NULL) {
        GSMFIBOCOMG510_DGB_PRINT_MSG("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        free(atCmdRespPtr);
        return -2;
    }

    // read AT Cmd Response
    uart.readInputBuffer(gsmSerialPortFileDescriptor, atCmdRespPtr);
    GSMFIBOCOMG510_DGB_PRINT_MSG("%s - INFO: AT Cmd Response: \"%s\"\n",__func__, atCmdRespPtr);

    // parse the response depending on the action demanded
    switch(action) {
        case RETURN_FULL_RESP:
            if(resp == NULL) {
                GSMFIBOCOMG510_DGB_PRINT_MSG("ERROR: file %s, line %d\n",__FILE__, __LINE__);
                free(atCmdRespPtr);
                return -3;
            }
            resp = atCmdRespPtr;
            break;
        case RETURN_RESP_VALUE:
            //TODO
            break;
        case MATCH_EXPECTED:
            //TODO
            break;
        case SEARCH_EXPECTED:
            //TODO
            break;
        default:
            //TODO
            break;
    }

    free(atCmdRespPtr);
    return 0;
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
    GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
    gpio.setDir(GSM_FIBOCOMG510_GPIO_RST, GPIO_DIR_OUT);
    gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_RST);
    sleepMs(GSM_HW_RESET_DELAY_MS);
    gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_RST);
}

static void gsm_fibocomg510_wakeUp() {
    GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
    gpio.setDir(GSM_FIBOCOMG510_GPIO_WAKE_UP, GPIO_DIR_OUT);
    gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_WAKE_UP);
    sleepMs(GSM_HW_WAKEUP_DELAY_MS);
    gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_WAKE_UP);
}

static int gsm_fibocomg510_getOperatingMode() {
    GSMFIBOCOMG510_DGB_PRINT_MSG("%s\n", __func__);
    //TODO
    return 0;
}
