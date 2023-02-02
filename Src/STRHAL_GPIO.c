#include "../Inc/STRHAL_GPIO.h"
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <STRHAL_GPIO.h>

void STRHAL_GPIO_Init()
{
	/* GPIO Ports Clock Enable */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
}

// requires the type parameter and the gpio type to be the same TODO: find a better way to enforce that!
void STRHAL_GPIO_SingleInit(const STRHAL_GPIO_t *gpio, STRHAL_GPIO_Type_t type)
{
	if (gpio->pin > 0x1F)
		return;

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	LL_GPIO_ResetOutputPin(gpio->port, (1 << gpio->pin));

	GPIO_InitStruct.Pin = (1 << gpio->pin);
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	switch (type)
	{
		case STRHAL_GPIO_TYPE_OPP:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
			break;

		case STRHAL_GPIO_TYPE_OOD:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
			GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
			break;

		case STRHAL_GPIO_TYPE_IHZ:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
			break;

		case STRHAL_GPIO_TYPE_IPU:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
			break;

		case STRHAL_GPIO_TYPE_IPD:
			GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
			break;
	}
	LL_GPIO_Init(gpio->port, &GPIO_InitStruct);
	//gpio->type = type;
}

void STRHAL_GPIO_GroupInit(STRHAL_GPIO_Group_t *gpios, STRHAL_GPIO_Type_t type)
{
	uint8_t i;
	for (i = 0; i < gpios->n; ++i)
	{
		STRHAL_GPIO_SingleInit(&gpios->gpios[i], type);
	}
}

inline void STRHAL_GPIO_Write(const STRHAL_GPIO_t *gpio, STRHAL_GPIO_Value_t value)
{
	if (gpio->pin > 0x1F)
		return;

	if (value == STRHAL_GPIO_VALUE_H)
	{
		LL_GPIO_SetOutputPin(gpio->port, (1 << gpio->pin));
	}
	else
	{
		LL_GPIO_ResetOutputPin(gpio->port, (1 << gpio->pin));
	}

}
void STRHAL_GPIO_GroupWrite(const STRHAL_GPIO_Group_t *gpios, STRHAL_GPIO_Value_t value)
{
	uint8_t i;
	for (i = 0; i < gpios->n; ++i)
		STRHAL_GPIO_Write(&gpios->gpios[i], value);
}

STRHAL_GPIO_Value_t STRHAL_GPIO_Read(const STRHAL_GPIO_t *gpio)
{
	if (gpio->pin > 0x1F)
		return STRHAL_GPIO_VALUE_L;

	return (LL_GPIO_ReadInputPort(gpio->port) & (1 << gpio->pin)) ? 1 : 0;
}

STRHAL_GPIO_Value_t STRHAL_GPIO_ReadOutput(const STRHAL_GPIO_t *gpio)
{
	if (gpio->pin > 0x1F)
		return STRHAL_GPIO_VALUE_L;

	return (LL_GPIO_ReadOutputPort(gpio->port) & (1 << gpio->pin)) ? 1 : 0;
}

void STRHAL_GPIO_GroupRead(const STRHAL_GPIO_Group_t *gpios, STRHAL_GPIO_Value_t *values)
{
	uint8_t i;
	for (i = 0; i < gpios->n; ++i)
		values[i] = STRHAL_GPIO_Read(&gpios->gpios[i]);
}
