#ifndef AS5147_H_
#define AS5147_H_


#include <foc/as5x47.h>
#include <stdint.h>

#include "foc/swdriver.h"
#include "spi.h"
#include "systick.h"

#define AS5047P 1
#define AS5047U 2
#define ENCODER AS5047U
void AS5x47_SendWord(uint8_t enc, uint16_t address, uint16_t data);
uint16_t AS5x47_ReceiveWord(uint8_t enc, uint16_t address);

void AS5x47_Init(uint8_t enc);
uint16_t AS5x47_GetAngle(uint8_t enc);


#endif /* AS5147_H_ */
