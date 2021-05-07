#if BOARD == LCB

#include "lcb.h"
#include "main.h"
#include "channels.h"
#include "ads131.h"
#include "speaker.h"
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
							{ 0, CHANNEL_TYPE_ADC24, {{0}} },
							{ 1, CHANNEL_TYPE_ADC24, {{0}} },
							{ 2, CHANNEL_TYPE_ADC24, {{0}} },
							{ 3, CHANNEL_TYPE_ADC24, {{0}} },
							{ 4, CHANNEL_TYPE_ADC24, {{0}} },
							{ 5, CHANNEL_TYPE_ADC24, {{0}} },
							{ 6, CHANNEL_TYPE_ADC24, {{0}} },
							{ 7, CHANNEL_TYPE_ADC24, {{0}} }
					}
				};
//@formatter:on

void LCB_main(void)
{
	uint64_t tick = 0;

	Ads131_Init();
	char serial_str[1000] =
	{ 0 };
	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);
		Ads131_UpdateData();
		Can_checkFifo(LCB_MAIN_CAN_BUS);
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
