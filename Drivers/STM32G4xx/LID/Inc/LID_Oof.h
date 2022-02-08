#ifndef LID_ERROR_H
#define LID_ERROR_H

typedef enum {
	LID_OOF_SYSCLK = (1<<0),
	LID_OOF_CLOCK = (1<<1),
	LID_OOF_GPIO = (1<<2),
	LID_OOF_UART = (1<<3),
	LID_OOF_CAN = (1<<4),
	LID_NOICE = 0x00,
} LID_Oof_t;

extern void LID_OofHandler(LID_Oof_t err, char *msg);

#endif /*LID_ERROR_H*/
