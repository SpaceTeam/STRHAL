#include "board_config.h"
#if BOARD == LCB
#include "main.h"
#include "channels.h"
#include "ads131.h"
#include "speaker.h"
#include "can.h"
#include "serial.h"
#include "generic_channel.h"
#include "systick.h"
#include "ui.h"
#include <string.h>

//@formatter:off
Node_t node = { .node_id = 0, .firmware_version = 0xDEADBEEF,
				.generic_channel = { NULL, NULL, NULL, NULL, DEFAULT_REFRESH_DIVIDER, DEFAULT_REFRESH_RATE },
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

void LCB_InitAdc(void)
{
	Adc_Init();
	node.generic_channel.bus1_voltage = Adc_AddRegularChannel(PWR_BUS_VOLTAGE_1);
	node.generic_channel.bus2_voltage = Adc_AddRegularChannel(PWR_BUS_VOLTAGE_2);
	node.generic_channel.power_voltage = Adc_AddRegularChannel(SUPPLY_VOLTAGE_SENSE);
	node.generic_channel.power_current = Adc_AddRegularChannel(SUPPLY_CURRENT_SENSE);
	Adc_Calibrate();
	Adc_StartAdc();
}

void LCB_TIM2_IRQHandler(void)
{

}

void LCB_main(void)
{
	uint64_t tick = 0;

	LCB_InitAdc();
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
				if (Can_sendMessage(1, 0x598, testdata, 25) == NOICE)
					Serial_PrintString("message sent\n");
				else
					Serial_PrintString("FOCK\n");
			}
			else
			{

				Can_MessageId_t message_id =
				{ 0 };
				message_id.info.special_cmd = STANDARD_SPECIAL_CMD;
				message_id.info.direction = MASTER2NODE_DIRECTION; //TODO REMOVE: Just here for debugging
				message_id.info.node_id = NODE_ID;
				message_id.info.priority = STANDARD_PRIORITY;

				Can_MessageData_t data =
				{ 0 };
				data.bit.info.channel_id = GENERIC_CHANNEL_ID;
				data.bit.info.buffer = DIRECT_BUFFER;

				data.bit.cmd_id = GENERIC_REQ_GET_VARIABLE;
				GetMsg_t *getmsg = (GetMsg_t*) &data.bit.data;
				getmsg->variable_id = GENERIC_BUS2_VOLTAGE;

				Result_t result = Ui_SendCanMessage(DEBUG_CAN_BUS, message_id, &data, sizeof(GetMsg_t));
				//Result_t result = Generic_NodeInfo();

				if (result == NOICE)
					Serial_PrintString("Noice");
				else
					Serial_PrintString((result == OOF_CAN_TX_FULL) ? "OOF_CAN_TX_FULL" : "Oof");
			}
		}

	}
}

#endif
