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

	LID_GPIO_Write(&LED_PINS[0], LID_GPIO_VALUE_H);
	LID_GPIO_Write(&LED_PINS[1], LID_GPIO_VALUE_H);

	LID_ADC_Run();

	LID_ADC_Channel_t PyroSenseChannel =
	{ 0 };
	PyroSenseChannel.ADCx = ADC1;
	PyroSenseChannel.channelId = LID_ADC_CHANNEL_12;

	LID_ADC_Channel_t Press2Channel =
	{ 0 };
	Press2Channel.ADCx = ADC1;
	Press2Channel.channelId = LID_ADC_CHANNEL_14;

	LID_ADC_Channel_t V12SenseChannel =
	{ 0 };
	V12SenseChannel.ADCx = ADC2;
	V12SenseChannel.channelId = LID_ADC_CHANNEL_3;

	LID_ADC_Channel_t V5SenseChannel =
	{ 0 };
	V5SenseChannel.ADCx = ADC2;
	V5SenseChannel.channelId = LID_ADC_CHANNEL_5;

	LID_ADC_Data_t * pyroVptr = LID_ADC_SubscribeChannel(&PyroSenseChannel, LID_ADC_INTYPE_REGULAR);
	LID_ADC_Data_t * press2ptr = LID_ADC_SubscribeChannel(&Press2Channel, LID_ADC_INTYPE_REGULAR);
	LID_ADC_Data_t * V12ptr = LID_ADC_SubscribeChannel(&V12SenseChannel, LID_ADC_INTYPE_REGULAR);
	LID_ADC_Data_t * V5ptr = LID_ADC_SubscribeChannel(&V5SenseChannel, LID_ADC_INTYPE_REGULAR);

	if(pyroVptr == NULL || V12ptr == NULL || V5ptr == NULL) {
		char buf[] = {'O','O','F'};
		LID_UART_Write(buf,3);
		return 0;
	}

	char kartoffelBuffer[128];

	while(1) {
		sprintf(kartoffelBuffer,"%d, %d, %d, %d, %lu\n",*pyroVptr,*press2ptr,*V12ptr,*V12ptr,LL_ADC_IsActiveFlag_OVR(ADC1));
		//sprintf(kartoffelBuffer,"%lu, %lu, %lu\n",ADC1->DR,ADC2->DR,LL_ADC_IsActiveFlag_OVR(ADC1));
		//sprintf(kartoffelBuffer,"%lu\n",DMA1->ISR);
		LID_UART_Write(kartoffelBuffer,strlen(kartoffelBuffer));
		int i;
		for(i = 0; i < 1000000; i++);
	}

	return ret;
}

void Error_Handler(void) {
}
