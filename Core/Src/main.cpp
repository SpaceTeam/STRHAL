#include "ECU.h"
#include "LID.h"

int main(void) {
	ECU ecu(0,0xDEADBEEF);

	if(ecu.init() != 0)
		return -1;

	ecu.exec();
}

void LID_OofHandler(LID_Oof_t oof, char *msg) {
	do{}while(0);
}
