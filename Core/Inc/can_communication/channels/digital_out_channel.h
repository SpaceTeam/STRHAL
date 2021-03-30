#ifndef DIGITAL_OUT_CHANNEL_H__
#define DIGITAL_OUT_CHANNEL_H__

#include "cmds.h"
#include "gpio.h"
#include "measurement.h"

typedef enum
{
	DIGITAL_OUT_RESET_SETTINGS,


	DIGITAL_OUT_TOTAL_CMDS
} DIGITAL_OUT_CMDs;

extern const can_function digital_out_array[];

typedef struct
{
	uint8_t channel_index; //ToDo: Probably unnecessary
	Measurement * measurement;
	uint_least8_t state;
	uint_least8_t mode;
	GPIO_Pin_t * pin;
}Digital_out_t;

Result_t Digital_out_getData(void *channel, uint8_t *array, uint8_t *length);
uint32_t Digital_out_getStatus(void *channel);




#endif
