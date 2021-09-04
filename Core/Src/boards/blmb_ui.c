#include "blmb_ui.h"
#include "main.h"
#include "systick.h"
#include "serial.h"

void BlmbUi_InitEXTI(void)
{
	LL_EXTI_InitTypeDef EXTI_InitStruct =
	{ 0 };

	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE8);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE9);
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTD, LL_SYSCFG_EXTI_LINE15);

	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_8 | LL_EXTI_LINE_9 | LL_EXTI_LINE_15;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.LineCommand = ENABLE;
	LL_EXTI_Init(&EXTI_InitStruct);

	//enable exti interrupts
	NVIC_SetPriority(EXTI9_5_IRQn, 3);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 3);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static volatile uint32_t cw_button_old_tick = 0;
static volatile uint32_t select_button_old_tick = 0;
static volatile uint32_t ccw_button_old_tick = 0;

static volatile BlmbUi_Mode_t mode = BLMB_UI_MODE_NORMAL;
static volatile BlmbUi_Speed_t speed = BLMB_UI_SPEED_MEDIUM;
static volatile BlmbUi_Button_State_t ccw_button_state = BLMB_UI_IDLE;
static volatile BlmbUi_Button_State_t select_button_state = BLMB_UI_IDLE;
static volatile BlmbUi_Button_State_t cw_button_state = BLMB_UI_IDLE;

//CW BUTTON ISR
//SELECT BUTTON ISR
void EXTI9_5_IRQHandler(void)
{
	uint32_t tick = Systick_GetTick();

	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8))
	{
		if ((tick - cw_button_old_tick) > COOLDOWN_TICKS)
		{
			cw_button_state = (LL_GPIO_IsInputPinSet(BLMB_CW_Button_GPIO_Port, BLMB_CW_Button_Pin) == RESET) ? BLMB_UI_PRESSED : BLMB_UI_RELEASED;
			cw_button_old_tick = tick;
		}
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);
	}

	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9))
	{
		if ((tick - select_button_old_tick) > COOLDOWN_TICKS)
		{
			select_button_state = (LL_GPIO_IsInputPinSet(BLMB_Sel_Button_GPIO_Port, BLMB_Sel_Button_Pin) == RESET) ? BLMB_UI_PRESSED : BLMB_UI_RELEASED;
			select_button_old_tick = tick;
		}
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);

	}
}
void EXTI15_10_IRQHandler(void)
{

	uint32_t tick = Systick_GetTick();

	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_15))
	{
		if ((tick - ccw_button_old_tick) > COOLDOWN_TICKS)
		{
			ccw_button_state = (LL_GPIO_IsInputPinSet(BLMB_CCW_Button_GPIO_Port, BLMB_CCW_Button_Pin) == RESET) ? BLMB_UI_PRESSED : BLMB_UI_RELEASED;
			ccw_button_old_tick = tick;
		}
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_15);
	}

}
static Result_t BlmbUi_ProcessInput(uint16_t *return_var)
{


}


Result_t BlmbUi_CheckInput(uint16_t *return_var)
{
	uint32_t tick = Systick_GetTick(); //TODO Create struct for button and loop
	if ((cw_button_state == BLMB_UI_PRESSED || cw_button_state == BLMB_UI_PRESSED_IDLE) && (tick - cw_button_old_tick) > LONGPRESS_TICKS)
		cw_button_state = BLMB_UI_LONG_PRESSED;
	if ((select_button_state == BLMB_UI_PRESSED || select_button_state == BLMB_UI_PRESSED_IDLE) && (tick - select_button_old_tick) > LONGPRESS_TICKS)
		select_button_state = BLMB_UI_LONG_PRESSED;
	if ((ccw_button_state == BLMB_UI_PRESSED || ccw_button_state == BLMB_UI_PRESSED_IDLE) && (tick - ccw_button_old_tick) > LONGPRESS_TICKS)
		ccw_button_state = BLMB_UI_LONG_PRESSED;

	if ((cw_button_state != BLMB_UI_IDLE && cw_button_state != BLMB_UI_PRESSED_IDLE) || (select_button_state != BLMB_UI_IDLE && select_button_state != BLMB_UI_PRESSED_IDLE) || (ccw_button_state != BLMB_UI_IDLE && ccw_button_state != BLMB_UI_PRESSED_IDLE))
	{

		Result_t result = BlmbUi_ProcessInput(return_var);

		cw_button_state = (cw_button_state == BLMB_UI_PRESSED) ? BLMB_UI_PRESSED_IDLE : cw_button_state;
		select_button_state = (select_button_state == BLMB_UI_PRESSED) ? BLMB_UI_PRESSED_IDLE : select_button_state;
		ccw_button_state = (ccw_button_state == BLMB_UI_PRESSED) ? BLMB_UI_PRESSED_IDLE : ccw_button_state;


		cw_button_state = (cw_button_state == BLMB_UI_LONG_PRESSED) ? BLMB_UI_IDLE : cw_button_state;
		select_button_state = (select_button_state == BLMB_UI_LONG_PRESSED) ? BLMB_UI_IDLE : select_button_state;
		ccw_button_state = (ccw_button_state == BLMB_UI_LONG_PRESSED) ? BLMB_UI_IDLE : ccw_button_state;

		cw_button_state = (cw_button_state == BLMB_UI_RELEASED) ? BLMB_UI_IDLE : cw_button_state;
		select_button_state = (select_button_state == BLMB_UI_RELEASED) ? BLMB_UI_IDLE : select_button_state;
		ccw_button_state = (ccw_button_state == BLMB_UI_RELEASED) ? BLMB_UI_IDLE : ccw_button_state;
		return result;
	}
	return OOF_NO_NEW_DATA;
}
