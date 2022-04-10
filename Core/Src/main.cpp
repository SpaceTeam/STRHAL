#include <STRHAL.h>
#include "ECU.h"

int main(void) {
	ECU ecu(6,0xDEADBEEF,100); //6 ECU, 7 PMU, 9 TW

	if(ecu.init() != 0)
		return -1;

	STRHAL_UART_Write("STARTED\n",8);
	ecu.exec();

	while(1);
}

void STRHAL_OofHandler(STRHAL_Oof_t oof, char *msg) {
	do{}while(0);
}
