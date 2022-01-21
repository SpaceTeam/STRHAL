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

void LID_LinearBuffer_Init(LID_LinearBuffer_t *buf, uint8_t *mem, uint32_t size);

uint32_t LID_LinearBuffer_Write(LID_LinearBuffer_t *buf, uint8_t *data, uint32_t n);
uint32_t LID_LinearBuffer_Read(LID_LinearBuffer_t *buf, uint8_t *data, uint32_t n);
LID_Buffer_State_t LID_LinearBuffer_GetState(const LID_LinearBuffer_t *buf);

#endif /*LID_CONTAINER_H*/
