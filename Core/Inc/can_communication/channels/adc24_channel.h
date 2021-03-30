#ifndef ADC24_CHANNEL_H__
#define ADC24_CHANNEL_H__

#include "cmds.h"
#include "measurement.h"

typedef enum
{
	ADC24_RESET_SETTINGS,



	ADC24_TOTAL_CMDS
} ADC24_CMDs;

extern const can_function adc24_array[];


typedef struct
{
	uint8_t channel_index; //ToDo: Probably unnecessary
	int_least32_t offset;
	int_least32_t thresholds[2];

	Measurement * measurement;

}Adc24_t;

Result_t ADC24_getData(void *channel, uint8_t *array, uint8_t *length);
uint32_t ADC24_getStatus(void *channel);













#endif
