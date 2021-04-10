#if BOARD == LCB
#include "lcb.h"
#include "main.h"
#include "channels.h"
#include "ADS131.h"
#include "speaker.h"
#include "can.h"
#include "systick.h"


//@formatter:off
Node_t node = { .node_id = 0, .firmware_version = 0xDEADBEEF,
				.generic_channel = { 0 },
				.channels =
					{
							{ 0, CHANNEL_TYPE_ADC24, {{0}} },
							{ 1, CHANNEL_TYPE_ADC24, {{0}} },
							{ 2, CHANNEL_TYPE_ADC24, {{0}} },
							{ 3, CHANNEL_TYPE_ADC24, {{0}} },
							{ 4, CHANNEL_TYPE_ADC24, {{0}} },
							{ 5, CHANNEL_TYPE_ADC24, {{0}} },
							{ 6, CHANNEL_TYPE_ADC24, {{0}} },
							{ 7, CHANNEL_TYPE_ADC24, {{0}} }
					}
				};
//@formatter:on

void LCB_main(void)
{
	uint64_t tick = 0;
	ADS131_Init();

	while (1)
	{
		Speaker_Update(tick);
		ADS131_UpdateData();
		Can_checkFifo(LCB_MAIN_CAN_BUS);
	}
}

#endif
