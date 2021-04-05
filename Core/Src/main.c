#include "main.h"
#include "git_version.h"
#include "sysclock.h"
#include "systick.h"
#include "serial.h"
#include "adc.h"
#include "speaker.h"
#include <stdio.h>
#include <string.h>


#define UART_UPDATE_TIME 100

uint64_t tick = 0;
int main(void)
{

	uint64_t uart_last_updated;

	Sysclock_Init();
	Systick_Init();

	ADC_Init();
	Serial_Init();
	Serial_Println(GIT_COMMIT_HASH);
	Speaker_Init();
	Speaker_Set(300, 200, 50, 3);

	char send_buffer[256] =
	{ 0 };

	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);

		if (tick - uart_last_updated >= UART_UPDATE_TIME)
		{
			uart_last_updated = tick;
			sprintf(send_buffer, "%d, %d, %d, %d\n", ADC_GetData(0),
					ADC_GetData(1), ADC_GetData(2), ADC_GetData(3));
			Serial_Print(send_buffer);
			/*
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
