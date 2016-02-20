#################################################################################
# Name:        Makefile                                                         # 	
# Description: Makefile for Smart Lock Application                              #
# Author:      Calin Pantea                                                     #	
#################################################################################

# ------------------------------[ Application ]-------------------------------- #

EXEC_NAME = rpiSmartLockApp

# ------------------------------[ Hardware system configuration ]-------------- #

# platform
# <options>: RPI
PLATFORM = RPI

# gsm module
# <options>: FIBOCOM_G510
#            DUMMY
#            UNDEFINED
GSM_MODULE = FIBOCOM_G510

# accelerometer 
# <options>: NXP_MMA7660FCT
#            DUMMY
#	         UNDEFINED	  
ACCELEROMETER = DUMMY

# pir sensor
# <options>: PANASONIC_EKMC1601113
#            DUMMY
#            UNDEFINED
PIR_SENSOR = DUMMY

# ------------------------------[ Project Location Related ]------------------- #

PRJ_DIR       = $(shell pwd)
SRC_DIR       = $(PRJ_DIR)/src/$(PLATFORM)/
DUMMY_SRC_DIR = $(PRJ_DIR)/src/dummy/
INC_DIR       = $(PRJ_DIR)/include/$(PLATFORM)/
DUMMY_INC_DIR = $(PRJ_DIR)/include/dummy/
OUT_DIR       = $(PRJ_DIR)/output/

# ------------------------------[ Build Options ]------------------------------ #

CC     = gcc
DEBUG  = -g
CFLAGS = -c -Wall -I$(INC_DIR) -I$(DUMMY_INC_DIR)
OFLAGS = -o
BUILD_CMD_VEROSITY = @ 

# objects built regardless of the hw system configuration:
# main application object
OBJS += $(OUT_DIR)rpiSmartLockApp.o

# hardware abstraction layer objects
OBJS += $(OUT_DIR)hal.o
OBJS += $(OUT_DIR)gpioHal.o
OBJS += $(OUT_DIR)uartHal.o
OBJS += $(OUT_DIR)gsmHal.o
OBJS += $(OUT_DIR)accHal.o  

# platform dependent build flags
ifneq ($(PLATFORM),UNDEFINED )
 ifeq ($(PLATFORM),RPI)
  HW_CFG += -DPLATFORM_RPI
  OBJS += $(OUT_DIR)uart_linux.o
  OBJS += $(OUT_DIR)bcm2835.o  
 else
  $(error ERROR - invalid platform: $(PLATFORM).)
 endif	
else
 $(error ERROR - invalid platform: $(PLATFORM). A valid platfrom must be specified.)
endif

# gsm module dependent build flags
ifneq ($(GSM_MODULE),UNDEFINED)
 ifeq ($(GSM_MODULE),FIBOCOM_G510)
  HW_CFG += -DGSM_MODULE_FIBOCOM_G510
  OBJS += $(OUT_DIR)gsm_fibocomG510.o
 else ifeq ($(GSM_MODULE),DUMMY)
  HW_CFG += -DGSM_MODULE_DUMMY
  OBJS += $(OUT_DIR)gsm_dummy.o
 else
  $(error ERROR - invalid gsm_module: $(GSM_MODULE))
 endif
else 
 $(warning WARNING - undefined gsm_module.)
endif

# accelerometer dependent build flags
ifneq ($(ACCELEROMETER),UNDEFINED)
 ifeq ($(ACCELEROMETER),NXP_MMA7660FCT)
  HW_CFG += -DACCELEROMETER_NXP_MMA7660FCT
  #TODO add accelerometer module related objects here.
 else ifeq ($(ACCELEROMETER),DUMMY)
  HW_CFG += -DACCELEROMETER_DUMMY
  OBJS += $(OUT_DIR)acc_dummy.o
 endif
else
 $(warning WARNING - undefined accelerometer sensor module)
endif

# pir sensor dependent build flags
ifneq ($(PIR_SENSOR),UNDEFINED)
 ifeq ($(PIR_SENSOR),PANASONIC_EKMC1601113)
  HW_CFG += -DPIR_SENSOR_PANASONIC_EKMC1601113
  #TODO add pir sensor module related objects here.
 endif
else
 $(warning WARNING - undefined pir sensor module)
endif

# ------------------------------[ Targets ] ----------------------------------- #
# make targets
# TODO: find a way to automatically declare target rules based on the OBJS list;
# declare here rules to make all the possible targets in the application;
# only the objects found in $(OBJS) will actually be built and linked in the final app.

# make all; will build only the targets found in $(OBJS) 
all: $(OBJS)
	@echo "=========================================================="
	@echo "========== Building $(EXEC_NAME) application =========="
	@echo "Following object files will be build and linked:\n"
	@$(foreach obj,$(OBJS),echo $(obj);)
	@echo ""
	$(BUILD_CMD_VEROSITY) $(CC) $(OBJS) $(OFLAGS) $(OUT_DIR)$(EXEC_NAME)
	@echo "$(EXEC_NAME) application built sucessfully"
	@echo "=========================================================="

# make clean; will delete everytinhg in output/ directory
clean:
	@echo "Removing all files from $(OUT_DIR)" 
	rm -f $(OUT_DIR)*
	@echo "make clean ... Done"

$(OUT_DIR)rpiSmartLockApp.o: $(SRC_DIR)rpiSmartLockApp.c $(INC_DIR)rpiSmartLockApp.h $(INC_DIR)rpiSmartLockApp_Defines.h  
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(HW_CFG) $(SRC_DIR)rpiSmartLockApp.c $(OFLAGS) $(OUT_DIR)rpiSmartLockApp.o

$(OUT_DIR)hal.o: $(SRC_DIR)hal.c $(INC_DIR)hal.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(HW_CFG) $(SRC_DIR)hal.c $(OFLAGS) $(OUT_DIR)hal.o

$(OUT_DIR)gpioHal.o: $(SRC_DIR)gpioHal.c $(INC_DIR)gpioHal.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(HW_CFG) $(SRC_DIR)gpioHal.c $(OFLAGS) $(OUT_DIR)gpioHal.o

$(OUT_DIR)uartHal.o: $(SRC_DIR)uartHal.c $(INC_DIR)uartHal.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(HW_CFG) $(SRC_DIR)uartHal.c $(OFLAGS) $(OUT_DIR)uartHal.o

$(OUT_DIR)gsmHal.o: $(SRC_DIR)gsmHal.c $(INC_DIR)gsmHal.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(HW_CFG) $(SRC_DIR)gsmHal.c $(OFLAGS) $(OUT_DIR)gsmHal.o

$(OUT_DIR)accHal.o: $(SRC_DIR)accHal.c $(INC_DIR)accHal.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(HW_CFG) $(SRC_DIR)accHal.c $(OFLAGS) $(OUT_DIR)accHal.o

$(OUT_DIR)uart_linux.o: $(SRC_DIR)uart_linux.c $(INC_DIR)uart_linux.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(SRC_DIR)uart_linux.c $(OFLAGS) $(OUT_DIR)uart_linux.o

$(OUT_DIR)gsm_fibocomG510.o: $(SRC_DIR)gsm_fibocomG510.c $(INC_DIR)gsm_fibocomG510.h 
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(HW_CFG) $(SRC_DIR)gsm_fibocomG510.c $(OFLAGS) $(OUT_DIR)gsm_fibocomG510.o    

$(OUT_DIR)bcm2835.o: $(SRC_DIR)bcm2835.c $(INC_DIR)bcm2835.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(SRC_DIR)bcm2835.c $(OFLAGS) $(OUT_DIR)bcm2835.o 

# dummy object files. Used by sw to emulate some of the hw peripherals
$(OUT_DIR)gsm_dummy.o: $(DUMMY_SRC_DIR)gsm_dummy.c $(DUMMY_INC_DIR)gsm_dummy.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(DUMMY_SRC_DIR)gsm_dummy.c $(OFLAGS) $(OUT_DIR)gsm_dummy.o 

$(OUT_DIR)acc_dummy.o: $(DUMMY_SRC_DIR)acc_dummy.c $(DUMMY_INC_DIR)acc_dummy.h
	@echo "\nBuilding $@\n"
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(DUMMY_SRC_DIR)acc_dummy.c $(OFLAGS) $(OUT_DIR)acc_dummy.o 



