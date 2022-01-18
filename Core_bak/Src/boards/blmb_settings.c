#include "blmb_settings.h"
#include "flash.h"
#include "main.h"

#define BLMB_SETTINGS_STATE_ADDRESS 0
#define BLMB_SETTINGS_FLASH_ADDRESS 1


#define BLMB_DEFAULT_STARTPOINT					0
#define BLMB_DEFAULT_ENDPOINT 					UINT16_MAX
#define BLMB_DEFAULT_MAX_ACCEL 					100
#define BLMB_DEFAULT_MAX_SPEED 					5000
#define BLMB_DEFAULT_MAX_TORQ 					10000
#define BLMB_DEFAULT_PWM_IN_ENABLED				1
#define BLMB_DEFAULT_PRESSURE_CONTROL_ENABLED	1
#define BLMB_DEFAULT_P_PARAM 					1
#define BLMB_DEFAULT_I_PARAM 					1
#define BLMB_DEFAULT_D_PARAM 					1
#define BLMB_DEFAULT_POS_P_PARAM				200
#define BLMB_DEFAULT_POS_I_PARAM				0
#define BLMB_DEFAULT_VEL_P_PARAM				2000
#define BLMB_DEFAULT_VEL_I_PARAM				4000
#define BLMB_DEFAULT_TORQ_P_PARAM				100
#define BLMB_DEFAULT_TORQ_I_PARAM				1000
#define BLMB_DEFAULT_PRESSURE_HYSTERESIS		10

void BlmbSettings_StoreDefault(void)
{
	BLMB_Settings_t default_settings =
	{ 0 };
	default_settings.startpoint = BLMB_DEFAULT_STARTPOINT;
	default_settings.endpoint = BLMB_DEFAULT_ENDPOINT;
	default_settings.max_accel = BLMB_DEFAULT_MAX_ACCEL;
	default_settings.max_speed = BLMB_DEFAULT_MAX_SPEED;
	default_settings.max_torque = BLMB_DEFAULT_MAX_TORQ;
	default_settings.pwm_in_enabled = BLMB_DEFAULT_PWM_IN_ENABLED;
	default_settings.pressure_control_enabled = BLMB_DEFAULT_PRESSURE_CONTROL_ENABLED;
	default_settings.p_param = BLMB_DEFAULT_P_PARAM;
	default_settings.i_param = BLMB_DEFAULT_I_PARAM;
	default_settings.d_param = BLMB_DEFAULT_D_PARAM;
	default_settings.pos_p_param = BLMB_DEFAULT_POS_P_PARAM;
	default_settings.pos_i_param = BLMB_DEFAULT_POS_I_PARAM;
	default_settings.vel_p_param = BLMB_DEFAULT_VEL_P_PARAM;
	default_settings.vel_i_param = BLMB_DEFAULT_VEL_I_PARAM;
	default_settings.torq_p_param = BLMB_DEFAULT_TORQ_P_PARAM;
	default_settings.torq_i_param = BLMB_DEFAULT_TORQ_I_PARAM;
	default_settings.pressure_hysteresis = BLMB_DEFAULT_PRESSURE_HYSTERESIS;

	BlmbSettings_Store(&default_settings);
}

void BlmbSettings_Load(BLMB_Settings_t *settings)
{
	if (Flash_GetByteFromAddress(BLMB_SETTINGS_STATE_ADDRESS))
		BlmbSettings_StoreDefault();

	Flash_GetBlockStartingFromAddress(BLMB_SETTINGS_FLASH_ADDRESS, (uint8_t*) settings, sizeof(BLMB_Settings_t));

}
void BlmbSettings_Store(BLMB_Settings_t *settings)
{
	Flash_EraseSector(BLMB_SETTINGS_FLASH_ADDRESS);
	Flash_SetBlockStartingAtAddress(BLMB_SETTINGS_FLASH_ADDRESS, (uint8_t*) settings, sizeof(BLMB_Settings_t));
	Flash_SetByteAtAddress(BLMB_SETTINGS_STATE_ADDRESS,0);
}
