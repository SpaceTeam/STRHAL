#include <channel_util.h>
#include "main.h"
#include "git_version.h"
#include "cmds.h"
#include "can.h"
#include "sysclock.h"
#include "systick.h"
#include "serial.h"
#include "adc.h"
#include "flash.h"
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

	ADC_Init();
	Serial_Init();
	Serial_PrintString("STARTED?");
	Serial_PrintString(GIT_COMMIT_HASH);
	Speaker_Init();
	Speaker_Set(300, 200, 50, 5);
	Flash_Init();

	Can_Init(12);
	/*
	 uint8_t data[32] =
	 { 0 };
	 for (uint8_t c = 0; c < 32; c++)
	 {
	 data[c] = Flash_GetByteFromAddress(c);

	 }
	 for (uint8_t c = 0; c < 32; c++)
	 {
	 Serial_PutInt(c);
	 Serial_PutString(": ");
	 Serial_PutInt(data[c]);
	 Serial_PrintString("");
	 }
	 Flash_SetByteAtAddress(13, 42);

	 Flash_SetByteAtAddress(21, data[21] >> 1);
	 */

	char send_buffer[256] =
	{ 0 };
	char string[RECEIVE_STRING_SIZE] = "";

	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);
		Can_checkFifo(0);
		Can_checkFifo(1);

		if (Serial_CheckInput(string))
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
			sprintf(send_buffer, "Echo: 0x%lx %ld : %ld - %ld  : %ld\n", tone, tone, on, off, number);
			Serial_PrintString(send_buffer);

			Speaker_Set(tone, on, off, number);


		}

		if (tick - uart_last_updated >= UART_UPDATE_TIME)
		{
			uart_last_updated = tick;
			/*sprintf(send_buffer, "%d, %d, %d, %d\n", ADC_GetData(0), ADC_GetData(1), ADC_GetData(2), ADC_GetData(3));
			 Serial_Print(send_buffer);

			 sprintf(send_buffer + strlen(send_buffer), "%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %08ld\n\r", ADS131_getData(0),
			 ADS131_getData(1), ADS131_getData(2), ADS131_getData(3), ADS131_getData(4), ADS131_getData(5),
			 ADS131_getData(6), ADS131_getData(7), ADS131_getStatus());
			 Serial_print(send_buffer);
			 */
		}

	}
}

void Error_Handler(void)
{
}
