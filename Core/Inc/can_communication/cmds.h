#ifndef INC_CMDS_H__
#define INC_CMDS_H__


#include <stdint.h>

#define MAX_CHANNELS 32

typedef enum
{
	CHANNEL_TYPE_NODE_GENERIC,
	CHANNEL_TYPE_NODE_SPECIFIC, //?????? Todo: Implement or probably remove
	CHANNEL_TYPE_ADC16,
	CHANNEL_TYPE_ADC16_SINGLE,
	CHANNEL_TYPE_ADC24,
	CHANNEL_TYPE_DIGITAL_OUT,
	CHANNEL_TYPE_SERVO,


	CHANNEL_TYPE_LAST,
	CHANNEL_TYPE_UNKNOWN
} CHANNEL_TYPE;

typedef enum
{
	CHANNEL_STATUS_NOICE,
	CHANNEL_STATUS_TYPE_UNKNOWN,
	CHANNEL_STATUS_SENSOR_NOT_CONNECTED,
	CHANNEL_STATUS_WRONG_READINGS,
	CHANNEL_STATUS_ERROR,
	CHANNEL_STATUS_NO_CHANNEL
} CHANNEL_STATUS;



typedef enum
{
	NOICE = 0,
	OOF,
	OOF_SPI_TXP_FULL,
	OOF_SPI_RXP_FULL,
	OOF_SPI_NO_EOT,
	OOF_ADS_NO_RDY,
	OOF_ADS_INIT,
	OOF_CAN_INIT,
	OOF_CAN_TX_FULL,
}Result_t;

typedef Result_t (* can_function)(uint32_t *);









#endif
