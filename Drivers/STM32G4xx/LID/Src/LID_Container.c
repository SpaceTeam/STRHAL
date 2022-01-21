#include "../Inc/LID_Container.h"

void LID_LinearBuffer_Init(LID_LinearBuffer_t *buf, uint8_t *mem, uint32_t size) {
	buf->state = LID_BUFFER_STATE_0;
	buf->data = (uint8_t *) mem;
	buf->size = size;
	buf->n = 0;
}

uint32_t LID_LinearBuffer_Write(LID_LinearBuffer_t *buf, uint8_t *data, uint32_t n) {
	if(buf->state != LID_BUFFER_STATE_0 || n > buf->size)
		return 0;

	buf->state = LID_BUFFER_STATE_W;
	uint32_t i;

	for(i = 0; i < n; ++i)
		buf->data[i] = data[i];

	buf->state = LID_BUFFER_STATE_0;
	buf->n = n;
	return n;
}

uint32_t LID_LinearBuffer_Read(LID_LinearBuffer_t *buf, uint8_t *data, uint32_t n) {
	if(buf->state != LID_BUFFER_STATE_0 || n > buf->size)
		return 0;

	if(n > buf->n)
		n = buf->n;

	buf->state = LID_BUFFER_STATE_R;
	uint32_t i;
	for(i = 0; i < n; ++i)
		buf->data[i] = data[i];


	buf->state = LID_BUFFER_STATE_0;
	return n;
}

LID_Buffer_State_t LID_LinearBuffer_GetState(const LID_LinearBuffer_t *buf) {
	return buf->state;
}
