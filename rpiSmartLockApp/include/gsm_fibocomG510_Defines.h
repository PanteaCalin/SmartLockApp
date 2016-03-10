// Name:        rpiSmartLockApp_fibocomG510GSM_Defines.h
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Defines 
// ###########################################################
#define GSMFIBOCOMG510_DGB_PRINT_INFO_ENABLE
#define GSMFIBOCOMG510_DGB_PRINT_ERROR_ENABLE

#ifdef GSMFIBOCOMG510_DGB_PRINT_INFO_ENABLE
 #define GSMFIBOCOMG510_PRINT_INFO printf
#else
 #define GSMFIBOCOMG510_PRINT_INFO
#endif

#ifdef GSMFIBOCOMG510_DGB_PRINT_ERROR_ENABLE
 #define GSMFIBOCOMG510_PRINT_ERROR printf
#else
 #define GSMFIBOCOMG510_PRINT_ERROR
#endif

// Fibocom G510 GSM Module Parameters
// -----------------------------------------------------------
#define GSM_HW_POWER_DOWN_DELAY_MS     3100 // data-sheet recommends min 3000ms
#define GSM_HW_POWER_UP_DELAY_MS       1500 // data-sheet recommends min 800ms
#define GSM_HW_RESET_DELAY_MS          100  // not stated anywhere in documentation
#define GSM_HW_WAKEUP_DELAY_MS         100  // not stated anywhere in documentation
#define GSM_HW_DELAY_AFTER_POWER_UP_MS 3000 // min amount of time to wait between power-up sequence
                                            // and the state when gsm is ready to received AT cmds
#define GSM_HW_DELAY_BTWN_ATCMD_AND_RESP 500 // delay between AT cmd sent and response, due to slow nature of UART

#define GSM_CARRIAGE_RETURN "\r"

// TODO: move this define in main app as it is universally applicable not specific to this GSM module
// NOTE: protocol specifies SMS maximum payload of 140 bytes.
// This means that maximum 140 8-bit characters or 160 7-bit characters can be send in one SMS chunk.
#define GSM_MAX_SMS_SIZE_BYTES 140

// Fibocom G510 GSM Module AT Commands Prefix
// -----------------------------------------------------------
#define GSM_AT_CMD_PREFIX "AT"

// Fibocom G510 GSM Module AT Commands
// -----------------------------------------------------------
#define GSM_SIGNAL_STRENGHT             "+CSQ"
#define GSM_OPERATOR_SELECTION          "+COPS"
#define GSM_NETWORK_REGISTRATION_STATUS "+CREG"
#define GSM_SIM_PIN			            "+CPIN"
#define GSM_DIAL                        "D"

#define GSM_HW_POWER_DOWN "+MRST"

// Modem ID Commands
#define GSM_MANUFACTURER_ID "+CGMI"
#define GSM_MODEL_ID        "+CGMM"
#define GSM_REVISION        "+CGMR"

// Fibocom G510 GSM Module AT Commands Action
// -----------------------------------------------------------
#define GSM_AT_CMD_SET	        "="
#define GSM_AT_CMD_SET_NO_VALUE ""
#define GSM_AT_CMD_READ	        "?"
#define GSM_AT_CMD_TEST	        "=?"

// Fibocom G510 GSM Module Hardware States
// -----------------------------------------------------------

typedef enum {
   VDD_POWER_OFF,
   VDD_POWER_ON
} ty_vdd_power_status;

typedef enum {
	TURN_OFF_SW,
	TURN_OFF_HW
} ty_turnOff_mode;

typedef enum {
    RETURN_FULL_RESP,
    RETURN_RESP_VALUE,
    MATCH_EXPECTED,
    SEARCH_EXPECTED
} ty_ATCmdRespAction;
