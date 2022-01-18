#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include <stdint.h>

#define MAX_MEASUREMENTS_EXP	(7)
#define MAX_MEASUREMENTS		(1<<MAX_MEASUREMENTS_EXP)


typedef struct
{
	int32_t  values[MAX_MEASUREMENTS];
	uint32_t oldest_index;
	int64_t  sum_of_last_measurements;
	uint32_t number_of_measurements;
}Measurement;

void Measurement_Init(Measurement *measurement);
void Measurement_Add(Measurement * measurement, int32_t value);
int32_t Measurement_GetAverage(Measurement * measurement);
int32_t Measurement_GetLatest(Measurement * measurement, uint8_t offset);





#endif /* MEASUREMENT_H_ */
