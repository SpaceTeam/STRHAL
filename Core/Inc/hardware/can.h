#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>



#define CAN_TIMEOUT_VALUE				1000

#define CAN_ELMTS_ARRAY_SIZE			64
#define CAN_ELMTS_SIZE					FDCAN_DATA_BYTES_64


#define CAN_NOMINAL_PRESCALER			1
#define CAN_NOMINAL_SYNC_JUMP_WIDTH		1
#define CAN_NOMINAL_TIMESEG_1			2
#define CAN_NOMINAL_TIMESEG_2			2
#define CAN_DATA_PRESCALER				1
#define CAN_DATA_SYNC_JUMP_WIDTH		1
#define CAN_DATA_TIMESEG_1				1
#define CAN_DATA_TIMESEG_2				1
#define CAN_STD_FILTER_NUMBER			0
#define CAN_EXT_FILTER_NUMBER			0
#define CAN_RX_FIFO0_ELMTS_NUMBER		4
#define CAN_RX_FIFO0_ELMTS_SIZE			CAN_ELMTS_SIZE
#define CAN_RX_FIFO1_ELMTS_NUMBER		4
#define CAN_RX_FIFO1_ELMTS_SIZE			CAN_ELMTS_SIZE
#define CAN_RX_BUFFER_NUMBER			0
#define CAN_RX_BUFFER_SIZE				CAN_ELMTS_SIZE
#define CAN_TX_EVENT_NUMBER				0
#define CAN_TX_BUFFER_NUMBER			0
#define CAN_TX_FIFO_QUEUE_ELMTS_NUMBER	4
#define CAN_TX_FIFO_QUEUE_MODE			FDCAN_TX_QUEUE_OPERATION
#define CAN_TX_ELMTS_SIZE				CAN_ELMTS_SIZE
#define CAN_TDC_OFFSET					12
#define CAN_TDC_FILTER					0


typedef union {
	struct {
		uint32_t SFID2:11;		//bit: 0..10   Standard Filter ID 2
		uint32_t :5;			//bit: 11..15  Reserved
		uint32_t SFID1:11;		//bit: 16..26  Standard Filter ID 1
		uint32_t SFEC:3;		//bit: 27..29  Standard Filter Element Configuration
		uint32_t SFT:2;			//bit: 30..31  Standard Filter Type
	} bit;
	uint32_t reg;
} CAN_STANDARD_MESSAGE_FILTER_ELEMENT_S0_Type;
typedef struct {
	CAN_STANDARD_MESSAGE_FILTER_ELEMENT_S0_Type S0;
}Can_Standard_Filter_Element;


typedef union {
	struct {
		uint32_t EFID1:29;	//bit: 0..28   Extended Filter ID 1
		uint32_t EFEC:3;	//bit: 29..31  Extended Filter Element Configuration
	} bit;
	uint32_t reg;
} CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F0_Type;
typedef union {
	struct {
		uint32_t EFID2:29;	//bit: 0..28  Extended Filter ID 2
		uint32_t :1;		//bit: 29     Reserved
		uint32_t EFT:2;		//bit: 30..31 Extended Filter Type
	} bit;
	uint32_t reg;
} CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F1_Type;
typedef struct {
	CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F0_Type F0;
	CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F1_Type F1;
} Can_Extended_Filter_Element;

typedef union
{
	struct
	{
		uint32_t ID :29;		//bit:  0..28  Identifier
		uint32_t RTR :1;		//bit:  29     Remote Transmission Request
		uint32_t XTD :1;		//bit:  30     Extended Identifier
		uint32_t ESI :1;		//bit:  31     Error State Indicator
	} bit;
	uint32_t reg;
} CAN_RX_ELEMENT_R0_Type;
typedef union
{
	struct
	{
		uint32_t RXTS :16;		//bit: 0..15   Rx Timestamp
		uint32_t DLC :4;		//bit: 16..19  Data Length Code
		uint32_t BRS :1;		//bit: 20      Bit Rate Switch
		uint32_t FDF :1;		//bit: 21      FD Format
		uint32_t :2;			//bit: 22..23  Reserved
		uint32_t FIDX :7;		//bit: 24..30  Filter Index
		uint32_t ANMF :1;		//bit: 31      Accepted Non-matching Frame
	} bit;
	uint32_t reg;
} CAN_RX_ELEMENT_R1_Type;

typedef union
{
	uint8_t uint8[CAN_ELMTS_ARRAY_SIZE];
	uint32_t uint32[CAN_ELMTS_ARRAY_SIZE / 4];
} CAN_RX_ELEMENT_DATA_Type;
typedef struct
{
	CAN_RX_ELEMENT_R0_Type R0;
	CAN_RX_ELEMENT_R1_Type R1;
	CAN_RX_ELEMENT_DATA_Type data;
} Can_Rx_Element;



typedef union {
	struct {
		uint32_t ID:29;		//bit:  0..28  Identifier
		uint32_t RTR:1;		//bit:  29     Remote Transmission Request
		uint32_t XTD:1;		//bit:  30     Extended Identifier
		uint32_t :1;		//bit:  31     Reserved
	} bit;
	uint32_t reg;
} CAN_TX_ELEMENT_T0_Type;
typedef union {
	struct {
		uint32_t :16;		//bit: 0..15   Reserved
		uint32_t DLC:4;		//bit: 16..19  Data Length Code
		uint32_t BRS:1;		//bit: 20      Bit Rate Switching
		uint32_t FDF:1;		//bit: 21      FD Format
		uint32_t :1;		//bit: 22      Reserved
		uint32_t EFCC:1;	//bit: 23      Event FIFO Control
		uint32_t MM:8;		//bit: 24..31  Message Marker
	} bit;
	uint32_t reg;
} CAN_TX_ELEMENT_T1_Type;

typedef union {
	uint8_t uint8[CAN_ELMTS_ARRAY_SIZE];
	uint32_t uint32[CAN_ELMTS_ARRAY_SIZE/4];
} CAN_TX_ELEMENT_DATA_Type;

typedef struct  {
	CAN_TX_ELEMENT_T0_Type T0;
	CAN_TX_ELEMENT_T1_Type T1;
	CAN_TX_ELEMENT_DATA_Type data;
}Can_Tx_Element;

typedef struct
{
	Can_Standard_Filter_Element std_filters[CAN_STD_FILTER_NUMBER];
	Can_Extended_Filter_Element ext_filters[CAN_EXT_FILTER_NUMBER];
	Can_Rx_Element rx_buffer[CAN_RX_BUFFER_NUMBER];
	Can_Rx_Element rx_fifo0[CAN_RX_FIFO0_ELMTS_NUMBER];
	Can_Rx_Element rx_fifo1[CAN_RX_FIFO1_ELMTS_NUMBER];
	Can_Tx_Element tx_buffer[CAN_TX_BUFFER_NUMBER];
	Can_Tx_Element tx_fifo[CAN_TX_FIFO_QUEUE_ELMTS_NUMBER];

} Can_Message_RAM;

#endif
