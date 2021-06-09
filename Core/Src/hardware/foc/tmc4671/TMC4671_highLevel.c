#include "foc/tmc4671/TMC4671_highLevel.h"
#include "foc/tmc4671/TMC4671.h"
#include "foc/swdriver.h"
#include "foc/as5147.h"
#include "systick.h"
#include "serial.h"
#include <stdio.h>

//TODO: use functions from TMC4671.c, use masking
void TMC4671_highLevel_init(void)
{
	tmc4671_writeInt(TMC4671_MODE_RAMP_MODE_MOTION, 0); // stopped_mode

	// Motor type &  PWM configuration
	tmc4671_writeInt(TMC4671_MOTOR_TYPE_N_POLE_PAIRS, (3 << TMC4671_MOTOR_TYPE_SHIFT) | (POLE_PAIRS << TMC4671_N_POLE_PAIRS_SHIFT)); // BLDC, 7 pole pairs
	tmc4671_writeInt(TMC4671_PWM_POLARITIES, 0); // LS and HS polarity off
	tmc4671_writeInt(TMC4671_PWM_MAXCNT, 3999); // 3999 --> 25kHz PWM
	tmc4671_writeInt(TMC4671_PWM_BBM_H_BBM_L, (30 << TMC4671_PWM_BBM_H_SHIFT) | (30 << TMC4671_PWM_BBM_L_SHIFT)); // LS and HS 300ns BBM
	tmc4671_writeInt(TMC4671_PWM_SV_CHOP, (0 << TMC4671_PWM_SV_SHIFT) | (7 << TMC4671_PWM_CHOP_SHIFT)); // Space Vector PWM disabled, centered PWM for FOC

	// ADC configuration
	tmc4671_writeInt(TMC4671_ADC_I_SELECT, 0x24000100); // adcs for current measurement, default assignment (ux=0 v=1 wy=2)
//	tmc4671_writeInt(TMC4671_dsADC_MCFG_B_MCFG_A, 0); // internal ds-modulator, MCLK for both groups FIXME make this work
	tmc4671_writeInt(TMC4671_dsADC_MCFG_B_MCFG_A, 0x00100010); // internal ds-modulator, CLK (100MHz) for both groups FIXME bits might actually be inverted
	tmc4671_writeInt(TMC4671_dsADC_MCLK_A, (1 << 29)); // group a clock frequency 25MHz
	tmc4671_writeInt(TMC4671_dsADC_MCLK_B, 0); // group b clock frequency 0 --> off
	tmc4671_writeInt(TMC4671_dsADC_MDEC_B_MDEC_A, (1000 << TMC4671_DSADC_MDEC_B_SHIFT) | (1000 << TMC4671_DSADC_MDEC_A_SHIFT)); // decimation ratio FIXME adapt to clock

	Systick_BusyWait(100);

	uint16_t i;
	uint32_t adcOffs0 = 0;
	uint32_t adcOffs1 = 0;
	for (i = 0; i < 256; i++)
	{
		adcOffs0 += TMC4671_getAdcRaw0();
		adcOffs1 += TMC4671_getAdcRaw1();
		Systick_BusyWait(1);
	}
	adcOffs0 /= 256;
	adcOffs1 /= 256;

	tmc4671_writeInt(TMC4671_ADC_I0_SCALE_OFFSET, (-490 << TMC4671_ADC_I0_SCALE_SHIFT) | (adcOffs0 << TMC4671_ADC_I0_OFFSET_SHIFT)); // offset, scale 2mA/lsb
	tmc4671_writeInt(TMC4671_ADC_I1_SCALE_OFFSET, (-490 << TMC4671_ADC_I1_SCALE_SHIFT) | (adcOffs1 << TMC4671_ADC_I1_OFFSET_SHIFT)); // offset, scale 2mA/lsb

	// ABN encoder settings
	tmc4671_writeInt(TMC4671_ABN_DECODER_MODE, 0); // standard polarity and count direction, don't clear at n pulse
	tmc4671_writeInt(TMC4671_ABN_DECODER_PPR, 2048); // decoder pulses per revolution
	tmc4671_writeInt(TMC4671_ABN_DECODER_COUNT, 0); // decoder angle 0 FIXME: writing anything else doesn't work but writing current angle would allow for more elegant solution. 3 lines below could be deleted, see git history
	//uint16_t angle_current = (as5147_getAngle(drv) << 5);  // current decoder angle
	//swdriver[drv].ofs_enc_phim += angle_current;
	//tmc4671_writeInt(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, ((swdriver[drv].ofs_phim_phie << TMC4671_ABN_DECODER_PHI_E_OFFSET_SHIFT) & 0xFFFF0000) | ((swdriver[drv].ofs_enc_phim << TMC4671_ABN_DECODER_PHI_M_OFFSET_SHIFT) & 0x0000FFFF));
	//tmc4671_writeInt(TMC4671_PID_POSITION_ACTUAL, (int32_t)swdriver[drv].ofs_enc_phim); // set position to current position
	//tmc4671_writeInt(TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, ((0 << TMC4671_ABN_DECODER_PHI_E_OFFSET_SHIFT) & 0xFFFF0000) | ((0 << TMC4671_ABN_DECODER_PHI_M_OFFSET_SHIFT) & 0x0000FFFF));
	//tmc4671_writeInt(TMC4671_PID_POSITION_ACTUAL, (int32_t)0); // set position to current position
	tmc4671_writeInt(TMC4671_PID_POSITION_ACTUAL, 0);

	// Feedback selection
	tmc4671_writeInt(TMC4671_PHI_E_SELECTION, 3); // phi_e_abn
	tmc4671_writeInt(TMC4671_VELOCITY_SELECTION, (9 << TMC4671_VELOCITY_SELECTION_SHIFT) | (1 << TMC4671_VELOCITY_METER_SELECTION_SHIFT)); // phi_m_abn, advanced velocity meter (time difference measurement)
	tmc4671_writeInt(TMC4671_POSITION_SELECTION, 9); // phi_m_abn

	// Limits
	tmc4671_writeInt(TMC4671_PIDOUT_UQ_UD_LIMITS, 23169); // UQ/UD limit TODO optimize
	tmc4671_writeInt(TMC4671_PID_TORQUE_FLUX_TARGET_DDT_LIMITS, 32767); // torque/flux ddt limit TODO optimize
	tmc4671_writeInt(TMC4671_PID_TORQUE_FLUX_LIMITS, 10000); // torque/flux limit 20A TODO optimize
	tmc4671_writeInt(TMC4671_PID_ACCELERATION_LIMIT, 10000); // acceleration limit TODO optimize
	tmc4671_writeInt(TMC4671_PID_VELOCITY_LIMIT, 10000); // velocity limit TODO optimize
	tmc4671_writeInt(TMC4671_PID_POSITION_LIMIT_LOW, -655335); // position lower limit -1 turn TODO optimize
	tmc4671_writeInt(TMC4671_PID_POSITION_LIMIT_HIGH, 2359260); // position upper limit, 36 turns  TODO optimize

	// PI settings
	tmc4671_writeInt(TMC4671_PID_FLUX_P_FLUX_I, (100 << TMC4671_PID_FLUX_P_SHIFT) | (1000 << TMC4671_PID_FLUX_I_SHIFT)); // flux PI TODO optimize
	tmc4671_writeInt(TMC4671_PID_TORQUE_P_TORQUE_I, (100 << TMC4671_PID_TORQUE_P_SHIFT) | (1000 << TMC4671_PID_TORQUE_I_SHIFT)); // torque PI TODO optimize
	tmc4671_writeInt(TMC4671_PID_VELOCITY_P_VELOCITY_I, (5000 << TMC4671_PID_VELOCITY_P_SHIFT) | (10000 << TMC4671_PID_VELOCITY_I_SHIFT)); // velocity PI TODO optimize
	tmc4671_writeInt(TMC4671_PID_POSITION_P_POSITION_I, (500 << TMC4671_PID_POSITION_P_SHIFT) | (0 << TMC4671_PID_POSITION_I_SHIFT)); // position PI TODO optimize

	// // Actual Velocity Biquad settings (lowpass 2nd order, f=500, d=1.0)
	// tmc4671_writeInt(TMC4671_CONFIG_ADDR, 9); // biquad_v_a_1
	// tmc4671_writeInt(TMC4671_CONFIG_DATA, 946788180);
	// tmc4671_writeInt(TMC4671_CONFIG_ADDR, 10); // biquad_v_a_2
	// tmc4671_writeInt(TMC4671_CONFIG_DATA, -417422437);
	// tmc4671_writeInt(TMC4671_CONFIG_ADDR, 12); // biquad_v_b_0
	// tmc4671_writeInt(TMC4671_CONFIG_DATA, 1876292);
	// tmc4671_writeInt(TMC4671_CONFIG_ADDR, 13); // biquad_v_b_1
	// tmc4671_writeInt(TMC4671_CONFIG_DATA, 3752585);
	// tmc4671_writeInt(TMC4671_CONFIG_ADDR, 14); // biquad_v_b_2
	// tmc4671_writeInt(TMC4671_CONFIG_DATA, 1876292);
	// tmc4671_writeInt(TMC4671_CONFIG_ADDR, 15); // biquad_v_enable
	// tmc4671_writeInt(TMC4671_CONFIG_DATA, 1);
	// tmc4671_writeInt(TMC4671_CONFIG_ADDR, 0); //none

	// Actual Velocity Biquad settings (lowpass 2nd order, f=200, d=1.0)
	tmc4671_writeInt(TMC4671_CONFIG_ADDR, 9); // biquad_v_a_1
	tmc4671_writeInt(TMC4671_CONFIG_DATA, 1021092885);
	tmc4671_writeInt(TMC4671_CONFIG_ADDR, 10); // biquad_v_a_2
	tmc4671_writeInt(TMC4671_CONFIG_DATA, -485512745);
	tmc4671_writeInt(TMC4671_CONFIG_ADDR, 12); // biquad_v_b_0
	tmc4671_writeInt(TMC4671_CONFIG_DATA, 322693);
	tmc4671_writeInt(TMC4671_CONFIG_ADDR, 13); // biquad_v_b_1
	tmc4671_writeInt(TMC4671_CONFIG_DATA, 645386);
	tmc4671_writeInt(TMC4671_CONFIG_ADDR, 14); // biquad_v_b_2
	tmc4671_writeInt(TMC4671_CONFIG_DATA, 322693);
	tmc4671_writeInt(TMC4671_CONFIG_ADDR, 15); // biquad_v_enable
	tmc4671_writeInt(TMC4671_CONFIG_DATA, 1);
	tmc4671_writeInt(TMC4671_CONFIG_ADDR, 0); //none

	// // Target Position Biquad settings (lowpass 2nd order, f=20, d=1.0)
	// tmc4671_writeInt( TMC4671_CONFIG_ADDR, 1); // biquad_x_a_1
	// tmc4671_writeInt( TMC4671_CONFIG_DATA, 1068358140);
	// tmc4671_writeInt( TMC4671_CONFIG_ADDR, 2); // biquad_x_a_2
	// tmc4671_writeInt( TMC4671_CONFIG_DATA, -531500724);
	// tmc4671_writeInt( TMC4671_CONFIG_ADDR, 4); // biquad_x_b_0
	// tmc4671_writeInt( TMC4671_CONFIG_DATA, 3374);
	// tmc4671_writeInt( TMC4671_CONFIG_ADDR, 5); // biquad_x_b_1
	// tmc4671_writeInt( TMC4671_CONFIG_DATA, 6748);
	// tmc4671_writeInt( TMC4671_CONFIG_ADDR, 6); // biquad_x_b_2
	// tmc4671_writeInt( TMC4671_CONFIG_DATA, 3374);
	// tmc4671_writeInt( TMC4671_CONFIG_ADDR, 7); // biquad_x_enable
	// tmc4671_writeInt( TMC4671_CONFIG_DATA, 1);
	// tmc4671_writeInt( TMC4671_CONFIG_ADDR, 0); //none

	// Target Position Biquad settings (lowpass 2nd order, f=100, d=1.0)
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 1); // biquad_x_a_1
	tmc4671_writeInt( TMC4671_CONFIG_DATA, 1047090657);
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 2); // biquad_x_a_2
	tmc4671_writeInt( TMC4671_CONFIG_DATA, -510550497);
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 4); // biquad_x_b_0
	tmc4671_writeInt( TMC4671_CONFIG_DATA, 82688);
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 5); // biquad_x_b_1
	tmc4671_writeInt( TMC4671_CONFIG_DATA, 165376);
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 6); // biquad_x_b_2
	tmc4671_writeInt( TMC4671_CONFIG_DATA, 82688);
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 7); // biquad_x_enable
	tmc4671_writeInt( TMC4671_CONFIG_DATA, 1);
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 0); //none
}

void TMC4671_highLevel_pwmOff(void)
{
	tmc4671_writeInt( TMC4671_PWM_SV_CHOP, 0x00000000); // PWM off
}

void TMC4671_highLevel_stoppedMode(void)
{
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 0); // stopped_mode
}

void TMC4671_highLevel_positionMode(void) 	// Switch to position mode
{
	tmc4671_writeInt( TMC4671_PID_POSITION_TARGET, 0); // target position 0
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 3); // position_mode
}

void TMC4671_highLevel_positionMode2(void)
{
	// Switch to position mode without setting target position to 0
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 3); // position_mode
}

void TMC4671_highLevel_setPosition(int32_t position)
{
	tmc4671_writeInt( TMC4671_PID_POSITION_TARGET, position); // position target
}
/*
void TMC4671_highLevel_setPosition_nonBlocking(int32_t position)
{
	tmc4671_writeInt_nonBlocking( TMC4671_PID_POSITION_TARGET, position); // position target
}*/

void TMC4671_highLevel_printOffsetAngle(void)
{
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 8); // uq_ud_ext
	tmc4671_writeInt( TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);
	tmc4671_writeInt( TMC4671_PHI_E_SELECTION, 1);  // phi_e_ext
	tmc4671_writeInt( TMC4671_PHI_E_EXT, 0);
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (2000 << TMC4671_UD_EXT_SHIFT)); // uq=0, ud=2000
	Systick_BusyWait(1000);

	char string[64];
	// uint16_t angle = as5147_getAngle(drv); // will be printed as int16!!!
	// uint16_t len = snprintf(string, 64, "\n\rdriver %d encoder zero angle: %d (11bit) %d (16bit)\n\r",  angle, (int16_t)(angle << 5));
	uint16_t angle = as5147_getAngle(0); // will be printed as int16!!!
	snprintf(string, 64, "\n\rencoder zero angle: %d (16bit)\n\r", angle);
	Serial_PutString(string);
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (0 << TMC4671_UD_EXT_SHIFT)); // ud=0 uq=0
}

uint16_t TMC4671_getAdcRaw0(void)
{
	tmc4671_writeInt( TMC4671_ADC_RAW_ADDR, 0);
	return tmc4671_readInt( TMC4671_ADC_RAW_DATA) & 0xFFFF;
}

uint16_t TMC4671_getAdcRaw1(void)
{
	tmc4671_writeInt( TMC4671_ADC_RAW_ADDR, 0);
	return (tmc4671_readInt( TMC4671_ADC_RAW_DATA) >> 16) & 0xFFFF;
}

void TMC4671_highLevel_initEncoder(void)
{
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 8); // uq_ud_ext
	tmc4671_writeInt( TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);
	tmc4671_writeInt( TMC4671_PHI_E_SELECTION, 1);  // phi_e_ext
	tmc4671_writeInt( TMC4671_PHI_E_EXT, 0);
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (2000 << TMC4671_UD_EXT_SHIFT)); // uq=0, ud=2000
	Systick_BusyWait(1000);
	tmc4671_writeInt( TMC4671_ABN_DECODER_COUNT, 0);
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (0 << TMC4671_UD_EXT_SHIFT)); // ud=0 uq=0
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 0); // off
	tmc4671_writeInt( TMC4671_PHI_E_SELECTION, 3); // phi_e_abn
}

void TMC4671_highLevel_initEncoder_new(void)
{
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 8);
	tmc4671_writeInt( TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0);
	tmc4671_writeInt( TMC4671_PHI_E_SELECTION, 1);
	tmc4671_writeInt( TMC4671_PHI_E_EXT, 0);
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (2500 << TMC4671_UD_EXT_SHIFT));
	Systick_BusyWait(2000);
	uint16_t angle = as5147_getAngle(0);
	tmc4671_writeInt( TMC4671_ABN_DECODER_COUNT, 0);
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (0 << TMC4671_UD_EXT_SHIFT)); // ud=0 uq=0
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 0);

	int32_t position = (int32_t) ((int16_t) ((angle) - swdriver.ofs_pos0));
	// static char string[128];
	// uint16_t len = snprintf(string, 128, "-->driver %d encoder angle: %d (11bit) %d (16bit) %ld\n\r",  angle, (angle << 5), position );
	// HAL_UART_Transmit_IT(&huart3, (uint8_t*)string, len);
	tmc4671_writeInt( TMC4671_PHI_E_SELECTION, 3); // phi_e_abn
	tmc4671_writeInt( TMC4671_PID_POSITION_ACTUAL, position);
}

void TMC4671_highLevel_positionMode_fluxTorqueRamp(void) // TODO read actual position before torque ramp, ramp position from actual to 0 afterwards
{
	uint16_t torque_flux_limit = 5000;
	uint16_t torque_flux = 0;
	uint8_t i;

	torque_flux_limit = tmc4671_readRegister16BitValue( TMC4671_PID_TORQUE_FLUX_LIMITS, BIT_0_TO_15);

	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_LIMITS, 0);
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 3); // position_mode

	for (i = 0; i < 100; i++)
	{
		torque_flux = (uint16_t) ((float) i / 100.0 * (float) torque_flux_limit);
		tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_LIMITS, torque_flux);
		Systick_BusyWait(1);
	}
	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_LIMITS, torque_flux_limit);
}

void TMC4671_highLevel_positionMode_rampToZero(void)
{
	uint8_t i;
	int32_t position = tmc4671_readInt( TMC4671_PID_POSITION_ACTUAL);

	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 3);

	for (i = 100; i > 0; i--)
	{
		//TMC4671_highLevel_setPosition( position);
		tmc4671_writeInt( TMC4671_PID_POSITION_TARGET, (int32_t) ((float) position * (float) i / 100.0));
		Systick_BusyWait(10);
	}
	tmc4671_writeInt( TMC4671_PID_POSITION_TARGET, 0);
}

void TMC4671_highLevel_setPositionFilter(bool status)
{
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 7);
	//bool enabled = (bool)tmc4671_readInt( TMC4671_CONFIG_DATA);
	tmc4671_writeInt( TMC4671_CONFIG_DATA, status);
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 0);
}

void TMC4671_highLevel_setCurrentLimit(uint16_t torque_flux_limit)
{
	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_LIMITS, torque_flux_limit);
}

void TMC4671_highLevel_setIntegralPosition(uint16_t integral)
{
	tmc4671_writeRegister16BitValue( TMC4671_PID_POSITION_P_POSITION_I, BIT_0_TO_15, integral);
}

int32_t TMC4671_highLevel_getPositionTarget(void)
{
	return tmc4671_readInt( TMC4671_PID_POSITION_TARGET);
}

int32_t TMC4671_highLevel_getPositionActual(void)
{
	return tmc4671_readInt( TMC4671_PID_POSITION_ACTUAL);
}

int16_t TMC4671_highLevel_getTorqueActual(void)
{
	return tmc4671_readRegister16BitValue(TMC4671_PID_TORQUE_FLUX_ACTUAL, BIT_16_TO_31);
}

int16_t TMC4671_highLevel_getVelocityActual(void)
{
	// tmc4671_writeInt( TMC4671_INTERIM_ADDR, 24);
	// INTERIM_ADDR
	//  INTERIM_DATA
	// return tmc4671_readRegister16BitValue(TMC4671_PID_VELOCITY_ACTUAL_LSB, BIT_0_TO_15);
	return (int16_t) tmc4671_readInt( TMC4671_PID_VELOCITY_ACTUAL);
}

int16_t TMC4671_highLevel_getTorqueTarget(void)
{
	// INTERIM_ADDR = 0 => PIDIN_TARGET_TORQUE
	tmc4671_writeInt( TMC4671_INTERIM_ADDR, 0);
	return (int16_t) tmc4671_readInt( TMC4671_INTERIM_DATA);
}

int16_t TMC4671_highLevel_getVelocityTarget(void)
{
	// INTERIM_ADDR = 2 => PIDIN_TARGET_VELOCITY
	tmc4671_writeInt( TMC4671_INTERIM_ADDR, 2);
	return (int16_t) tmc4671_readInt( TMC4671_INTERIM_DATA);
}

char* TMC4671_highLevel_getStatus(void)
{
	static char string[200];
	tmc4671_writeInt( TMC4671_CONFIG_ADDR, 7);
	bool enabled = (bool) tmc4671_readInt( TMC4671_CONFIG_DATA);
	uint16_t torque_flux_limit = tmc4671_readRegister16BitValue( TMC4671_PID_TORQUE_FLUX_LIMITS, BIT_0_TO_15);
	uint16_t position_i = tmc4671_readRegister16BitValue( TMC4671_PID_POSITION_P_POSITION_I, BIT_0_TO_15);
	snprintf(&string[0], 200, "Position-Filter [f]: %s\r\n"
			"Torque-Limit    [c]: %d\r\n"
			"Position-I:     [i]: %d\r\n"
			"I0:                  %d\n\r"
			"I1:                  %d\n\r"
			"---------------------------\n\r", enabled ? "on" : "off", torque_flux_limit, position_i, tmc4671_getAdcI0Offset(), tmc4671_getAdcI1Offset());

	return &string[0];
}

void TMC4671_highLevel_positionTest(void)
{
	// Switch to position mode
	tmc4671_writeInt( TMC4671_PID_POSITION_TARGET, 0); // position target 0
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 3); // position_mode

	// move
	uint8_t i;
	for (i = 0; i < 4; i++)
	{
		tmc4671_writeInt( TMC4671_PID_POSITION_TARGET, 65535); // position target 65535
		Systick_BusyWait(2000);
		tmc4671_writeInt( TMC4671_PID_POSITION_TARGET, 0); // position target 0
		Systick_BusyWait(2000);
	}

	// Stop
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 0); // stopped_mode
}

void TMC4671_highLevel_velocityTest(void)
{
	// Switch to velocity mode
	tmc4671_writeInt( TMC4671_PID_VELOCITY_TARGET, (0 << TMC4671_PID_VELOCITY_TARGET_SHIFT)); // velocity target 0
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 2); // velocity_mode

	// Rotate right
	tmc4671_writeInt( TMC4671_PID_VELOCITY_TARGET, (60 << TMC4671_PID_VELOCITY_TARGET_SHIFT)); // velocity target
	Systick_BusyWait(5000);

	// Rotate left
	tmc4671_writeInt( TMC4671_PID_VELOCITY_TARGET, (-60 << TMC4671_PID_VELOCITY_TARGET_SHIFT)); // velocity target
	Systick_BusyWait(5000);

	// Stop
	tmc4671_writeInt( TMC4671_PID_VELOCITY_TARGET, (0 << TMC4671_PID_VELOCITY_TARGET_SHIFT)); // velocity target 0
}

void TMC4671_highLevel_torqueTest(void)
{
	// Switch to torque mode
	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_TARGET, (0 << TMC4671_PID_TORQUE_TARGET_SHIFT)); // torque target 0
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 1); // torque_mode

	// Rotate right
	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_TARGET, (200 << TMC4671_PID_TORQUE_TARGET_SHIFT)); // torque target 200 (400mA)
	Systick_BusyWait(3000);

	// Rotate left
	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_TARGET, (-200 << TMC4671_PID_TORQUE_TARGET_SHIFT)); // torque target -200 (-400mA)
	Systick_BusyWait(3000);

	// Stop
	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_TARGET, (0 << TMC4671_PID_TORQUE_TARGET_SHIFT)); // torque target 0
}

void TMC4671_highLevel_openLoopTest(void)
{
	// Open loop settings
	tmc4671_writeInt( TMC4671_OPENLOOP_MODE, 0x00000000); // openloop phi positive
	tmc4671_writeInt( TMC4671_OPENLOOP_ACCELERATION, 60); // openloop phi acceleration
	tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 0); // velocity target 0

	// Feedback selection
	tmc4671_writeInt( TMC4671_PHI_E_SELECTION, 2); // phi_e_openloop
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (2000 << TMC4671_UD_EXT_SHIFT)); // uq=0, ud=2000

	// ===== Open loop test drive =====

	// Switch to open loop velocity mode
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 8); // uq_ud_ext

	// Rotate right
	tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 1); // velocity target 1
	Systick_BusyWait(20000);

	// Rotate left
	//tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, -1); // velocity target -1
	//Systick_BusyWait(5000);

	// Stop
	tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 0); // velocity target 0
	Systick_BusyWait(2000);
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (0 << TMC4671_UD_EXT_SHIFT)); // ud=0 uq=0
}

void TMC4671_highLevel_openLoopTest2(void) // to verify correct encoder initialisation
{
	// Open loop settings
	tmc4671_writeInt( TMC4671_OPENLOOP_MODE, 0x00000000); // openloop phi positive
	tmc4671_writeInt( TMC4671_OPENLOOP_ACCELERATION, 60); // openloop phi acceleration
	tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 0); // velocity target 0

	// Feedback selection
	tmc4671_writeInt( TMC4671_PHI_E_SELECTION, 2); // phi_e_openloop
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (3000 << TMC4671_UD_EXT_SHIFT)); // uq=0, ud=3000

	// Switch to open loop velocity mode
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 8); // uq_ud_ext
	tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 10); // rotate right, velocity target 1
}

void TMC4671_highLevel_openLoopTest3(void) // low duty cycle operation for high current without overheating
{
	// Open loop settings
	tmc4671_writeInt( TMC4671_OPENLOOP_MODE, 0x00000000); // openloop phi positive
	tmc4671_writeInt( TMC4671_OPENLOOP_ACCELERATION, 60); // openloop phi acceleration
	tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 0); // velocity target 0

	// Feedback selection
	tmc4671_writeInt( TMC4671_PHI_E_SELECTION, 2); // phi_e_openloop
	tmc4671_writeInt( TMC4671_UQ_UD_EXT, (0 << TMC4671_UQ_EXT_SHIFT) | (5000 << TMC4671_UD_EXT_SHIFT)); // uq=0, ud=5000 --> ~40-50A peak phase @ 20Vin

	// Switch to open loop velocity mode
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 8); // uq_ud_ext
	tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 1); // rotate right, velocity target 1

	while (1)
	{
		Systick_BusyWait(10000);
		tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 0); // stopped
		tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 0); // velocity target 0
		Systick_BusyWait(50000);
		tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 8); // uq_ud_ext
		tmc4671_writeInt( TMC4671_OPENLOOP_VELOCITY_TARGET, 1); // rotate right, velocity target 1
	}
}

void TMC4671_highLevel_referenceEndStop(void)
{
	//lower torque limit
	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_LIMITS, 1000); // torque/flux limit 2A

	// Switch to velocity mode
	tmc4671_writeInt( TMC4671_PID_VELOCITY_TARGET, (0 << TMC4671_PID_VELOCITY_TARGET_SHIFT)); // velocity target 0
	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 2); // velocity_mode

	// drive into end stop
	tmc4671_writeInt( TMC4671_PID_VELOCITY_TARGET, (-600 << TMC4671_PID_VELOCITY_TARGET_SHIFT)); // velocity target

	//wait until stopped
	int32_t pos_last;
	do
	{
		pos_last = TMC4671_highLevel_getPositionActual();
		Systick_BusyWait(100);
	} while (TMC4671_highLevel_getPositionActual() - pos_last < -10000); //average much less than half of the target speed over the last 100ms

	tmc4671_writeInt( TMC4671_PID_POSITION_ACTUAL, -131070); //2 rotations

	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 0); //stopped_mode

	tmc4671_writeInt( TMC4671_PID_TORQUE_FLUX_LIMITS, 10000); // reset limit TODO: use old value

	tmc4671_writeInt( TMC4671_PID_POSITION_TARGET, 0);

	tmc4671_writeInt( TMC4671_MODE_RAMP_MODE_MOTION, 3); // position_mode
}
