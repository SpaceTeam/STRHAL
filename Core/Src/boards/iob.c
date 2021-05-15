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
	[10] = {{GPIOC,  LL_GPIO_PIN_8}, {GPIOC, LL_GPIO_PIN_8}, {GPIOA, LL_GPIO_PIN_4}},
	[11] = {{GPIOC,  LL_GPIO_PIN_9}, {GPIOA, LL_GPIO_PIN_9}, {GPIOA, LL_GPIO_PIN_5}}
};

//@formatter:on

// TODO Channel type init ausbessern, adc16/dig out über enable pin steuern

void IOB_main(void)
{
	uint64_t tick = 0;

	Adc_Init();
	IOB_pins_init();
	Node_init();

	char serial_str[1000] =
	{ 0 };

	for(int i = 0; i < MAX_IOB_CHANNELS; i++)
	{
		sprintf(serial_str,"Channel: %d -> %d", node.channels[i].id, node.channels[i].type);
		Serial_PrintString(serial_str);
	}
	uint8_t flag = 0;

	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);
		Can_checkFifo(IOB_MAIN_CAN_BUS);
		Can_checkFifo(1);
		sprintf(serial_str,"ADC: %d", Adc_GetData(7));
		Serial_PrintString(serial_str);

		if(tick % 2000 == 0)
		{
			SetMsg_t data;
			if(flag == 0)
			{
				data.value = 1;
				flag = 1;
				LL_GPIO_SetOutputPin(iob_channels[5].enable.port,iob_channels[5].enable.pin);
			}
			else
			{
				data.value = 0;
				flag = 0;
				LL_GPIO_ResetOutputPin(iob_channels[5].enable.port,iob_channels[5].enable.pin);
			}
			data.variable_id = DIGITAL_OUT_STATE;
			//DigitalOut_ProcessMessage(2, DIGITAL_OUT_REQ_SET_VARIABLE, (uint8_t *) &data, 0);
		}

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

void IOB_pins_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	for(int i = 0; i < MAX_IOB_CHANNELS; i++)
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
{
		CHANNEL_TYPE_DIGITAL_OUT,
		CHANNEL_TYPE_ADC16,			//PT100
		CHANNEL_TYPE_ADC16,
		CHANNEL_TYPE_ADC16
};

void Node_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	for(int i = 0; i < MAX_IOB_CHANNELS; i++)
	{
		uint8_t type_index = LL_GPIO_IsInputPinSet(iob_channels[i].select.port, iob_channels[i].select.pin);
		type_index |= LL_GPIO_IsInputPinSet(iob_channels[i].enable.port, iob_channels[i].enable.pin) << 1;
		node.channels[i].type = channel_lookup[type_index];
		// assign enable pin that is needed within the channel, switch is used in case other channels also need such assignments
		switch(channel_lookup[type_index])
		{
			case CHANNEL_TYPE_DIGITAL_OUT:
				node.channels[i].channel.digital_out.enable_pin = iob_channels[i].enable.pin;
				break;
			default:
				break;
		}

		// set enable pin as output
		GPIO_InitStruct.Pin = iob_channels[i].enable.pin;
		LL_GPIO_Init(iob_channels[i].enable.port, &GPIO_InitStruct);
	}

	// Read dipswitches and set node id accordingly

	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	for(int i = 0; i < 8; i++)
	{
		// set enable dipswitch pin as input
		GPIO_InitStruct.Pin = 0x1UL << i;
		LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}

	node.node_id = LL_GPIO_ReadInputPort(GPIOD) & 0xFF;
}

#endif
