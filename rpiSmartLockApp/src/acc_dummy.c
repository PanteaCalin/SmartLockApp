// Name:        acc_dummy.c
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Includes 
// ###########################################################
#include <acc_dummy.h>

// ###########################################################
// Function Implementation 
// ###########################################################

int acc_dummy_init(void) {
 ACCDUMMY_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

int acc_dummy_hw_check(void) {
 ACCDUMMY_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

int acc_dummy_getStatus(char *status) {
 ACCDUMMY_DGB_PRINT_MSG("%s\n", __func__);
 return 0;
}

