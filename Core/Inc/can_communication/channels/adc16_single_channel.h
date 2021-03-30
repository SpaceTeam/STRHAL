#ifndef ADC16_SINGLE_CHANNEL_H__
#define ADC16_SINGLE_CHANNEL_H__

#include "cmds.h"
#include "measurement.h"

typedef enum
{
	ADC16_SINGLE_RESET_SETTINGS,
	ADC16_SINGLE_SAMPLINGRATE,
	ADC16_SINGLE_MODE,

	ADC16_SINGLE_TOTAL_CMDS
} ADC16_SINGLE_CMDs;

extern const can_function adc16_single_array[];

typedef struct
{
	uint8_t channel_index; //ToDo: Probably unnecessary
	int_least32_t offset;
	int_least32_t thresholds[2];

	Measurement * measurement;

}Adc16_single_t;

Result_t ADC16_single_getData(void *channel, uint8_t *array, uint8_t *length);
uint32_t ADC16_single_getStatus(void *channel);












#endif
