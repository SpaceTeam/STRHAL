#include <IOBv1.h>

#include <STRHAL.h>
#include "ECU.h"
#include "PMU.h"
#include "RCU.h"
#include "LCB.h"
#include "git_version.h"

int main(void)
{

#ifdef ECU_BOARD
#ifdef UART_DEBUG
	ECU ecu(6,GIT_COMMIT_HASH_VALUE,100); //6 ECU, 7 PMU, 9 TW
#else
	ECU ecu(6,GIT_COMMIT_HASH_VALUE,2);
#endif

	if(ecu.init() != 0)
		return -1;

	STRHAL_UART_Debug_Write_Blocking("ECU STARTED\n", 12, 50);
	ecu.exec();
#elif defined(PMU_BOARD)
#ifdef UART_DEBUG
	PMU pmu(7,GIT_COMMIT_HASH_VALUE,100);
#else
	PMU pmu(7,GIT_COMMIT_HASH_VALUE,2);
#endif
	if(pmu.init() != 0)
			return -1;

	STRHAL_UART_Debug_Write_Blocking("PMU STARTED\n", 12, 50);
	pmu.exec();
#elif defined(RCU_BOARD)
#ifdef UART_DEBUG
	RCU rcu(8,GIT_COMMIT_HASH_VALUE,100);
#else
	RCU rcu(8, GIT_COMMIT_HASH_VALUE, 4);
#endif

	if (rcu.init() != 0)
		return -1;

	STRHAL_UART_Debug_Write_Blocking("RCU STARTED\n", 12, 50);
	rcu.exec();
#elif defined(IOBv1_BOARD)
	IOBv1 iob(10,0xDEADBEEF,100); // TODO disregard node ID and read dipswitches in IOB/LCB class

	if(iob.init() != 0)
			return -1;

	STRHAL_UART_Debug_Write_Blocking("IOB STARTED\n", 12, 50);
	iob.exec();
#elif defined(IOBv3_BOARD)
	IOBv3 iob(10,GIT_COMMIT_HASH_VALUE,100); // TODO disregard node ID and read dipswitches in IOB/LCB class

	if(iob.init() != 0)
			return -1;

	STRHAL_UART_Debug_Write_Blocking("IOB STARTED\n", 12, 50);
	iob.exec();
#elif defined(LCB_BOARD)
	LCB lcb(10,0xDEADBEEF,100);

	if(lcb.init() != 0)
			return -1;

	STRHAL_UART_Debug_Write_Blocking("LCB STARTED\n", 12, 50);
	lcb.exec();
#endif

	while (1);
}

void STRHAL_OofHandler(STRHAL_Oof_t oof, char *msg)
{
	do
	{
	} while (0);
}
