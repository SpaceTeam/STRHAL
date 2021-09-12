#include "board_config.h"
#if BOARD == BLMB
#include "main.h"
#include "blmb_ui.h"
#include "blmb_settings.h"
#include "channels.h"
#include "speaker.h"
#include "can.h"
#include "dac.h"
#include "foc/tmc6200/TMC6200_highLevel.h"
#include "foc/tmc6200/TMC6200.h"
#include "foc/tmc6200/TMC6200_Register.h"
#include "foc/tmc4671/TMC4671_highLevel.h"
#include "foc/tmc4671/TMC4671.h"
#include "foc/swdriver.h"
#include <foc/as5x47.h>
#include "spi.h"
#include "serial.h"
#include "generic_channel.h"
#include "systick.h"
#include "ui.h"
#include "pwm.h"
#include "tim.h"
#include "git_version.h"
#include <string.h>

//@formatter:off
Node_t node = { .node_id = 0, .firmware_version = GIT_COMMIT_HASH_VALUE,
				.generic_channel = { 0 },
				.channels =
				{
					{ 0, CHANNEL_TYPE_SERVO, {{0}} }
				}
				};
//@formatter:on
uint32_t BLMB_CalcMotorPos(uint32_t var)
{
	return (uint32_t) (BLMB_REDUCTION * var);
}

void BLMB_LoadSettings(void)
{
	BLMB_Settings_t settings =
	{ 0 };
	BlmbSettings_Load(&settings);
	Servo_Channel_t *servo = &node.channels[BLMB_SERVO_CHANNEL].channel.servo;
	servo->startpoint = settings.startpoint;
	servo->endpoint = settings.endpoint;
	servo->max_accel = settings.max_accel;
	servo->max_speed = settings.max_speed;
	servo->max_torque = settings.max_torque;
	Serial_PutString("\r\nLoadSettings\r\nStartpoint: ");
	Serial_PutInt(servo->startpoint);
	Serial_PutString("\r\nEndpoint: ");
	Serial_PrintInt(servo->endpoint);
}

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
	// 25MHz clock for foc controller and gate driver FIXME currently 24MHz
	TIM4_Init();
	LL_TIM_EnableCounter(TIM4);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH3);
	SPI1_Init(LL_SPI_DATAWIDTH_8BIT, LL_SPI_PHASE_2EDGE, LL_SPI_POLARITY_HIGH, LL_SPI_NSS_SOFT, LL_SPI_BAUDRATEPRESCALER_DIV256);

	AS5x47_Init(BLMB_POSITION_ENCODER);
	AS5x47_Init(BLMB_MOTOR_ENCODER);

	Systick_BusyWait(100);
	tmc6200_highLevel_init();
	Systick_BusyWait(10);
	swdriver_setEnable(true);
	Systick_BusyWait(10);

	TMC4671_highLevel_init();
	Systick_BusyWait(10);

	Serial_PutString("motor encoder init:");
	TMC4671_highLevel_initEncoder();
	Serial_PrintString(" done");

	Systick_BusyWait(100);

	uint16_t pos = AS5x47_GetAngle(BLMB_POSITION_ENCODER);

	tmc4671_writeInt(TMC4671_ABN_DECODER_COUNT, 0);
	tmc4671_writeInt(TMC4671_PID_POSITION_ACTUAL, BLMB_CalcMotorPos(pos));
	tmc4671_writeInt(TMC4671_PID_POSITION_TARGET, BLMB_CalcMotorPos(pos));

	Serial_PutString("position mode:");
	TMC4671_highLevel_positionMode2();
	Serial_PrintString(" done");

	Systick_BusyWait(100);
}

void BLMB_InitGPIO(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	// button inputs
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;

	GPIO_InitStruct.Pin = BLMB_CW_Button_Pin;
	LL_GPIO_Init(BLMB_CW_Button_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BLMB_Sel_Button_Pin;
	LL_GPIO_Init(BLMB_Sel_Button_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BLMB_CCW_Button_Pin;
	LL_GPIO_Init(BLMB_CCW_Button_GPIO_Port, &GPIO_InitStruct);

	//status inputs
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	GPIO_InitStruct.Pin = STATUS_Pin;
	LL_GPIO_Init(STATUS_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = FAULT_Pin;
	LL_GPIO_Init(FAULT_GPIO_Port, &GPIO_InitStruct);

	//enable and chip select outputs
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	GPIO_InitStruct.Pin = EN_Pin;
	LL_GPIO_Init(EN_GPIO_Port, &GPIO_InitStruct);
	LL_GPIO_ResetOutputPin(EN_GPIO_Port, EN_Pin);

	GPIO_InitStruct.Pin = CSN_CTR_Pin;
	LL_GPIO_Init(CSN_CTR_GPIO_Port, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(CSN_CTR_GPIO_Port, CSN_CTR_Pin);

	GPIO_InitStruct.Pin = CSN_DRV_Pin;
	LL_GPIO_Init(CSN_DRV_GPIO_Port, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(CSN_DRV_GPIO_Port, CSN_DRV_Pin);

	GPIO_InitStruct.Pin = CSN_ENC0_Pin;
	LL_GPIO_Init(CSN_ENC0_GPIO_Port, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(CSN_ENC0_GPIO_Port, CSN_ENC0_Pin);

	GPIO_InitStruct.Pin = CSN_ENC1_Pin;
	LL_GPIO_Init(CSN_ENC1_GPIO_Port, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(CSN_ENC1_GPIO_Port, CSN_ENC1_Pin);

	GPIO_InitStruct.Pin = CSN_ENC2_Pin;
	LL_GPIO_Init(CSN_ENC2_GPIO_Port, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(CSN_ENC2_GPIO_Port, CSN_ENC2_Pin);

	GPIO_InitStruct.Pin = CSN_ENC3_Pin;
	LL_GPIO_Init(CSN_ENC3_GPIO_Port, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(CSN_ENC3_GPIO_Port, CSN_ENC3_Pin);
}

void BLMB_disableMotor(void)
{
	static uint16_t angle_correct_counter = 0;
	if ((abs((int32_t) TMC4671_highLevel_getPositionActual() - (int32_t) TMC4671_highLevel_getPositionTarget()) < BLMB_ERROR_THRESHOLD) && angle_correct_counter < 65535)
		angle_correct_counter++;
	else
		angle_correct_counter = 0;

	if (angle_correct_counter > BLMB_ERROR_TIMER)
		swdriver_setEnable(false);
	else
		swdriver_setEnable(true);
}

void BLMB_main(void)
{
	uint64_t tick = 0;
	uint64_t old_tick = 0;

	BLMB_InitGPIO(); // button inputs, status inputs, chip select outputs

	BLMB_LoadSettings();
	BLMB_InitAdc();
	BLMB_InitFoc();

	Dac_Init();
	PWM_InitPwmIn();
	Serial_PrintString("pwm in done");

	//BlmbUi_InitTIM(); // button handling timers
	BlmbUi_InitEXTI(); // button interrupts
	Servo_InitChannel(&node.channels[BLMB_SERVO_CHANNEL].channel.servo);

	char serial_str[1000] =
	{ 0 };

	while (1)
	{
		tick = Systick_GetTick();
		//Speaker_Update(tick);
		Can_checkFifo(BLMB_MAIN_CAN_BUS);
		Can_checkFifo(DEBUG_CAN_BUS);

		node.channels[BLMB_SERVO_CHANNEL].channel.servo.position = AS5x47_GetAngle(BLMB_POSITION_ENCODER);
		Dac_SetValue(node.channels[BLMB_SERVO_CHANNEL].channel.servo.position >> 2);

		uint16_t position = 0;
		Result_t result = BlmbUi_CheckInput(&position);

		if (BlmbUi_GetUiMode() == BLMB_UI_MODE_NORMAL)
		{
			if (result == OOF_NO_NEW_DATA)
				result = PWM_GetPWM(&position);
		}
		if (result == NOICE)
			Servo_SetPosition(&node.channels[BLMB_SERVO_CHANNEL].channel.servo, position);

		TMC4671_highLevel_setPosition(BLMB_CalcMotorPos(node.channels[BLMB_SERVO_CHANNEL].channel.servo.target_position));

		BLMB_disableMotor();

		if (tick - old_tick >= 250)
		{
			old_tick = tick;
			/*
			 Serial_PutInt(LL_TIM_IC_GetCaptureCH1(TIM1));
			 Serial_PutString(" ");
			 Serial_PutInt(LL_TIM_IC_GetCaptureCH2(TIM1));
			 Serial_PutString(" ");
			 Serial_PrintInt(LL_TIM_IC_GetCaptureCH2(TIM1) - LL_TIM_IC_GetCaptureCH1(TIM1));


			 Serial_PutInt(PWM_GetPWM());
			 Serial_PutString(", ");
			 Serial_PutInt(LL_TIM_IC_GetCaptureCH2(TIM1) - LL_TIM_IC_GetCaptureCH1(TIM1));
			 Serial_PutString(", ");
			 Serial_PutInt(AS5x47_GetAngle(BLMB_POSITION_ENCODER));
			 Serial_PutString(", ");
			 Serial_PutInt(AS5x47_GetAngle(BLMB_MOTOR_ENCODER));
			 Serial_PutString(", ");

			 //Serial_PutInt(tmc4671_readInt(TMC4671_ABN_DECODER_COUNT));
			 Serial_PrintInt(tmc4671_readInt(TMC4671_PID_POSITION_ACTUAL));
			 Serial_PutString(", ");

			 //Serial_PrintHex(as5147_getAngle(BLMB_POSITION_ENCODER));
			 Serial_PutHex(tmc6200_readRegister(TMC6200_GSTAT));
			 Serial_PutString(", ");

			 //Serial_PrintInt(tick);
			 //Serial_PrintInt(LL_GPIO_IsInputPinSet(STATUS_GPIO_Port, STATUS_Pin));
			 Serial_PrintInt(LL_GPIO_IsInputPinSet(FAULT_GPIO_Port, FAULT_Pin));
			 */
			/*
			 Serial_PutInt(tmc4671_readInt(TMC4671_ABN_DECODER_COUNT));
			 Serial_PutString(", ");
			 Serial_PutInt(tmc4671_readInt(TMC4671_PID_POSITION_ACTUAL));
			 Serial_PutString(", ");
			 Serial_PutInt(AS5x47_GetAngle(BLMB_MOTOR_ENCODER));
			 Serial_PutString(", ");*/
			/*
			 Serial_PutInt(node.channels[BLMB_SERVO_CHANNEL].channel.servo.startpoint);
			 Serial_PutString(", ");
			 Serial_PutInt(node.channels[BLMB_SERVO_CHANNEL].channel.servo.endpoint);
			 Serial_PutString(", ");
			 Serial_PutInt(node.channels[BLMB_SERVO_CHANNEL].channel.servo.target_position);
			 Serial_PutString(", ");
			 Serial_PutInt(node.channels[BLMB_SERVO_CHANNEL].channel.servo.target_percentage);
			 Serial_PutString(", ");
			 Serial_PrintInt(node.channels[BLMB_SERVO_CHANNEL].channel.servo.position);*/
		}
		if (Serial_CheckInput(serial_str))
		{
			//Serial_PrintString(serial_str);
			uint32_t input = atoi(serial_str);
			/*			if(input == 0)
			 TMC4671_highLevel_pwmOff();
			 else
			 TMC4671_highLevel_positionMode2();*/
			Servo_SetPosition(&node.channels[BLMB_SERVO_CHANNEL].channel.servo, input);

		}
	}
}

#endif
