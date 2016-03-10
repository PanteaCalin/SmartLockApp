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
static int gsm_fibocomg510_sendATcmd(char* atCmdAction, char* atCmd, char* atCmdVal);
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
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);

    // init serial port
    gsmSerialPortFileDescriptor = uart.setupPortParams(GSM_UART_PORT, &serialPortCfg_Default);
    if (gsmSerialPortFileDescriptor < 0) {
        GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -1;
    }
    // turn on GSM module
    gsm_fibocomg510_turnOn();
    sleepMs(GSM_HW_DELAY_AFTER_POWER_UP_MS);

    // verify GSM module power state
    if( gsm_fibocomg510_getPowerState() == VDD_POWER_OFF ) {
        GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -2;
    }

    // test-only
    GSMFIBOCOMG510_PRINT_INFO("----------------------------------------------\n");
    gsm_fibocomg510_sendATcmd(GSM_AT_CMD_READ, GSM_SIGNAL_STRENGHT, NULL);
     sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);
    gsm_fibocomg510_getATcmdResp(RETURN_FULL_RESP, NULL, NULL);
     sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);

     GSMFIBOCOMG510_PRINT_INFO("----------------------------------------------\n");
    gsm_fibocomg510_sendATcmd(GSM_AT_CMD_READ, GSM_SIM_PIN, NULL);
     sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);
    gsm_fibocomg510_getATcmdResp(RETURN_FULL_RESP, NULL, NULL);
     sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);

    GSMFIBOCOMG510_PRINT_INFO("----------------------------------------------\n");
    gsm_fibocomg510_sendATcmd(GSM_AT_CMD_SET, GSM_SIM_PIN, "\"0000\"");
     sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);
    gsm_fibocomg510_getATcmdResp(RETURN_FULL_RESP, NULL, NULL);
     sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);

    GSMFIBOCOMG510_PRINT_INFO("----------------------------------------------\n");
    gsm_fibocomg510_sendATcmd(GSM_AT_CMD_READ, GSM_SIM_PIN, NULL);
     sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);
    gsm_fibocomg510_getATcmdResp(RETURN_FULL_RESP, NULL, NULL);
     sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);

    sleepMs(1000);
    gsm_fibocomg510_turnOff(TURN_OFF_SW);

    // flush I/O UART buffers
    sleepMs(GSM_HW_DELAY_BTWN_ATCMD_AND_RESP);
    uart_flushInOutBuffers(gsmSerialPortFileDescriptor);

    gsm_fibocomg510_initialized = 1;
    return 0;
}

int gsm_fibocomg510_hw_check(void) {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    return 0;
}

int gsm_fibocomg510_call(char* phoneNumber) {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    return 0;
}

int gsm_fibocomg510_sendSMS(char* phoneNumber, char* sms) {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    return 0;
}

int gsm_fibocomg510_getStatus(char *status) {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    return 0;
}

// static function definition
// -----------------------------------------------------------
static int gsm_fibocomg510_sendATcmd(char* atCmdAction, char* atCmd, char* atCmdVal) {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);

    char* atCmdPtr = NULL;
    int atCmdSize = 0;
    int bytesSend = 0;

    // verify the validity of argument pointers
    if( (atCmdAction == NULL) || (atCmd == NULL) || ((atCmdAction == (char*)GSM_AT_CMD_SET) && (atCmdVal == NULL)) ) {
        GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
       return -1;
    }

    // verify the validity of the global serial port file descriptor assigned to GSM module
    if(gsmSerialPortFileDescriptor < 0) {
        GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
       return -2;
    }

    // calculate the size of the heap buffer that has to be allocated to hold the AT command to be send
    atCmdSize = (strlen(GSM_AT_CMD_PREFIX) + \
                 strlen(atCmdAction)       + \
                 strlen(atCmd)             +
                 ((atCmdAction == (char*)GSM_AT_CMD_SET) ? strlen(atCmdVal) : 0) + \
                 1 /* GSM_CARRIAGE_RETURN */
                 ) * sizeof(char);

    // truncate the maximum length of the heap allocated buffer to the maximum SMS payload, as specified by the protocol;
    // SMS has been chosen as the ceiling limit for the buffer size as it looks like the only AT command that would
    // require that much data sent in one AT command.
    // TODO: take into consideration other AT commands too. e.g.: GPRS AT commands.
    // TODO: should I just return an error here when condition is true instead of truncating the size to GSM_MAX_SMS_SIZE_BYTES?
    atCmdSize = (atCmdSize > GSM_MAX_SMS_SIZE_BYTES) ? GSM_MAX_SMS_SIZE_BYTES : atCmdSize /*do nothing*/;

    // TODO: put a little more thought into this. Is it really safe to use malloc in such an application?
    // allocate memory on the HEAP for the AT command string
    atCmdPtr = malloc(atCmdSize);

    if (atCmdPtr == NULL) {
        GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        free(atCmdPtr);
        return -3;
    }

    // initialize the chunk of memory returned by malloc to 0(ZERO)
    memset(atCmdPtr, 0x00, atCmdSize);

    // compose the AT cmd
    strcat(atCmdPtr, (char*)GSM_AT_CMD_PREFIX);
    strcat(atCmdPtr, atCmd);
    strcat(atCmdPtr, atCmdAction);
    ((atCmdAction == (char*)GSM_AT_CMD_SET) ? strcat(atCmdPtr, atCmdVal) : 0  /*do nothing*/ );
    strcat(atCmdPtr, (char*)GSM_CARRIAGE_RETURN);
    //snprintf(atCmdPtr, atCmdSize, "%s%s%s%s", GSM_AT_CMD_PREFIX, GSM_SIGNAL_STRENGHT, GSM_AT_CMD_READ, CR);
    //snprintf(atCmdPtr, atCmdSize, "%s%s%s%s", GSM_AT_CMD_PREFIX, atCmd, atCmdAction, CR);

    GSMFIBOCOMG510_PRINT_INFO("%s - AT Cmd: %s\n",__func__, atCmdPtr);
    GSMFIBOCOMG510_PRINT_INFO("%s - AT Cmd Size: %d\n", __func__, atCmdSize);

    // send AT cmd using serial port
    bytesSend = uart.writeData(gsmSerialPortFileDescriptor, atCmdPtr, atCmdSize);

    // free the heap allocated pointer
    free(atCmdPtr);

    if(bytesSend < 0) {
        GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -4;
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
    GSMFIBOCOMG510_PRINT_INFO("%s - INFO: uart input buffer bytes available: %d\n",__func__, uartInputBufferReceivedBytes);
    if(uartInputBufferReceivedBytes <= 0) {
        GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        return -1;
    }

    // allocate heap buffer for AT Cmd Response
    // TODO: reconsider your options here.
    // Maybe there's a safer way of doing this instead of using dynamic buffer allocation
    char* atCmdRespPtr = malloc(uartInputBufferReceivedBytes * sizeof(char));

    if(atCmdRespPtr == NULL) {
        GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
        free(atCmdRespPtr);
        return -2;
    }

    // read AT Cmd Response
    uart.readInputBuffer(gsmSerialPortFileDescriptor, atCmdRespPtr);
    GSMFIBOCOMG510_PRINT_INFO("%s - INFO: AT Cmd Response: \"%s\"\n",__func__, atCmdRespPtr);

    // parse the response depending on the action demanded
    switch(action) {
        case RETURN_FULL_RESP:
            if(resp == NULL) {
                GSMFIBOCOMG510_PRINT_ERROR("ERROR: file %s, line %d\n",__FILE__, __LINE__);
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
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    switch(mode) {
    case TURN_OFF_HW:
        gpio.setDir(GSM_FIBOCOMG510_GPIO_POWER_ON, GPIO_DIR_OUT);
        gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_POWER_ON); // just to make sure there's a negative edge
        gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_POWER_ON);
        sleepMs(GSM_HW_POWER_DOWN_DELAY_MS);
        gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_POWER_ON);
        break;

    case TURN_OFF_SW:
        gsm_fibocomg510_sendATcmd(GSM_AT_CMD_SET_NO_VALUE, GSM_HW_POWER_DOWN, NULL);
        break;

    default:
        break;
    }
}

static void gsm_fibocomg510_turnOn() {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    gpio.setDir(GSM_FIBOCOMG510_GPIO_POWER_ON, GPIO_DIR_OUT);
    gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_POWER_ON); // just to make sure there's a negative edge
    gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_POWER_ON);
    sleepMs(GSM_HW_POWER_UP_DELAY_MS);
    gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_POWER_ON);
}

static void gsm_fibocomg510_reset() {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    gpio.setDir(GSM_FIBOCOMG510_GPIO_RST, GPIO_DIR_OUT);
    gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_RST);
    sleepMs(GSM_HW_RESET_DELAY_MS);
    gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_RST);
}

static void gsm_fibocomg510_wakeUp() {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    gpio.setDir(GSM_FIBOCOMG510_GPIO_WAKE_UP, GPIO_DIR_OUT);
    gpio.setLvlLow(GSM_FIBOCOMG510_GPIO_WAKE_UP);
    sleepMs(GSM_HW_WAKEUP_DELAY_MS);
    gpio.setLvlHigh(GSM_FIBOCOMG510_GPIO_WAKE_UP);
}

static int gsm_fibocomg510_getOperatingMode() {
    GSMFIBOCOMG510_PRINT_INFO("%s\n", __func__);
    //TODO
    return 0;
}
