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

static AdcData_t* torque_ptr = NULL;

//@formatter:off
Node_t node = { .node_id = 0, .firmware_version = GIT_COMMIT_HASH_VALUE,
		.generic_channel = { NULL, NULL, NULL, NULL, DEFAULT_REFRESH_DIVIDER, DEFAULT_REFRESH_RATE },
				.channels =
				{
					{ 0, CHANNEL_TYPE_SERVO, {{0}} },
					{ 1, CHANNEL_TYPE_ADC16, {{0}} },
					{ 2, CHANNEL_TYPE_ADC16, {{0}} }
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
	servo->p_param = settings.p_param;
	servo->i_param = settings.i_param;
	servo->d_param = settings.d_param;
	servo->pwm_in_enabled = settings.pwm_in_enabled;
	servo->refresh_divider = 0; // TODO ask about default setting
	servo->pressure_control_enabled = settings.pressure_control_enabled;
	servo->pos_p_param = settings.pos_p_param;
	servo->pos_i_param = settings.pos_i_param;
	servo->vel_p_param = settings.vel_p_param;
	servo->vel_i_param = settings.vel_i_param;
	servo->torq_p_param = settings.torq_p_param;
	servo->torq_i_param = settings.torq_i_param;

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

	node.channels[1].channel.adc16.analog_in = Adc_AddRegularChannel(1);
	node.channels[2].channel.adc16.analog_in = torque_ptr;
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

//	Serial_PutString("motor encoder init:");
	TMC4671_highLevel_initEncoder();
//	Serial_PrintString(" done");

	Systick_BusyWait(100);

	uint16_t pos = AS5x47_GetAngle(BLMB_POSITION_ENCODER) << 2;

	tmc4671_writeInt(TMC4671_ABN_DECODER_COUNT, 0);
	tmc4671_writeInt(TMC4671_PID_POSITION_ACTUAL, BLMB_CalcMotorPos(pos));
	tmc4671_writeInt(TMC4671_PID_POSITION_TARGET, BLMB_CalcMotorPos(pos));

	//Serial_PutString("position mode:");
	TMC4671_highLevel_positionMode2();
	//Serial_PrintString(" done");

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
	if ((labs((int32_t) TMC4671_highLevel_getPositionActual() - (int32_t) TMC4671_highLevel_getPositionTarget()) < BLMB_ERROR_THRESHOLD) && angle_correct_counter < BLMB_ERROR_TIMER)
		angle_correct_counter++;
	else
		angle_correct_counter = 0;

	if (angle_correct_counter > BLMB_ERROR_TIMER)
		swdriver_setEnable(false);
	else
		swdriver_setEnable(true);
}

Result_t BLMB_PressureControl(uint16_t *return_var, Servo_Channel_t *servo, AdcData_t *pressure_raw)
{
	static uint64_t contrl_tick_last = 0;
	static float pressure_old = -100;
	static float threshold = 0;

	if(Systick_GetTick() > contrl_tick_last) // at least 1ms passed
	{
		float dt = Systick_GetTick() - contrl_tick_last;
		contrl_tick_last = Systick_GetTick();
		//static float valvePercentage = 1; //init valve percentage

		float pressure = (float)*pressure_raw * 0.023497 - 8.74088; //TODO: change to actual data sheet conversion
		static float filt_pressure = -100;
		filt_pressure = filt_pressure * (0.999) + pressure * 0.001;
		float dp = filt_pressure - pressure_old;
		float gradient = dp / dt;
		static float gradient_filt = 0;
		gradient_filt = gradient_filt * 0.99 + gradient * 0.01;
		static uint16_t grad_pos_count = 0;
		if(gradient_filt >= -0.001)
		{
			grad_pos_count++;
		}
		else
		{
			grad_pos_count = 0;
		}
		if(pressure > threshold) //if pressure too high
		{
			threshold = servo->target_pressure - servo->pressure_hysteresis/10;
			if(grad_pos_count > 100)
			{
				if(servo->target_percentage < 0xffff)
				{
					*return_var = servo->target_percentage + ((servo->target_percentage < 0x4000) ? 0x1A00 : 0x0400); // ramp up to open
				}
			}
		}
		else // pressure < threshold
		{
			threshold = servo->target_pressure;
			if(servo->target_percentage != 0)
			{
				*return_var = 0;
			}
		}
		pressure_old = filt_pressure; //set old pressure to current pressure
	}

	return NOICE;
}

void BLMB_main(void)
{
	uint64_t tick = 0;
	uint64_t old_tick = 0;
	uint8_t blink_counter = 0;
	BLMB_InitGPIO(); // button inputs, status inputs, chip select outputs
	Servo_InitChannel(&node.channels[BLMB_SERVO_CHANNEL].channel.servo);

	BLMB_LoadSettings();
	BLMB_InitAdc();
	BLMB_InitFoc();

	Dac_Init();
	PWM_InitPwmIn();
	//Serial_PrintString("pwm in done");

	//BlmbUi_InitTIM(); // button handling timers
	BlmbUi_InitEXTI(); // button interrupts

	char serial_str[1000] =
	{ 0 };
	Servo_Channel_t *servo = &node.channels[BLMB_SERVO_CHANNEL].channel.servo;
	while (1)
	{
		tick = Systick_GetTick();
		//Speaker_Update(tick);
		Can_checkFifo(BLMB_MAIN_CAN_BUS);
		Can_checkFifo(DEBUG_CAN_BUS);

		servo->position = AS5x47_GetAngle(BLMB_POSITION_ENCODER);
		Servo_GetRawData(BLMB_SERVO_CHANNEL, NULL);
		Dac_SetValue(servo->position_percentage >> 2);
		uint16_t position = 0;
		Result_t result;

		//TODO calc position with PID if enabled
		if(servo->pressure_control_enabled)
		{
			result = BLMB_PressureControl(&position, servo, node.channels[BLMB_SENSOR_CHANNEL].channel.adc16.analog_in);
		}
		else
		{
			result = BlmbUi_CheckInput(&position);
		}

		if (BlmbUi_GetUiMode() == BLMB_UI_MODE_NORMAL)
		{
			if (result == OOF_NO_NEW_DATA)
				result = PWM_GetPWM(&position);
		}
		if (result == NOICE)
		{
			Servo_SetPosition(servo, position);
			LL_GPIO_SetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
		}
		else
			LL_GPIO_ResetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);

		TMC4671_highLevel_setPosition(BLMB_CalcMotorPos(servo->target_position));

		*torque_ptr = TMC4671_highLevel_getTorqueActual();

		BLMB_disableMotor();

		if (tick - old_tick >= 250)
		{
			old_tick = tick;
			blink_counter++;
			if (blink_counter % 2)
			{
				if (servo->position_percentage < ENDPOINT_THRESHOLD)
				{
					LL_GPIO_ResetOutputPin(LED_STATUS_1_GPIO_Port, LED_STATUS_1_Pin);
					LL_GPIO_SetOutputPin(LED_STATUS_2_GPIO_Port, LED_STATUS_2_Pin);
				}
				else if ((((uint32_t) 1UL << 16UL) - servo->position_percentage) < ENDPOINT_THRESHOLD)
				{
					LL_GPIO_SetOutputPin(LED_STATUS_1_GPIO_Port, LED_STATUS_1_Pin);
					LL_GPIO_ResetOutputPin(LED_STATUS_2_GPIO_Port, LED_STATUS_2_Pin);
				}
				else
				{
					LL_GPIO_ResetOutputPin(LED_STATUS_1_GPIO_Port, LED_STATUS_1_Pin);
					LL_GPIO_ResetOutputPin(LED_STATUS_2_GPIO_Port, LED_STATUS_2_Pin);
				}
			}
			else
			{
				LL_GPIO_ResetOutputPin(LED_STATUS_1_GPIO_Port, LED_STATUS_1_Pin);
				LL_GPIO_ResetOutputPin(LED_STATUS_2_GPIO_Port, LED_STATUS_2_Pin);
			}

			Serial_PutInt(servo->startpoint);
			Serial_PutString(", ");
			Serial_PutInt(servo->endpoint);
			Serial_PutString(", ");
			Serial_PutInt(position);
			Serial_PutString(",     ");
			Serial_PutInt(servo->position_percentage);
			Serial_PutString(", ");
			Serial_PutInt(servo->position);
			Serial_PutString(",     ");
			Serial_PutInt(servo->target_position);
			Serial_PutString(", ");
			Serial_PutInt(servo->target_percentage);
			Serial_PutString(", ");
			Serial_PutInt(*node.channels[2].channel.adc16.analog_in);
			Serial_PrintString(",     ");

			//Serial_PutInt(AS5x47_GetAngle(BLMB_POSITION_ENCODER));
			//Serial_PutString(", ");
			//Serial_PutInt(AS5x47_GetAngle(BLMB_MOTOR_ENCODER));
			//Serial_PutString(", ");
			//Serial_PutInt(tmc4671_readInt(TMC4671_ABN_DECODER_COUNT));
			//Serial_PrintInt(tmc4671_readInt(TMC4671_PID_POSITION_ACTUAL));
			//Serial_PutString(", ");
			//Serial_PrintHex(as5147_getAngle(BLMB_POSITION_ENCODER));
			//Serial_PutHex(tmc6200_readRegister(TMC6200_GSTAT));

//			Serial_PrintInt(tick);
			//Serial_PrintInt(LL_GPIO_IsInputPinSet(STATUS_GPIO_Port, STATUS_Pin));
			// Serial_PrintInt(LL_GPIO_IsInputPinSet(FAULT_GPIO_Port, FAULT_Pin));
			/*
			 Serial_PutInt(tmc4671_readInt(TMC4671_ABN_DECODER_COUNT));
			 Serial_PutString(", ");
			 Serial_PutInt(tmc4671_readInt(TMC4671_PID_POSITION_ACTUAL));
			 Serial_PutString(", ");
			 Serial_PutInt(AS5x47_GetAngle(BLMB_MOTOR_ENCODER));
			 Serial_PutString(", ");*/

		}
		if (Serial_CheckInput(serial_str))
		{
			Serial_PrintString(serial_str);
			uint32_t input = atoi(serial_str);
			/*			if(input == 0)
			 TMC4671_highLevel_pwmOff();
			 else
			 TMC4671_highLevel_positionMode2();*/
			Servo_SetPosition(servo, input);

			/*
			SetMsg_t set_msg = { 0 };
			set_msg.variable_id = SERVO_MAX_TORQUE;
			set_msg.value = 10000;
			Servo_ProcessMessage(0, SERVO_REQ_SET_VARIABLE, (uint8_t*)&set_msg, 0);
			*/

		}
	}
}

#endif
