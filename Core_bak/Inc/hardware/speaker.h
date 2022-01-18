#ifndef SPEAKER_H_
#define SPEAKER_H_

#include <stdint.h>

void Speaker_Init( void );
void Speaker_Update(uint64_t tick);
void Speaker_Set(uint16_t tone, uint16_t beep_on_period, uint16_t beep_off_period, uint8_t count);

#endif

