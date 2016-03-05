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
 #define GPIO_DIR_OUT BCM2835_GPIO_FSEL_OUTP
 #define GPIO_DIR_IN  BCM2835_GPIO_FSEL_INPT
 #define GPIO_HIGH    1
 #define GPIO_LOW     0

 #define GSM_UART_PORT "/dev/ttyAMA0"

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
