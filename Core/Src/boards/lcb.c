#if BOARD == LCB
#include "lcb.h"
#include "main.h"
#include "channels.h"

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

}

#endif
