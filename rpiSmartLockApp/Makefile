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
SRC_DIR       = $(PRJ_DIR)/src/
INC_DIR       = $(PRJ_DIR)/include/
OUT_DIR       = $(PRJ_DIR)/output/

# ------------------------------[ Build Options ]------------------------------ #

CC      = gcc
DEBUG   = -g
CFLAGS  = -c -Wall -I$(INC_DIR)
INCFLAG = -I$(INC_DIR)
OFLAGS  = -o
BUILD_CMD_VEROSITY = @ 

# objects built regardless of the hw system configuration:
# main application object
SRCS += rpiSmartLockApp.c

# hardware abstraction layer objects
SRCS += hal.c
SRCS += gpioHal.c
SRCS += uartHal.c
SRCS += gsmHal.c
SRCS += accHal.c  

#TODO: find a way to construct all the conditional stuff below automatically

# platform dependent build flags
ifneq ($(PLATFORM),UNDEFINED )
 ifeq ($(PLATFORM),RPI)
  HW_CFG += -DPLATFORM_RPI
  SRCS += uart_linux.c
  SRCS += bcm2835.c
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
  SRCS += gsm_fibocomG510.c
 else ifeq ($(GSM_MODULE),DUMMY)
  HW_CFG += -DGSM_MODULE_DUMMY
  SRCS += gsm_dummy.c
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
  SRCS += acc_dummy.c
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

# ---------------------[ DO NOT EDIT BELOW THIS LINE ]------------------------- #

# VPATH variable tells make where to look for prerequisite files
# VPATH = $(SRC_DIR)

# this is just a more fancy way of doing what the above VPATH was doing.
# Although it could be useful for the case where there are different source
# directories for each platform the project may run on.
vpath %.c $(SRC_DIR)

#TODO: should not forget about the .d dependency files

# create an object file for each source file
OBJS = $(patsubst %.c, %.o, $(SRCS))

# append prefix $(OUT_DIR) to all object files
OBJS := $(addprefix $(OUT_DIR), $(OBJS))

# append prefix $(SRC_DIR) to all source files
#SRCS := $(addprefix $(SRC_DIR), $(SRCS))

# ------------------------------[ Targets ] ----------------------------------- #

# build application. It is build by linking all the object files in $(OBJS)
#all: $(OUT_DIR)$(DEPENDENCIES) $(OBJS)
all: $(OBJS)
	@echo "=========================================================="
	@echo "========== Building $(EXEC_NAME) application =========="
	@echo "Following object files will be build and linked:\n"
	@$(foreach obj,$(OBJS),echo $(obj);)
	@echo ""
	$(BUILD_CMD_VEROSITY) $(CC) $(OBJS) $(OFLAGS) $(OUT_DIR)$(EXEC_NAME)
	@echo "$(EXEC_NAME) application built sucessfully"
	@echo "=========================================================="

# build object files. For each %.c source file in $(SRCS), an %.o object is created; 
$(OBJS): %.o: %.c
#$(OBJS): %.o: $(SRCS): %.c
	@echo "\n"
	@echo "Building $@ object. Source file is $<"
	@echo "====================================================================="
	$(BUILD_CMD_VEROSITY) $(CC) $(CFLAGS) $(HW_CFG) $< $(OFLAGS) $(OUT_DIR)$@

echoSRCS:
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "VPATH: $(VPATH)"

# make clean; will delete everytinhg in output/ directory
clean:
	@echo "Removing all files from $(OUT_DIR)" 
	rm -f $(OUT_DIR)*
	@echo "make clean ... Done"
