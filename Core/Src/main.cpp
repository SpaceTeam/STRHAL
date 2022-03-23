#include "ECU.h"
#include "LID.h"

int main(void) {
	ECU ecu(3,0x08b16d8c);

	if(ecu.init() != 0)
		return -1;

	LID_UART_Write("STARTED\n",8);
	ecu.exec();

	while(1);
}

void LID_OofHandler(LID_Oof_t oof, char *msg) {
	do{}while(0);
}
