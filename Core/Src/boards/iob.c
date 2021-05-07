#include "board_config.h"
#if BOARD == IOB
#include "main.h"
#include "channels.h"
#include "speaker.h"
#include "adc.h"
#include "can.h"
#include "serial.h"
#include "generic_channel.h"
#include "systick.h"
#include <string.h>



//@formatter:off
Node_t node = { .node_id = 0, .firmware_version = 0xDEADBEEF,
				.generic_channel = { 0 },
				.channels =
					{
							{ 0, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 1, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 2, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 3, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 4, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 5, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 6, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 7, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 8, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{ 9, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{10, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{11, CHANNEL_TYPE_UNKNOWN, {{0}} }
					}
				};

IOB_Pins_t iob_channels[] =
{
	[ 0] = {{GPIOE,  LL_GPIO_PIN_3}, {GPIOC,LL_GPIO_PIN_13}, {GPIOC, LL_GPIO_PIN_0}},
	[ 1] = {{GPIOE,  LL_GPIO_PIN_1}, {GPIOE, LL_GPIO_PIN_0}, {GPIOC, LL_GPIO_PIN_1}},
	[ 2] = {{GPIOB,  LL_GPIO_PIN_8}, {GPIOB, LL_GPIO_PIN_7}, {GPIOC, LL_GPIO_PIN_2}},
	[ 3] = {{GPIOB,  LL_GPIO_PIN_6}, {GPIOB, LL_GPIO_PIN_5}, {GPIOC, LL_GPIO_PIN_3}},
	[ 4] = {{GPIOE, LL_GPIO_PIN_14}, {GPIOB,LL_GPIO_PIN_14}, {GPIOC, LL_GPIO_PIN_4}},
	[ 5] = {{GPIOE,  LL_GPIO_PIN_7}, {GPIOE,LL_GPIO_PIN_11}, {GPIOC, LL_GPIO_PIN_5}},
	[ 6] = {{GPIOB,  LL_GPIO_PIN_4}, {GPIOB, LL_GPIO_PIN_3}, {GPIOA, LL_GPIO_PIN_0}},
	[ 7] = {{GPIOA, LL_GPIO_PIN_15}, {GPIOC,LL_GPIO_PIN_12}, {GPIOA, LL_GPIO_PIN_1}},
	[ 8] = {{GPIOE, LL_GPIO_PIN_13}, {GPIOB,LL_GPIO_PIN_15}, {GPIOA, LL_GPIO_PIN_2}},
	[ 9] = {{GPIOC,  LL_GPIO_PIN_6}, {GPIOD,LL_GPIO_PIN_15}, {GPIOA, LL_GPIO_PIN_3}},
	[10] = {{GPIOC,  LL_GPIO_PIN_8}, {GPIOC, LL_GPIO_PIN_8}, {GPIOA, LL_GPIO_PIN_4}},
	[11] = {{GPIOC,  LL_GPIO_PIN_9}, {GPIOA, LL_GPIO_PIN_9}, {GPIOA, LL_GPIO_PIN_5}}
};

//@formatter:on



void IOB_main(void)
{
	uint64_t tick = 0;

	Adc_Init();
	char serial_str[1000] =
	{ 0 };
	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);
		Can_checkFifo(IOB_MAIN_CAN_BUS);
		Can_checkFifo(1);

		if (Serial_CheckInput(serial_str))
		{
			Serial_PrintString(serial_str);

			uint8_t testdata[64] =
			{ 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

			if (strlen(serial_str) > 4)
			{
				if (Can_sendMessage(0, 0x2AA, testdata, 25) == NOICE)
					Serial_PrintString("message sent\n");
				else
					Serial_PrintString("FOCK\n");
			}
			else
			{
				if (Can_sendMessage(0, 0x555, testdata, 25) == NOICE)
					Serial_PrintString("message sent\n");
				else
					Serial_PrintString("FOCK\n");

			}
			/*Result_t result = Generic_NodeInfo();

			 if (result == NOICE)
			 Serial_PrintString("Noice");
			 else
			 Serial_PrintString((result == OOF_CAN_TX_FULL) ? "OOF_CAN_TX_FULL" : "Oof");

			 */
		}

	}
}

#endif
