#ifndef HARDWARE_ADC_H__
#define HARDWARE_ADC_H__

#include <stdint.h>
#include "stm32h7xx_ll_adc.h"

typedef enum
{
	ANALOG_IN_1,
	ANALOG_IN_2,
	ANALOG_IN_3,
	ANALOG_IN_4,
	ANALOG_IN_5,
	ANALOG_IN_6,
	ANALOG_IN_7,
	ANALOG_IN_8,
	ANALOG_IN_9,
	ANALOG_IN_10,
	ANALOG_IN_11,
	ANALOG_IN_12,
	PWR_BUS_VOLTAGE_1,
	PWR_BUS_VOLTAGE_2,
	SUPPLY_VOLTAGE_SENSE,
	SUPPLY_CURRENT_SENSE,

	ADC_CHANNEL_ID_LAST
} ADC_CHANNEL_ID;

typedef struct
{
	ADC_TypeDef *adc;
	uint32_t channel;
} AdcChannel_t;

typedef volatile uint16_t AdcData_t;

void Adc_Init(void);
AdcData_t* Adc_AddRegularChannel(ADC_CHANNEL_ID id);
void Adc_Calibrate(void);
void Adc_StartAdc(void);

void Adc_StartInjectedChannel(ADC_CHANNEL_ID id);
uint8_t Adc_IsInjectedChannelFinished(void);
uint16_t Adc_GetInjectedChannelData(void);


#endif
