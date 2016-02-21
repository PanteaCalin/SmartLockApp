// Name:        rpiSmartLockApp.c
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Includes 
// ###########################################################
#include <rpiSmartLockApp.h>

// ###########################################################
// Function Definition 
// ###########################################################

int main(int argc, char *argv[]) {

 MAIN_DGB_PRINT_MSG("Main app start\n");

 selectAPIs();
 gsm.init();

 MAIN_DGB_PRINT_MSG("Main app done. Will Resume.\n");
 return 0;
}


