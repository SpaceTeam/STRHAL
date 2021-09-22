#ifndef PNEUMATIC_VALVE_CHANNEL_H_
#define PNEUMATIC_VALVE_CHANNEL_H_

#include "pneumatic_valve_channel_def.h"
#include "main.h"

#define PNEUMATIC_VALVE_DEFAULT_THRESHOLD  1000
#define PNEUMATIC_VALVE_DEFAULT_HYSTERESIS 200
//#define PNEUMATIC_VALVE_DEFAULT_STARTPOINT 11500
//#define PNEUMATIC_VALVE_DEFAULT_ENDPOINT   35000

//#define PNEUMATIC_VALVE_DEFAULT_STARTPOINT 31400
//#define PNEUMATIC_VALVE_DEFAULT_ENDPOINT 54000
#define PNEUMATIC_VALVE_DEFAULT_STARTPOINT 54000
#define PNEUMATIC_VALVE_DEFAULT_ENDPOINT 31400

#define PNEUMATIC_MAX_ERRORS 1000
typedef struct
{
		uint32_t enable;
		uint32_t target_percentage;
		uint32_t target_position;
		uint32_t position_percentage;
		uint32_t threshold;
		uint32_t hysteresis;
		uint32_t refresh_divider;
		uint32_t on_channel_id;
		uint32_t off_channel_id;
		uint32_t pos_channel_id;
		uint32_t startpoint;
		uint32_t endpoint;
		uint32_t error_counter;
}PneumaticValve_Channel_t;


Result_t PneumaticValve_InitChannel(PneumaticValve_Channel_t *valve, uint32_t on_channel_id, uint32_t off_channel_id, uint32_t pos_channel_id);
uint32_t* PneumaticValve_VariableSelection(PneumaticValve_Channel_t *pneumatic_valve, uint8_t var_id, uint8_t ch_id);
Result_t PneumaticValve_ProcessMessage(uint8_t ch_id, uint8_t cmd_id, uint8_t *data, uint32_t length);
Result_t PneumaticValve_GetData(uint8_t ch_id, uint8_t *data, uint32_t *length);

Result_t PneumaticValve_Update(PneumaticValve_Channel_t *valve);


#endif
