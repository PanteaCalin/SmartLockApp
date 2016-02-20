// Name:        hal.c
// Description:
//
// Author: Calin Pantea
//

// ###########################################################
// Includes
// ###########################################################
#include <hal.h>

// ###########################################################
// Function Definition
// ###########################################################

// select API depending on the hw configuration parameters in makefile
void selectAPIs(void) {

 selectAPI_gpio();
 selectAPI_uart();
 selectAPI_gsm();
 selectAPI_acc();
}
