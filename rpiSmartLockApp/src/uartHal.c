// Name:        uartHal.c
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#include <uartHal.h>

// ###########################################################
// Function Definition
// ###########################################################

// global functions
// -----------------------------------------------------------
void selectAPI_uart(void) {
 #ifdef PLATFORM_RPI
  uart.setupPortParams = uart_setupSerialPortParameters;
  uart.writeData       = uart_writeData;
  uart.readData        = uart_readData;
 #else
  //TODO:
 #endif
}

// static functions
// -----------------------------------------------------------

