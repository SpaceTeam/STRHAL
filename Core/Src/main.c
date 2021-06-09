#include <ads131.h>
#include <channel_util.h>
#include "main.h"
#include "git_version.h"
#include "cmds.h"
#include "can.h"
#include "gpio.h"
#include "sysclock.h"
#include "systick.h"
#include "serial.h"
#include "usart.h"
#include "adc.h"
#include "flash.h"
#include "speaker.h"
#include "lcb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dipswitch.h"

#define UART_UPDATE_TIME 100

uint64_t tick = 0;
int main(void)
{
	Sysclock_Init();
	Systick_Init();

	GPIO_Init();
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1); //DMA1 init
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2); //DMA2 init

	Serial_Init();
	Can_Init(NodeID_Get());

	Serial_PrintString("STARTED?");
	Serial_PrintString(GIT_COMMIT_HASH);
	Speaker_Init();
	Speaker_Set(300, 200, 50, 5);
	Flash_Init();

#if BOARD == LCB
	LCB_main();
#elif BOARD == IOB
	IOB_main();
#elif BOARD == BLMB
	BLMB_main();
#endif

}

void Error_Handler(void)
{
}
