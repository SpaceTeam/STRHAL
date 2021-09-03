#ifndef PNEUMATIC_VALVE_CHANNEL_H_
#define PNEUMATIC_VALVE_CHANNEL_H_

#include "pneumatic_valve_def.h"

typedef struct
{
		uint32_t target_position;
		uint32_t p_param;
		uint32_t i_param;
		uint32_t d_param;
		uint32_t refresh_divider;
		uint32_t on_channel_id;
		uint32_t off_channel_id;
		uint32_t pos_channel_id;

}PneumaticValve_Channel_t;


#endif
