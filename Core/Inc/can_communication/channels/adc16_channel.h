#ifndef ADC16_CHANNEL_H__
#define ADC16_CHANNEL_H__

#include "cmds.h"
#include "measurement.h"
typedef enum
{
	ADC16_RESET_SETTINGS,
	ADC16_SAMPLINGRATE, //0 means disabled (not sampling at all)
	ADC16_MODE, 		//digital in or analog in (maybe more settings)
	ADC16_SET_OFFSET, 	//maybe like this or tare or both ??? (maybe set offset = 0xFFFF means tare)
	//other stuff



	ADC16_TOTAL_CMDS
} ADC16_CMDs;

extern const can_function adc16_array[];


typedef struct
{
	uint8_t channel_index; //ToDo: Probably unnecessary
	int_least32_t offset;
	int_least32_t thresholds[2];

	Measurement * measurement;

}Adc16_t;

Result_t ADC16_getData(void *channel, uint8_t *array, uint8_t *length);
uint32_t ADC16_getStatus(void *channel);







#endif
