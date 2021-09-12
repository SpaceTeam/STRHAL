#include "channel_util.h"
#include "main.h"
#include "cmds.h"
#include "can.h"
#include "channels.h"
#include "gpio.h"
#include "tim.h"
#include "sysclock.h"
#include "systick.h"
#include "serial.h"
#include "flash.h"
#include "speaker.h"
#include "dipswitch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UART_UPDATE_TIME 100

uint64_t tick = 0;
int main(void)
{
	Sysclock_Init();
	Systick_Init();

	GPIO_Init();
	Serial_Init();

	node.node_id = NodeID_Get();

#if BOARD != BLMB
	Speaker_Init();
	Speaker_Set(300, 2000 / node.node_id, 100 / node.node_id, node.node_id);
#endif
	Can_Init(node.node_id);
	Serial_PrintInt(node.node_id);

	Serial_PrintString("STARTED");
	Serial_PrintString(GIT_COMMIT_HASH);

	TIM2_Init(node.generic_channel.refresh_rate);
	Flash_Init();

	LL_TIM_EnableCounter(TIM2);


#if BOARD == LCB
	LCB_main();
#elif BOARD == IOB
	IOB_main();
#elif BOARD == BLMB
	BLMB_main();
#endif

}

static void SendGenericData(void)
{
	Result_t result = Generic_Data();

	//TODO Add proper Error handling
	if (result != NOICE)
	{
		Serial_PutString("CAN ERROR:");
		Serial_PrintInt(result); //TODO @ANDI Change TO Debug_PrintResult (and implement it)
	}
	else
		//Serial_PutString("0x1234567");
		Serial_PutString(", ");
}

void main_TIM2_IRQHandler(void)
{
	static int32_t refresh_counter = 0;
	if (node.generic_channel.refresh_divider)
		if (++refresh_counter >= node.generic_channel.refresh_divider)
		{
			SendGenericData();
			refresh_counter = 0;
		}
}

void TIM2_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM2) != 0)
	{
		main_TIM2_IRQHandler();
#if BOARD == LCB
		LCB_TIM2_IRQHandler();
#elif BOARD == IOB
		IOB_TIM2_IRQHandler();
#endif
		LL_TIM_ClearFlag_UPDATE(TIM2);
	}
}

void Error_Handler(void)
{
}
