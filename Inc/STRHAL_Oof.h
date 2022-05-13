#ifndef STRHAL_ERROR_H
#define STRHAL_ERROR_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	STRHAL_OOF_SYSCLK = (1<<0),
	STRHAL_OOF_CLOCK = (1<<1),
	STRHAL_OOF_GPIO = (1<<2),
	STRHAL_OOF_UART = (1<<3),
	STRHAL_OOF_CAN = (1<<4),
	STRHAL_NOICE = 0x00,
} STRHAL_Oof_t;

extern void STRHAL_OofHandler(STRHAL_Oof_t err, char *msg);


#ifdef __cplusplus
}
#endif

#endif /*STRHAL_ERROR_H*/
