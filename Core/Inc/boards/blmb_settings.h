#ifndef BLMB_SETTINGS_H_
#define BLMB_SETTINGS_H_

#include "stdint.h"
typedef struct
{
		uint32_t startpoint;
		uint32_t endpoint;
		uint32_t max_accel;
		uint32_t max_speed;
		uint32_t max_torque;
		uint32_t p_param;
		uint32_t i_param;
		uint32_t d_param;
		uint32_t pwm_in_enabled;
		uint32_t pressure_control_enabled;
		uint32_t pos_p_param;
		uint32_t pos_i_param;
		uint32_t vel_p_param;
		uint32_t vel_i_param;
		uint32_t torq_p_param;
		uint32_t torq_i_param;
		uint32_t pressure_hysteresis;
}BLMB_Settings_t;


void BlmbSettings_Load(BLMB_Settings_t * settings);
void BlmbSettings_Store(BLMB_Settings_t * settings);
void BlmbSettings_StoreDefault(void);

#endif
