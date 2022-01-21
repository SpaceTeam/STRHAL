#ifndef LID_CLOCK_H_
#define LID_CLOCK_H_

typedef enum {
	LID_CLOCK_GOOD = 0,
	LID_CLOCK_ERR_HSE = -1
} LID_Clock_Status_t;

LID_Clock_Status_t LID_Clock_Init();


#endif /* LID_CLOCK_H_ */
