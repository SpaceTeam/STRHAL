#include "main.h"
#include "git_version.h"
#include "ADS131.h"
#include "cmds.h"
#include "can.h"
#include "gpio.h"
#include "sysclock.h"
#include "systick.h"
#include "serial.h"
#include "usart.h"
#include "adc.h"
#include "speaker.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UART_UPDATE_TIME 100

uint64_t tick = 0;
int main(void)
{

	uint64_t uart_last_updated;

	Sysclock_Init();
	Systick_Init();

	GPIO_Init();
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1); //DMA init
	ADC_Init();

	Serial_Init();

	Serial_Println(GIT_COMMIT_HASH);
	Result_t result = ADS131_Init();
	Speaker_Init();
	Speaker_Set(300, 200, 50, 5);

	char send_buffer[1024] = { 0 };

	sprintf(send_buffer, "RESULT = %i\n\r", result);
	Serial_Print(send_buffer);

	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);

		if(ADS131_UpdateData() != NOICE) Serial_Println("ADS UPDATE OOF");

		if (tick - uart_last_updated >= UART_UPDATE_TIME)
		{
			uart_last_updated = tick;
			sprintf(send_buffer, "%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\n\r", ADS131_GetData(0),ADS131_GetData(1), ADS131_GetData(2), ADS131_GetData(3), ADS131_GetData(4), ADS131_GetData(5),ADS131_GetData(6), ADS131_GetData(7), ADS131_GetStatus());
			Serial_Print(send_buffer);
		}

	}
}

void Error_Handler(void)
{
}
