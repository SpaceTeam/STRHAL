#ifndef LID_CONTAINER_H
#define LID_CONTAINER_H

#include <stdint.h>

typedef enum {
	LID_BUFFER_STATE_0 = 0,
	LID_BUFFER_STATE_W = (1<<0),
	LID_BUFFER_STATE_R = (1<<1)
} LID_Buffer_State_t;

typedef struct {
	uint8_t *data;
	LID_Buffer_State_t state;
	uint32_t size;
	uint32_t n;
} LID_LinearBuffer_t;

typedef struct {
	uint8_t *data;
	LID_Buffer_State_t state;
	uint32_t size;
	uint32_t t;
	uint32_t n;
} LID_CircularBuffer_t;

void LID_LinearBuffer_Init(LID_LinearBuffer_t *buf, uint8_t *mem, uint32_t size);
uint32_t LID_LinearBuffer_Write(LID_LinearBuffer_t *buf, uint8_t *data, uint32_t n);
uint32_t LID_LinearBuffer_Read(LID_LinearBuffer_t *buf, uint8_t *data, uint32_t n);

void LID_CircularBuffer_Init(LID_CircularBuffer_t *buf, uint8_t *mem, uint32_t size);
uint32_t LID_CircularBuffer_VWrite(LID_CircularBuffer_t *buf, uint32_t n);
uint32_t LID_CircularBuffer_VRead(LID_CircularBuffer_t *buf, uint32_t n);
uint32_t LID_CircularBuffer_Write(LID_CircularBuffer_t *buf, const uint8_t *data, uint32_t n);
uint32_t LID_CircularBuffer_Read(LID_CircularBuffer_t *buf, uint8_t *data, uint32_t n);
uint32_t LID_CircularBuffer_GetFreeSpace(LID_CircularBuffer_t *buf);
void LID_CircularBuffer_Flush(LID_CircularBuffer_t *buf);

#endif /*LID_CONTAINER_H*/
