#ifndef TMC_IC_TMC6200_H_
#define TMC_IC_TMC6200_H_

#include "foc/tmc_helpers/API_Header.h"
#include "foc/tmc6200/TMC6200_Register.h"
#include "foc/tmc6200/TMC6200_Constants.h"

int tmc6200_readInt(uint8_t address);
void tmc6200_writeInt(uint8_t address, int value);

#endif /* TMC_IC_TMC6630_H_ */
