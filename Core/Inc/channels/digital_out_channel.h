#ifndef DIGITAL_OUT_CHANNEL_H_
#define DIGITAL_OUT_CHANNEL_H_

#include "digital_out_channel_def.h"
#include "measurement.h"

typedef struct
{
	Measurement * measurement;
	int32_t state;
	int32_t mode;
	//GPIO_Pin_t * pin;
}DigitalOut_Channel_t;

#endif
