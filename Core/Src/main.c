#include "main.h"
#include "sysclock.h"
#include "systick.h"
#include "serial.h"
#include "speaker.h"

uint64_t tick = 0;
int main(void)
{


	Sysclock_Init();
	Systick_Init();

	Serial_Init();
	Speaker_Init();
	Speaker_Set(500, 120, 100, 2);
	while (1)
	{
		Speaker_Update(Systick_GetTick());
	}
}

void Error_Handler(void)
{
}
