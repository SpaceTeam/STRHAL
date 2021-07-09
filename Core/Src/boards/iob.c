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
#include <stdio.h>
#include "ui.h"
#include "dipswitch.h"

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

// {ENABLE},{SELECT},{IN}
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
	[10] = {{GPIOC,  LL_GPIO_PIN_8}, {GPIOC, LL_GPIO_PIN_7}, {GPIOA, LL_GPIO_PIN_4}},
	[11] = {{GPIOC,  LL_GPIO_PIN_9}, {GPIOA, LL_GPIO_PIN_9}, {GPIOA, LL_GPIO_PIN_5}}
};

//@formatter:on

void IOB_Pins_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	for (int i = 0; i < MAX_IOB_CHANNELS; i++)
	{
		// set enable pin as input
		GPIO_InitStruct.Pin = iob_channels[i].enable.pin;
		LL_GPIO_Init(iob_channels[i].enable.port, &GPIO_InitStruct);

		// set select pin as input
		GPIO_InitStruct.Pin = iob_channels[i].select.pin;
		LL_GPIO_Init(iob_channels[i].select.port, &GPIO_InitStruct);
	}
}

const CHANNEL_TYPE channel_lookup[4] =
{ CHANNEL_TYPE_DIGITAL_OUT, CHANNEL_TYPE_ADC16_SINGLE,			//PT100
		CHANNEL_TYPE_ADC16, CHANNEL_TYPE_ADC16 };

void Node_Init(void)
{
	IOB_Pins_Init();

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	for (int i = 0; i < MAX_IOB_CHANNELS; i++)
	{
		uint8_t type_index = LL_GPIO_IsInputPinSet(iob_channels[i].select.port, iob_channels[i].select.pin);
		type_index |= LL_GPIO_IsInputPinSet(iob_channels[i].enable.port, iob_channels[i].enable.pin) << 1;
		node.channels[i].type = channel_lookup[type_index];

		// assign enable pin that is needed within the channel, switch is used in case other channels also need such assignments
		switch (channel_lookup[type_index])
		{
			case CHANNEL_TYPE_DIGITAL_OUT:
				node.channels[i].channel.digital_out.enable_pin = &iob_channels[i].enable;
				break;
			case CHANNEL_TYPE_ADC16_SINGLE:
				node.channels[i].channel.adc16single.enable_pin = &iob_channels[i].enable;
				break;
			default:
				break;
		}

		// set enable pin as output
		GPIO_InitStruct.Pin = iob_channels[i].enable.pin;
		LL_GPIO_Init(iob_channels[i].enable.port, &GPIO_InitStruct);
	}

	node.node_id = NodeID_Get();
}

void IOB_InitAdc(void)
{
	Adc_Init();
	// init generic adcs
	node.generic_channel.bus1_voltage = Adc_AddRegularChannel(PWR_BUS_VOLTAGE_1);
	node.generic_channel.bus2_voltage = Adc_AddRegularChannel(PWR_BUS_VOLTAGE_2);
	node.generic_channel.power_voltage = Adc_AddRegularChannel(SUPPLY_VOLTAGE_SENSE);
	node.generic_channel.power_current = Adc_AddRegularChannel(SUPPLY_CURRENT_SENSE);

	// init channel adcs
	for (int i = 0; i < MAX_IOB_CHANNELS; i++)
	{
		CHANNEL_TYPE type = node.channels[i].type;
		switch (type)
		{
			case CHANNEL_TYPE_DIGITAL_OUT:
				node.channels[i].channel.digital_out.analog_in = Adc_AddRegularChannel(i);
				break;
			case CHANNEL_TYPE_ADC16:
				node.channels[i].channel.adc16.analog_in = Adc_AddRegularChannel(i);
				break;
			case CHANNEL_TYPE_ADC16_SINGLE:
			{
				AdcSingleData_t *single_data = Adc_AddSingleChannel(i);
				node.channels[i].channel.adc16single.analog_in = single_data->data;
				node.channels[i].channel.adc16single.last_measurement = single_data->last_measurement;
				break;
			}
			default:
				break;
		}
	}
	Adc_Calibrate();
	Adc_StartAdc();
}

void IOB_main(void)
{
	uint64_t tick = 0, old_tick = 0;

	Node_Init();
	IOB_InitAdc();

	char serial_str[1000] =
	{ 0 };

	sprintf(serial_str, "Node ID: %ld", node.node_id);
	Serial_PrintString(serial_str);

	for (int i = 0; i < MAX_IOB_CHANNELS; i++)
	{
		sprintf(serial_str, "Channel: %d -> %d", node.channels[i].id, node.channels[i].type);
		Serial_PrintString(serial_str);
	}

	GetMsg_t data;

	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);
		Can_checkFifo(IOB_MAIN_CAN_BUS);
		Can_checkFifo(1);

		if (tick - old_tick > 500)
		{
			old_tick = tick;
			for(int i = 0; i < 12; i++)
			{
				if(node.channels[i].type == CHANNEL_TYPE_DIGITAL_OUT)
				{
					data.variable_id = DIGITAL_OUT_MEASUREMENT;
					DigitalOut_ProcessMessage(i, DIGITAL_OUT_REQ_GET_VARIABLE, (uint8_t *) &data, 0);
				}
				else if(node.channels[i].type == CHANNEL_TYPE_ADC16_SINGLE)
				{
					data.variable_id = ADC16_SINGLE_DATA;
					Adc16Single_ProcessMessage(i, ADC16_SINGLE_REQ_GET_VARIABLE, (uint8_t *) &data, 0);
				}
				else
				{
					data.variable_id = ADC16_MEASUREMENT;
					Adc16_ProcessMessage(i, ADC16_REQ_GET_VARIABLE, (uint8_t *) &data, 0);
				}
			}
			Serial_PutString("\n");
		}

		if (Serial_CheckInput(serial_str))
		{
			Serial_PrintString(serial_str);
			uint8_t testdata[64] =
			{ 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

			SetMsg_t msg;
			msg.variable_id = DIGITAL_OUT_DUTY_CYCLE;

			if (strlen(serial_str) > 4)
			{
				if (Can_sendMessage(1, 0x598, testdata, 25) == NOICE)
					Serial_PrintString("message sent\n");
				else
					Serial_PrintString("FOCK\n");

			}
			else if (strlen(serial_str) > 3)
			{
				msg.value = 0x0;
				DigitalOut_ProcessMessage(5, DIGITAL_OUT_REQ_SET_VARIABLE, (uint8_t*) &msg, 0);
			}
			else if (strlen(serial_str) > 2)
			{
				msg.value = 0xFFFFFFFF;
				DigitalOut_ProcessMessage(5, DIGITAL_OUT_REQ_SET_VARIABLE, (uint8_t*) &msg, 0);
			}
			else
			{

				Can_MessageId_t message_id =
				{ 0 };
				message_id.info.special_cmd = STANDARD_SPECIAL_CMD;
				message_id.info.direction = MASTER2NODE_DIRECTION; //TODO REMOVE: Just here for debugging
				message_id.info.node_id = node.node_id;
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
