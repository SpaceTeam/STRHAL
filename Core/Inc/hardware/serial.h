#ifndef SERIAL_H__
#define SERIAL_H__

#include <stdint.h>

#define SERIAL_DELIMITER	13
#define LINE_FEED			10
#define RECEIVE_STRING_SIZE 64


void Serial_Init(void);
void Serial_Print(char _out[]);
void Serial_Println(char _out[]);
uint8_t Serial_CheckInput(char *message);

#endif
