// Name:        uart_linux.c
// Description: 
//
// Author: Calin Pantea
//
// http://mirror.datenwolf.net/serial/

// ###########################################################
// Includes
// ###########################################################
#include <uart_linux.h>

// ###########################################################
// Global Definitions 
// ###########################################################

int          serialPortFD;
volatile int HardwareFlowControlEnabled = 0;

struct termios options;

// default UART port configuration
ty_serialPortConfig serialPortCfg_Default = {.BaudRate   = "B009600",
                                             .DataLength = "DataBits8",
                                             .Parity 	 = "ParityNone",
                                             .StopBit 	 = "StopBit1",
                                             .FlowCtrl   = "NO"};

static t_symstruct_baudRate baudRates_table[] = {
		{.string = "B000050",.val = B50  },  {.string = "B000075",.val = B75   }, {.string = "B000110",.val = B110  }, {.string = "B000134",.val = B134},
		{.string = "B000150",.val = B150 },  {.string = "B000200",.val = B200  }, {.string = "B000300",.val = B300  }, {.string = "B000600",.val = B600},
		{.string = "B001200",.val = B1200},  {.string = "B001800",.val = B1800 }, {.string = "B002400",.val = B2400 }, {.string = "B004800",.val = B4800},
		{.string = "B009600",.val = B9600},  {.string = "B019200",.val = B19200}, {.string = "B038400",.val = B38400}, {.string = "B057600",.val = B57600},
		{.string = "B115200",.val = B115200},{.string = "B230400",.val = B230400},{.string = "B460800",.val = B460800},{.string = "STOP",   .val = 0}};

static t_symstruct_dataLength dataLength_table[] = {
		{.string = "DataBits5",.val = CS5}, 
 	        {.string = "DataBits6",.val = CS6}, 
                {.string = "DataBits7",.val = CS7}, 
                {.string = "DataBits8",.val = CS8}, 
                {.string = "STOP",.val = 0} };

// ###########################################################
// Function Definition
// ###########################################################

//int open_port(char* portName)
static int uart_open_port(char* portName)
{
  int fd;

  //fd = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
  fd = open(portName, O_RDWR | O_NOCTTY);
  if (fd == -1)
    UART_DGB_PRINT_MSG("open_port: Unable to open %s \n", portName);
  else
    fcntl(fd, F_SETFL, 0);

  return (fd);
}

int uart_writeData(int fd, void* writeData, int bytes)
{
 int n;
 n = write(fd, writeData, bytes);
 if(n < 0)
	 UART_DGB_PRINT_MSG("writeData: Unable to write to serial port\n");

 return n;
}

int uart_readData(int fd, void* readData, int bytes)
{
  int n;
  n = read(fd, readData, bytes);
  if(n < 0)
	  UART_DGB_PRINT_MSG("readData: Unable to read from serial port\n");

  return n;
}

//int getBaudRateFromCmd(char* cmd)
static int uart_getBaudRateFromCmd(char* cmd)
{
 int i = 0;
 int found=0;
 int baudRate;
 char stop[] = "STOP";

 while( !(strcmp(baudRates_table[i].string, stop) == 0) ) {
	 if(strcmp(cmd, baudRates_table[i].string) == 0) {

		 baudRate = baudRates_table[i].val;
		 found = 1;
		 break;
	 }
	 i++;
 }

 if(!found) {
	 UART_DGB_PRINT_MSG("ERROR: getBaudRateFromCmd: Baud Rate parameter not valid!\n");
 }

 return baudRate;
}

//void setBaudRate(int fd, char* baudRate_cmd)
static void uart_setBaudRate(int fd, char* baudRate_cmd)
{
	int baudRate;

	baudRate = uart_getBaudRateFromCmd(baudRate_cmd);

	tcgetattr(fd, &options);
	cfsetispeed(&options, baudRate);
	cfsetospeed(&options, baudRate);
	options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(fd, TCSANOW, &options);
}

//int getDataLengthFromCmd(char* cmd)
static int uart_getDataLengthFromCmd(char* cmd)
{
 int i=0;
 int found=0;
 int dataLength;
 char stop[] = "STOP";

 while( !(strcmp(dataLength_table[i].string, stop) == 0) ) {
	 if(strcmp(cmd, dataLength_table[i].string) == 0) {

		 dataLength = dataLength_table[i].val;
		 found = 1;
		 break;
	 }
	 i++;
 }

 if(!found) {
	 UART_DGB_PRINT_MSG("ERROR: getDataLengthFromCmd: Data Length parameter not valid!\n");
 }

 return dataLength;
}

//void setDataLength(int fd, char* dataLength_cmd)
static void uart_setDataLength(int fd, char* dataLength_cmd)
{
	int dataLength;

	dataLength = uart_getDataLengthFromCmd(dataLength_cmd);

	tcgetattr(fd, &options);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= dataLength;
	tcsetattr(fd, TCSANOW, &options);
}

//void setParityBit(int fd, char* parity)
static void uart_setParityBit(int fd, char* parity)
{
	char parityEven[] = "ParityEven";
	char parityOddd[] = "ParityOddd";
	char parityNone[] = "ParityNone";

	tcgetattr(fd, &options);

	if(strcmp(parity, parityEven) == 0) {

		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;

	} else if(strcmp(parity, parityOddd) == 0 ) {

		options.c_cflag |= PARENB;
		options.c_cflag |= PARODD;

	} else if(strcmp(parity, parityNone) == 0 ) {

		options.c_cflag &= ~PARENB;

	} else {
		UART_DGB_PRINT_MSG("ERROR: Parity parameter not valid\n");
	}

	tcsetattr(fd, TCSANOW, &options);
}

//void setStopBit(int fd, char* stopBit)
static void uart_setStopBit(int fd, char* stopBit)
{
	char StopBit1[] = "StopBit1";
	char StopBit2[] = "StopBit2";

	tcgetattr(fd, &options);

	if(strcmp(stopBit, StopBit1) == 0) {

		options.c_cflag &= ~CSTOPB;

	} else if(strcmp(stopBit, StopBit2) == 0 ) {

		options.c_cflag |= CSTOPB;

	} else {
		UART_DGB_PRINT_MSG("ERROR: StopBit parameter not valid\n");
	}

	tcsetattr(fd, TCSANOW, &options);
}

//void setFlowControl(int fd, char* flowControl)
static void uart_setFlowControl(int fd, char* flowControl)
{
  char flowControlHardware[] = "HW";
  int status;

  // get current configuration
  tcgetattr(fd, &options);

  if(strcmp(flowControl, flowControlHardware) == 0) {
	  // enable Hardware Flow Control
	  options.c_cflag |= CRTSCTS;
	  HardwareFlowControlEnabled = 1;

  } else {
	  // disable Hardware Flow Control
	  options.c_cflag &= ~CRTSCTS;
	  HardwareFlowControlEnabled = 0;
  }
  // apply changes
  tcsetattr(fd, TCSANOW, &options);
}

void uart_flushInOutBuffers(int fd)
{
	ioctl(fd, TCFLSH);
}

void uart_waitRTS(int fd)
{
  int status;

  // get Control Signals status
  ioctl(fd, TIOCMGET, &status);

  while((status & TIOCM_RTS) != TIOCM_RTS)
	  ioctl(fd, TIOCMGET, &status);
}

void uart_setCTS(int fd, int set_nClear)
{
  int status;

  ioctl(fd, TIOCMGET, &status);

  if(set_nClear)
	  status |= TIOCM_CTS;
  else
	  status &= ~TIOCM_CTS;

  ioctl(fd, TIOCMSET, status);
}

void uart_readUartInputData(int fd, char* readBuf)
{
  int i=0;
  int n;
  char recData = 0x00;

  while((recData != CARRIAGE_RETURN) && (recData != LINE_FEED)) {
	  n = uart_readData(fd, (void*)&recData, 1);
	  if(n > 0) {
		  readBuf[i] = recData;
		  i++;
	  }
  }
}

void uart_readUartInputBuffer(int fd, char* readBuf)
{
 int inputBytesAvailable;
 int n, i=0;
 char recData = 0x00;

 ioctl(fd, FIONREAD, &inputBytesAvailable);

 while(inputBytesAvailable > 0) {
	  n = uart_readData(fd, (void*)&recData, 1);
	  if(n > 0) {
		  readBuf[i] = recData;
		  i++;
	  }
	  ioctl(fd, FIONREAD, &inputBytesAvailable);
 }
}

int uart_setupSerialPortParameters(char* serialPortName, ty_serialPortConfig* serialPortCfg)
{
    char i;
    UART_DGB_PRINT_MSG("%s\n",__func__);

    UART_DGB_PRINT_MSG("port name: %s\n", serialPortName);

    UART_DGB_PRINT_MSG("\n");

    UART_DGB_PRINT_MSG("open serial port...\n");
    serialPortFD = uart_open_port(serialPortName);

    UART_DGB_PRINT_MSG("set baud rate: %s\n", serialPortCfg->BaudRate);
    uart_setBaudRate(serialPortFD, serialPortCfg->BaudRate);

    UART_DGB_PRINT_MSG("set parity bits: %s\n", serialPortCfg->Parity);
    uart_setParityBit(serialPortFD, serialPortCfg->Parity);

    UART_DGB_PRINT_MSG("set stop bits: %s\n", serialPortCfg->StopBit);
    uart_setStopBit(serialPortFD, serialPortCfg->StopBit);

    UART_DGB_PRINT_MSG("set data length: %s\n", serialPortCfg->DataLength);
    uart_setDataLength(serialPortFD, serialPortCfg->DataLength);

    UART_DGB_PRINT_MSG("set flow control: %s\n", serialPortCfg->FlowCtrl);
    uart_setFlowControl(serialPortFD, serialPortCfg->FlowCtrl);

   return serialPortFD;
}

