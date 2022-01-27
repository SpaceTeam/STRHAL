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

void LID_CircularBuffer_Init(LID_CircularBuffer_t *buf, uint8_t *mem, uint32_t size) {
	buf->data = mem;
	buf->size = size;
	buf->t = buf->n = 0;
	buf->state = LID_BUFFER_STATE_0;
}

inline uint32_t LID_CircularBuffer_VWrite(LID_CircularBuffer_t *buf, uint32_t n) {
	if(n > (buf->size - buf->n))
		return 0;

	buf->n += n;
	return n;
}

inline uint32_t LID_CircularBuffer_VRead(LID_CircularBuffer_t *buf, uint32_t n) {
	if(n > buf->n)
		return 0;

	buf->t += n;
	buf->t %= buf->size;

	return n;
}

inline uint32_t LID_CircularBuffer_Write(LID_CircularBuffer_t *buf, const uint8_t *data, uint32_t n) {
	if(n > (buf->size - buf->n))
		return 0;

	uint32_t i;
	for(i = buf->t + buf->n; i < n; ++i, ++data)
		buf->data[i%n] = *data;

	buf->n += n;
	return n;
}

inline uint32_t LID_CircularBuffer_Read(LID_CircularBuffer_t *buf, uint8_t *data, uint32_t n) {
	if(n > buf->n)
		return 0;

	uint32_t i;
	for(i = buf->t; i < n; ++i, ++data)
		*data = buf->data[i%n];

	return n;
}

inline uint32_t LID_CircularBuffer_GetFreeSpace(LID_CircularBuffer_t *buf) {
	return buf->size - buf->n;
}
inline void LID_CircularBuffer_Flush(LID_CircularBuffer_t *buf) {
	buf->t = buf->n = 0;
}
