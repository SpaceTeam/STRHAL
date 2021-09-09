#ifndef THRESHOLD_H_
#define THRESHOLD_H_

#include "main.h"

#define MAX_THRESHOLDS	8

typedef enum
{
	EQUALS = 0,
	LESS_THAN,
	GREATER_THAN
} Operation_t;

typedef struct
{
	uint8_t enabled;
	uint8_t var_id;
	Operation_t compare_id;
	int32_t threshold;
	Result_t result;
	uint8_t or_threshold_id;
	uint8_t and_threshold_id;
} Threshold_t;

Threshold_t channel_thresholds[MAX_STANDARD_IOB_CHANNELS][MAX_THRESHOLDS];

Result_t CheckThreshold(uint8_t channel_id, uint8_t threshold_id);
Result_t CheckThresholds(uint8_t channel_id);

#endif
