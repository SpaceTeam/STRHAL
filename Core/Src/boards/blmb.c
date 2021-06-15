#include "board_config.h"
#if BOARD == BLMB
#include "main.h"
#include "channels.h"
#include "speaker.h"
#include "can.h"
#include "foc/tmc6200/TMC6200_highLevel.h"
#include "foc/tmc6200/TMC6200.h"
#include "foc/tmc4671/TMC4671_highLevel.h"
#include "foc/tmc4671/TMC4671.h"
#include "foc/swdriver.h"
#include "foc/as5147.h"
#include "spi.h"
#include "serial.h"
#include "generic_channel.h"
#include "systick.h"
#include "ui.h"
#include "tim.h"
#include <string.h>

//@formatter:off
Node_t node = { .node_id = 0, .firmware_version = 0xDEADBEEF,
				.generic_channel = { 0 },
				.channels =
					{
							{ 0, CHANNEL_TYPE_SERVO, {{0}} }
					}
				};
//@formatter:on

void BLMB_InitAdc(void)
{
	Adc_Init();
	node.generic_channel.bus1_voltage = Adc_AddRegularChannel(PWR_BUS_VOLTAGE_1);
	node.generic_channel.bus2_voltage = Adc_AddRegularChannel(PWR_BUS_VOLTAGE_2);
	node.generic_channel.power_voltage = Adc_AddRegularChannel(SUPPLY_VOLTAGE_SENSE);
	node.generic_channel.power_current = Adc_AddRegularChannel(SUPPLY_CURRENT_SENSE);
	Adc_Calibrate();
	Adc_StartAdc();
}

void BLMB_InitFoc(void)
{

	TIM4_Init();

	LL_TIM_EnableCounter(TIM4);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH3);

	SPI1_Init(LL_SPI_DATAWIDTH_8BIT, LL_SPI_PHASE_2EDGE, LL_SPI_POLARITY_HIGH, LL_SPI_NSS_SOFT, LL_SPI_BAUDRATEPRESCALER_DIV128);

	Systick_BusyWait(100);
	tmc6200_highLevel_init();
	Systick_BusyWait(10);
	swdriver_setEnable(true);
	Systick_BusyWait(10);
/*
	TMC4671_highLevel_init();
	Systick_BusyWait(10);

	TMC4671_highLevel_initEncoder();

	Systick_BusyWait(100);

	tmc4671_writeInt(TMC4671_PID_POSITION_ACTUAL, (uint32_t) ((float) as5147_getAngle(BLMB_POSITION_ENCODER) * 32 * 231.1222));

	TMC4671_highLevel_setPosition(node.channels[BLMB_SERVO_CHANNEL].channel.servo.position_set);

	TMC4671_highLevel_positionMode2();
*/
	Systick_BusyWait(100);

}

void BLMB_main(void)
{
	uint64_t tick = 0;
	uint64_t old_tick = 0;

	BLMB_InitAdc();

	BLMB_InitFoc();

	char serial_str[1000] =
	{ 0 };
	while (1)
	{
		tick = Systick_GetTick();
		Speaker_Update(tick);
		Can_checkFifo(LCB_MAIN_CAN_BUS);
		Can_checkFifo(1);

	//	TMC4671_highLevel_setPosition(node.channels[BLMB_SERVO_CHANNEL].channel.servo.position_set);
		if (tick - old_tick >= 1000)
		{
			old_tick = tick;
			Serial_PrintHex(tmc6200_readInt(TMC6200_GSTAT));
			Serial_PrintInt(tick);
		}
		if (Serial_CheckInput(serial_str))
		{
			Serial_PrintString(serial_str);
			node.channels[BLMB_SERVO_CHANNEL].channel.servo.position_set = atoi(serial_str);

		}

	}
}

#endif
