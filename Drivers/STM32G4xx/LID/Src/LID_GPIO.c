#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>

#include "../Inc/LID_GPIO.h"

void LID_GPIO_Init() {
  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
}

void LID_GPIO_SingleInit(LID_GPIO_t *gpio, LID_GPIO_Type_t type) {
	if(gpio->pin > 0x1F)
		return;

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_GPIO_ResetOutputPin(gpio->port, (1<<gpio->pin));

	GPIO_InitStruct.Pin = (1<<gpio->pin);
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	switch(type) {
		case LID_GPIO_TYPE_OPP:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			break;

		case LID_GPIO_TYPE_OOD:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			break;

		case LID_GPIO_TYPE_IHZ:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
			break;


		case LID_GPIO_TYPE_IPU:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
			break;

		case LID_GPIO_TYPE_IPD:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
			break;
	}
	LL_GPIO_Init(gpio->port, &GPIO_InitStruct);
	gpio->type = type;
}

void LID_GPIO_GroupInit(LID_GPIO_Group_t *gpios, LID_GPIO_Type_t type) {
	uint8_t i;
	for(i = 0; i < gpios->n; ++i) {
		LID_GPIO_SingleInit(&gpios->gpios[i], type);
	}
}

inline void LID_GPIO_Write(const LID_GPIO_t *gpio, LID_GPIO_Value_t value) {
	if(gpio->pin > 0x1F)
		return;

	if(value == LID_GPIO_VALUE_H) {
		LL_GPIO_SetOutputPin(gpio->port, (1<<gpio->pin));
	} else {
		LL_GPIO_ResetOutputPin(gpio->port, (1<<gpio->pin));
	}

}
void LID_GPIO_GroupWrite(const LID_GPIO_Group_t *gpios, LID_GPIO_Value_t value) {
	uint8_t i;
	for(i = 0; i < gpios->n; ++i)
		LID_GPIO_Write(&gpios->gpios[i], value);
}


LID_GPIO_Value_t LID_GPIO_Read(const LID_GPIO_t *gpio) {
	if(gpio->pin > 0x1F)
		return LID_GPIO_VALUE_L;

	return (LL_GPIO_ReadInputPort(gpio->port) & (1<<gpio->pin)) ? 1 : 0;
}

void LID_GPIO_GroupRead(const LID_GPIO_Group_t *gpios, LID_GPIO_Value_t *values) {
	uint8_t i;
	for(i = 0; i < gpios->n; ++i)
		values[i] = LID_GPIO_Read(&gpios->gpios[i]);
}
