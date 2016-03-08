// Name:        uart_linux.h
// Description: 
//
// Author: Calin Pantea
//
// http://mirror.datenwolf.net/serial/

// ###########################################################
// Includes 
// ###########################################################
//#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>   /* for printf */

// ###########################################################
// Defines 
// ###########################################################

#define UART_DGB_PRINT_MSG_ENABLE

#ifdef UART_DGB_PRINT_MSG_ENABLE
 #define UART_DGB_PRINT_MSG printf
#else
 #define UART_DGB_PRINT_MSG
#endif

#define CARRIAGE_RETURN 0x0D
#define LINE_FEED       0x0A

/* modem lines */
#define TIOCM_LE        0x001
#define TIOCM_DTR       0x002
#define TIOCM_RTS       0x004
#define TIOCM_ST        0x008
#define TIOCM_SR        0x010
#define TIOCM_CTS       0x020
#define TIOCM_CAR       0x040
#define TIOCM_RNG       0x080
#define TIOCM_DSR       0x100
#define TIOCM_CD        TIOCM_CAR
#define TIOCM_RI        TIOCM_RNG
#define TIOCM_OUT1      0x2000
#define TIOCM_OUT2      0x4000
#define TIOCM_LOOP      0x8000

#define SET_CTS   1
#define CLEAR_CTS 0

// ###########################################################
// Global Definitions 
// ###########################################################

typedef struct
{
   char* string;
   int val;
} t_symstruct_baudRate;

typedef struct
{
	char* string;
	int val;
} t_symstruct_dataLength;

typedef struct {
 char* BaudRate;
 char* DataLength;
 char* Parity;
 char* StopBit;
 char* FlowCtrl;
} ty_serialPortConfig;

// ###########################################################
// Function Prototypes 
// ###########################################################

// global functions 
// -----------------------------------------------------------
int  uart_writeData(int fd, void* writeData, int bytes);
int  uart_readData(int fd, void* readData, int bytes);
int  uart_getInputBytesAvailable(int fd);
void uart_flushInOutBuffers(int fd);
void uart_waitRTS(int fd);
void uart_setCTS(int fd, int set_nClear);
void uart_readUartInputData(int fd, char* readBuf);
void uart_readUartInputBuffer(int fd, char* readBuf);
int  uart_setupSerialPortParameters(char* serialPortName, ty_serialPortConfig* serialPortCfg);
