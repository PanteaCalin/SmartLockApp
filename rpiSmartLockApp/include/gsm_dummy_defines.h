// Name:        gsm_dummy.h
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Defines 
// ###########################################################
#define GSMDUMMY_DGB_PRINT_MSG_ENABLE
#ifdef GSMDUMMY_DGB_PRINT_MSG_ENABLE
 #define GSMDUMMY_DGB_PRINT_MSG printf
#else
 #define GSMDUMMY_DGB_PRINT_MSG
#endif

