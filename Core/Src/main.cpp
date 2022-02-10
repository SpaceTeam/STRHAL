#include "LID.h"
#include <stdio.h>
#include <string.h>

int main(void) {
	if(LID_Init(LID_SYSCLK_SRC_INT, 0) != LID_NOICE)
		return -1;

	while(1) {
	}
	return 0;
}

void LID_OofHandler(LID_Oof_t oof, char *msg) {
	do{}while(0);
}
