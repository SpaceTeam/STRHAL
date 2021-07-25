#ifndef TMC_IC_TMC6200_H_
#define TMC_IC_TMC6200_H_

#include <stdint.h>


uint32_t tmc6200_readRegister(uint8_t address);
void tmc6200_writeRegister(uint8_t address, uint32_t value);


#endif /* TMC_IC_TMC6630_H_ */
