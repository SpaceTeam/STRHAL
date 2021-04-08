#ifndef SERIAL_H__
#define SERIAL_H__

#include <stdint.h>

#define SERIAL_DELIMITER	13
#define LINE_FEED			10
#define RECEIVE_STRING_SIZE 64


void Serial_Init(void);

void Serial_PutString(char *str);
void Serial_PutInt( uint32_t val);
void Serial_PutHex(uint32_t val);
void Serial_PrintString(char *str);
void Serial_PrintInt( uint32_t val);
void Serial_PrintHex(uint32_t val);

uint8_t Serial_CheckInput(char *message);

#endif
