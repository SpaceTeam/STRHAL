#include <STRHAL.h>
#include "ECU.h"
#include "PMU.h"
#include "RCU.h"
#include "IOB.h"
#include "LCB.h"

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
#elif defined(RCU_BOARD)
	RCU rcu(10,0xDEADBEEF,100);

	if(rcu.init() != 0)
			return -1;

	STRHAL_UART_Write("RCU STARTED\n",12);
	rcu.exec();
#elif defined(IOB_BOARD)
	IOB iob(10,0xDEADBEEF,100); // TODO disregard node ID and read dipswitches in IOB/LCB class

	if(iob.init() != 0)
			return -1;

	STRHAL_UART_Write("IOB STARTED\n",12);
	iob.exec();
#elif defined(LCB_BOARD)
	LCB lcb(10,0xDEADBEEF,100);

	if(lcb.init() != 0)
			return -1;

	STRHAL_UART_Write("LCB STARTED\n",12);
	lcb.exec();
#endif

	while(1);
}

void STRHAL_OofHandler(STRHAL_Oof_t oof, char *msg) {
	do{}while(0);
}
