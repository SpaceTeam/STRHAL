#include "foc/tmc6200/TMC6200_highLevel.h"
#include "foc/tmc6200/TMC6200.h"
#include "foc/tmc6200/TMC6200_Fields.h"
#include "foc/tmc6200/TMC6200_Register.h"


void tmc6200_highLevel_init(void)
{
	tmc6200_writeRegister(TMC6200_GCONF, (3 << TMC6200_AMPLIFICATION_SHIFT)); // single line control: off, current amplification: 20
	tmc6200_writeRegister(TMC6200_DRV_CONF, (0 << TMC6200_DRVSTRENGTH_SHIFT)); // no BBM, 150C OT, low drive strength
}

