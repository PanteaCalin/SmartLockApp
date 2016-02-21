// Name:        rpiSmartLockApp_Defines.h
// Description: 
//
// Author: Calin Pantea
//

// ###########################################################
// Defines 
// ###########################################################

// Raspberry PI platform
#ifdef PLATFORM_RPI
 // common
 #define GPIO_DIR_OUT 0 //TODO: replace '0' with bcm2835.h define
 #define GPIO_DIR_IN  1 //TODO: replace '1' with bcm2835.h define

 // GSM Module defines
 #ifdef GSM_MODULE_FIBOCOM_G510
  //TODO: update these defines accordingly
  #define GSM_FIBOCOMG510_GPIO_POWER_ON 0
  #define GSM_FIBOCOMG510_GPIO_VDD      1
  #define GSM_FIBOCOMG510_GPIO_WAKE_UP  2
  #define GSM_FIBOCOMG510_GPIO_RING     3
  #define GSM_FIBOCOMG510_GPIO_RST      4
  #define GSM_FIBOCOMG510_GPIO_LPG      5
 #endif
#else
 //TODO: some other platform e.g: NXP
#endif
