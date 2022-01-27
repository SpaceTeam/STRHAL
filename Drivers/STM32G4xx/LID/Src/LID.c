#include "../Inc/LID.h"

int LID_Init() {
	LID_Clock_Init();
	LID_GPIO_Init();
	LID_OPAMP_Init();
	LID_UART_Init();
	LID_ADC_Init();

	return 0;
}
