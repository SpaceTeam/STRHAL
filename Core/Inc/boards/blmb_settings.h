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
}BLMB_Settings_t;


void BlmbSettings_Load(BLMB_Settings_t * settings);
void BlmbSettings_Store(BLMB_Settings_t * settings);

#endif
