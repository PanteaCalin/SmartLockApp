// Name:        uartHal.h
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#ifdef PLATFORM_RPI
 #include <uart_linux.h>
#else
 //TODO: include other cpu platform specific uart driver
#endif

// ###########################################################
// Global Definitions
// ###########################################################

// uart API
typedef struct {
 int (*setupPortParams)(char* serialPortName);
 int (*writeData)(int fd, void* writeData, int bytes);
 int (*readData)(int fd, void* readData, int bytes);
} ty_uart_api;

ty_uart_api uart;

// ###########################################################
// Function Prototypes
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPI_uart(void);
