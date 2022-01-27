#include "LID.h"
#include <stdio.h>
#include <string.h>

LID_GPIO_t PYRO0_CTRL_PINS[3] = {
		{.port = GPIOA, .pin = 9},
		{.port = GPIOC, .pin = 9},
		{.port = GPIOC, .pin = 7},
};

LID_GPIO_Group_t PYRO0_CTRL_GRP = {
	.gpios = PYRO0_CTRL_PINS,
	.n = 3
};

LID_GPIO_t LED_PINS[2] = {
	{.port = GPIOD, .pin=2},
	{.port = GPIOD, .pin=1}
};

LID_GPIO_Group_t LED_GRP = {
	.gpios = LED_PINS,
	.n = 2
};

int main(void) {
	LID_Init();

	int ret;
	LID_GPIO_GroupInit(&LED_GRP, LID_GPIO_TYPE_OPP);
	LID_GPIO_GroupInit(&PYRO0_CTRL_GRP, LID_GPIO_TYPE_OPP);

	LID_GPIO_Write(&PYRO0_CTRL_PINS[0], LID_GPIO_VALUE_L);
	LID_GPIO_Write(&PYRO0_CTRL_PINS[1], LID_GPIO_VALUE_H);
	LID_GPIO_Write(&PYRO0_CTRL_PINS[2], LID_GPIO_VALUE_H);

	LID_GPIO_Write(&LED_PINS[0], LID_GPIO_VALUE_L);
	LID_GPIO_Write(&LED_PINS[1], LID_GPIO_VALUE_H);

	LID_ADC_Channel_t PyroSenseChannel =
	{ 0 };
	PyroSenseChannel.ADCx = ADC1;
	PyroSenseChannel.channelId = LID_ADC_CHANNEL_12;
	LID_ADC_Data_t * pyroVptr = LID_ADC_SubscribeChannel(&PyroSenseChannel, LID_ADC_INTYPE_REGULAR);

	char kartoffelBuffer[16];

	while(1) {
		sprintf(kartoffelBuffer,"%d",*pyroVptr);
		LID_UART_Write(kartoffelBuffer,strlen(kartoffelBuffer));
		int i;
		for(i = 0; i < 1000000; i++);
	}

	return ret;
}

void Error_Handler(void) {
}
