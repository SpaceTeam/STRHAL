#include <STRHAL.h>
#include "ECU.h"
#include "PMU.h"

int main(void) {

#ifdef ECU_BOARD
	ECU ecu(6,0xDEADBEEF,100); //6 ECU, 7 PMU, 9 TW

	if(ecu.init() != 0)
		return -1;

	STRHAL_UART_Write("ECU STARTED\n",12);
	ecu.exec();
#elif defined(PMU_BOARD)
	PMU pmu(7,0xDEADBEEF,100);

	if(pmu.init() != 0)
			return -1;

	STRHAL_UART_Write("PMU STARTED\n",12);
	pmu.exec();
#endif

	while(1);
}

void STRHAL_OofHandler(STRHAL_Oof_t oof, char *msg) {
	do{}while(0);
}
