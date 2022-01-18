#include "ECU.h"

int ECU_init() {
	ECU_Clock_Init();
	ECU_GPIO_Init();

	return 0;
}
