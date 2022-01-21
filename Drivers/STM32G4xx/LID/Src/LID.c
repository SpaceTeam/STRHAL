#include "../Inc/LID.h"

int LID_Init() {
	LID_Clock_Init();
	LID_GPIO_Init();

	return 0;
}
