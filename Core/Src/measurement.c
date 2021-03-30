#include "measurement.h"
#include <stdlib.h>
#include <limits.h>


void Measurement_Init(Measurement *measurement)
{
	for(uint8_t c = 0; c < MAX_MEASUREMENTS; c++)
		measurement->values[c] = 0;
	measurement->oldest_index = 0;
	measurement->sum_of_last_measurements = 0;
	measurement->number_of_measurements = 0;
}

void Measurement_Add(Measurement * measurement, int32_t value)
{
	measurement->values[measurement->oldest_index] = value;
	measurement->sum_of_last_measurements += measurement->values[measurement->oldest_index];
	measurement->oldest_index++;
	measurement->oldest_index &= (MAX_MEASUREMENTS - 1);

	measurement->sum_of_last_measurements -= measurement->values[measurement->oldest_index];
	measurement->number_of_measurements++;
}

int32_t Measurement_GetAverage(Measurement * measurement)
{
	return (int32_t)(measurement->sum_of_last_measurements >> MAX_MEASUREMENTS_EXP);
}
int32_t Measurement_GetLatest(Measurement * measurement, uint8_t offset)
{
	uint32_t prev_index = measurement->oldest_index + MAX_MEASUREMENTS - 1 - offset;
	prev_index &= (MAX_MEASUREMENTS - 1);
	return measurement->values[prev_index];
}

