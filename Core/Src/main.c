#include "main.h"
#include "git_version.h"
#include "ADS131.h"
#include "cmds.h"
#include "can.h"
#include "gpio.h"
#include "spi.h"
#include "sysclock.h"
#include "systick.h"
#include "serial.h"
#include "tim.h"
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

	MX_GPIO_Init();
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1); //DMA init
	ADC_Init();
	MX_SPI1_Init();
	MX_UART4_Init();
	MX_TIM4_Init();
	MX_TIM1_Init();

	Serial_Init();

	Serial_Println(GIT_COMMIT_HASH);
	ADS131_Init();
	Speaker_Init();
	Speaker_Set(300, 200, 50, 5);

	//Can_Init(10);

	char send_buffer[1024] = { 0 };
	//char string[RECEIVE_STRING_SIZE] = "";

	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);
		//Can_checkFifo(0);
		//Can_checkFifo(1);

		/*if (Serial_CheckInput(string))
		{
			uint32_t i = 0;
			char *p = strtok(string, ":");
			char *array[4];

			while (p != NULL)
			{
				array[i++] = p;
				p = strtok(NULL, ":");
			}
			int32_t tone = atoi(array[0]);
			int32_t on = atoi(array[1]);
			int32_t off = atoi(array[2]);
			int32_t number = atoi(array[3]);
			//sprintf(send_buffer, "Echo: 0x%lx %ld : %ld - %ld  : %ld\n", tone, tone, on, off, number);
			//Serial_Println(send_buffer);
			Speaker_Set(tone, on, off, number);

			uint8_t testdata[64] = {
					0xDE,
					0xAD,
					0xBE,
					0xEF };
			Can_sendMessage(1, tone, testdata, 63);

			 //if ( == NOICE)
			 //Serial_Println("message sent\n");
			 //else
			 //Serial_Println("FOCK\n");

		}*/
		ADS131_UpdateData();

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
