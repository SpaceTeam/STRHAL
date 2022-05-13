#include <STRHAL_Container.h>

void STRHAL_LinearBuffer_Init(STRHAL_LinearBuffer_t *buf, uint8_t *mem, uint32_t size) {
	buf->state = STRHAL_BUFFER_STATE_0;
	buf->data = (uint8_t *) mem;
	buf->size = size;
	buf->n = 0;
}

uint32_t STRHAL_LinearBuffer_Write(STRHAL_LinearBuffer_t *buf, uint8_t *data, uint32_t n) {
	if(buf->state != STRHAL_BUFFER_STATE_0 || n > buf->size)
		return 0;

	buf->state = STRHAL_BUFFER_STATE_W;
	uint32_t i;

	for(i = 0; i < n; ++i)
		buf->data[i] = data[i];

	buf->state = STRHAL_BUFFER_STATE_0;
	buf->n = n;
	return n;
}

uint32_t STRHAL_LinearBuffer_Read(STRHAL_LinearBuffer_t *buf, uint8_t *data, uint32_t n) {
	if(buf->state != STRHAL_BUFFER_STATE_0 || n > buf->size)
		return 0;

	if(n > buf->n)
		n = buf->n;

	buf->state = STRHAL_BUFFER_STATE_R;
	uint32_t i;
	for(i = 0; i < n; ++i)
		buf->data[i] = data[i];


	buf->state = STRHAL_BUFFER_STATE_0;
	return n;
}

void STRHAL_CircularBuffer_Init(STRHAL_CircularBuffer_t *buf, uint8_t *mem, uint32_t size) {
	buf->data = mem;
	buf->size = size;
	buf->t = buf->n = 0;
	buf->state = STRHAL_BUFFER_STATE_0;
}

inline uint32_t STRHAL_CircularBuffer_VWrite(STRHAL_CircularBuffer_t *buf, uint32_t n) {
	if(n > (buf->size - buf->n))
		return 0;

	buf->n += n;
	return n;
}

inline uint32_t STRHAL_CircularBuffer_VRead(STRHAL_CircularBuffer_t *buf, uint32_t n) {
	if(n > buf->n)
		return 0;

	buf->t += n;
	buf->t %= buf->size;

	return n;
}

inline uint32_t STRHAL_CircularBuffer_Write(STRHAL_CircularBuffer_t *buf, const uint8_t *data, uint32_t n) {
	if(n > (buf->size - buf->n))
		return 0;

	uint32_t i;
	for(i = buf->t + buf->n; i < n; ++i, ++data)
		buf->data[i%n] = *data;

	buf->n += n;
	return n;
}

inline uint32_t STRHAL_CircularBuffer_Read(STRHAL_CircularBuffer_t *buf, uint8_t *data, uint32_t n) {
	if(n > buf->n)
		return 0;

	uint32_t i;
	for(i = buf->t; i < n; ++i, ++data)
		*data = buf->data[i%n];

	return n;
}

inline uint32_t STRHAL_CircularBuffer_GetFreeSpace(STRHAL_CircularBuffer_t *buf) {
	return buf->size - buf->n;
}
inline void STRHAL_CircularBuffer_Flush(STRHAL_CircularBuffer_t *buf) {
	buf->t = buf->n = 0;
}
