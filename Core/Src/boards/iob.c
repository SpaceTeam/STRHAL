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
#include "git_version.h"

//@formatter:off
Node_t node = { .node_id = 0, .firmware_version = GIT_COMMIT_HASH_VALUE,
				.generic_channel = { NULL, NULL, NULL, NULL, DEFAULT_REFRESH_DIVIDER, DEFAULT_REFRESH_RATE },
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
							{11, CHANNEL_TYPE_UNKNOWN, {{0}} },
							{12, CHANNEL_TYPE_PNEUMATIC_VALVE, {{0}} },
							{13, CHANNEL_TYPE_PNEUMATIC_VALVE, {{0}} }
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

void IOB_InitPins(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	for (int i = 0; i < MAX_STANDARD_IOB_CHANNELS; i++)
	{
		// set enable pin as input
		GPIO_InitStruct.Pin = iob_channels[i].enable.pin;
		LL_GPIO_Init(iob_channels[i].enable.port, &GPIO_InitStruct);

		// set select pin as input
		GPIO_InitStruct.Pin = iob_channels[i].select.pin;
		LL_GPIO_Init(iob_channels[i].select.port, &GPIO_InitStruct);
	}

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
	for (int i = 0; i < MAX_STANDARD_IOB_CHANNELS; i++)
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
const CHANNEL_TYPE channel_lookup[4] =
{ CHANNEL_TYPE_DIGITAL_OUT, CHANNEL_TYPE_ADC16_SINGLE,			//PT100
		CHANNEL_TYPE_ADC16, CHANNEL_TYPE_ADC16 };

void IOB_Init(void)
{
	IOB_InitPins();

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	for (int i = 0; i < MAX_STANDARD_IOB_CHANNELS; i++)
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

	//TODO Read Config
	PneumaticValve_InitChannel(&node.channels[PNEUMATIC_VALVE_1_CHANNEL_ID].channel.pneumatic_valve, 2, 3, 1);
	PneumaticValve_InitChannel(&node.channels[PNEUMATIC_VALVE_2_CHANNEL_ID].channel.pneumatic_valve, 7, 6, 5);

	IOB_InitAdc();
}

void IOB_TIM2_IRQHandler(void)
{

}

void IOB_main(void)
{
	uint64_t tick = 0, old_tick = 0;

	IOB_Init();

	char serial_str[1000] =
	{ 0 };

	//TODO @ANDI DEBUG ....

	sprintf(serial_str, "Node ID: %ld", node.node_id);
	Serial_PrintString(serial_str);

	for (int i = 0; i < MAX_IOB_CHANNELS; i++)
	{
		sprintf(serial_str, "Channel: %d -> %d", node.channels[i].id, node.channels[i].type);
		Serial_PrintString(serial_str);
	}
	//GetMsg_t data;
	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);
		Can_checkFifo(IOB_MAIN_CAN_BUS);
		Can_checkFifo(DEBUG_CAN_BUS);

		PneumaticValve_Update(&node.channels[PNEUMATIC_VALVE_1_CHANNEL_ID].channel.pneumatic_valve);
		PneumaticValve_Update(&node.channels[PNEUMATIC_VALVE_2_CHANNEL_ID].channel.pneumatic_valve);


		 if (tick - old_tick > 500)
		 {
		 old_tick = tick;
		 uint16_t position = 0;
			Result_t result = Adc16_GetRawData(5, &position);
			Serial_PutString("Position: ");
			Serial_PutInt(position);
			Serial_PutString("  target: ");
			Serial_PutInt(node.channels[PNEUMATIC_VALVE_2_CHANNEL_ID].channel.pneumatic_valve.target_position);
			Serial_PutString("  off: ");
			Serial_PutInt(DigitalOut_GetState(node.channels[PNEUMATIC_VALVE_2_CHANNEL_ID].channel.pneumatic_valve.off_channel_id));
			Serial_PutString("  on: ");
			Serial_PrintInt(DigitalOut_GetState(node.channels[PNEUMATIC_VALVE_2_CHANNEL_ID].channel.pneumatic_valve.on_channel_id));

			/*
		 for (int i = 0; i < MAX_IOB_CHANNELS; i++)
		 {
		 switch (node.channels[i].type)
		 {
		 case CHANNEL_TYPE_DIGITAL_OUT:
		 data.variable_id = DIGITAL_OUT_MEASUREMENT;
		 DigitalOut_ProcessMessage(i, DIGITAL_OUT_REQ_GET_VARIABLE, (uint8_t*) &data, 0);
		 break;
		 case CHANNEL_TYPE_ADC16_SINGLE:
		 data.variable_id = ADC16_SINGLE_DATA;
		 Adc16Single_ProcessMessage(i, ADC16_SINGLE_REQ_GET_VARIABLE, (uint8_t*) &data, 0);
		 break;
		 case CHANNEL_TYPE_ADC16:
		 data.variable_id = ADC16_MEASUREMENT;
		 Adc16_ProcessMessage(i, ADC16_REQ_GET_VARIABLE, (uint8_t*) &data, 0);
		 break;
		 default:
		 break;
		 }
		 }
		 Serial_PrintString(" ");*/
		 }


		if (Serial_CheckInput(serial_str))
		{
			Serial_PrintString(serial_str);
			node.channels[PNEUMATIC_VALVE_2_CHANNEL_ID].channel.pneumatic_valve.target_position = atoi(serial_str);
		}
	}
}

#endif
