#ifndef BLMB_UI_H_
#define BLMB_UI_H_

#include "main.h"
#define BLMB_CW_Button_GPIO_Port 	GPIOC
#define BLMB_CW_Button_Pin			LL_GPIO_PIN_8

#define BLMB_Sel_Button_GPIO_Port 	GPIOC
#define BLMB_Sel_Button_Pin			LL_GPIO_PIN_9

#define BLMB_CCW_Button_GPIO_Port 	GPIOD
#define BLMB_CCW_Button_Pin			LL_GPIO_PIN_15

#define BLMB_UI_STEP_SIZE 50
#define COOLDOWN_TICKS (100)
#define LONGPRESS_TICKS (2000)

typedef enum
{
	BLMB_UI_MODE_NORMAL = 0,
	BLMB_UI_MODE_CALIBRATE_OPEN,
	BLMB_UI_MODE_CALIBRATE_CLOSE,

	BLMB_UI_LAST_MODE

} BlmbUi_Mode_t;

typedef enum
{
	BLMB_UI_SPEED_SLOW = 0,
	BLMB_UI_SPEED_MEDIUM,
	BLMB_UI_SPEED_FAST,
	BLMB_UI_LAST_SPEED
} BlmbUi_Speed_t;

typedef enum
{
	BLMB_UI_IDLE,
	BLMB_UI_PRESSED,
	BLMB_UI_PRESSED_IDLE,
	BLMB_UI_LONG_PRESSED,
	BLMB_UI_RELEASED
} BlmbUi_Button_State_t;

void BlmbUi_InitEXTI(void);
Result_t BlmbUi_CheckInput(uint16_t *return_var);



#endif
