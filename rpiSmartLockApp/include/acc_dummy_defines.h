// Name:        acc_dummy_defines.h
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Defines 
// ###########################################################
#define ACCDUMMY_DGB_PRINT_MSG_ENABLE
#ifdef ACCDUMMY_DGB_PRINT_MSG_ENABLE
 #define ACCDUMMY_DGB_PRINT_MSG printf
#else
 #define ACCDUMMY_DGB_PRINT_MSG
#endif

