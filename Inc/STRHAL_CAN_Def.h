#ifndef STRHAL_CAN_DEF_H
#define STRHAL_CAN_DEF_H

#include <stdint.h>
#include <stm32g4xx.h>


#ifdef __cplusplus
extern "C" {
#endif

#define FDCAN_FRAME_CLASSIC   ((uint32_t)0x00000000U)                         /*!< Classic mode                      */
#define FDCAN_FRAME_FD_NO_BRS ((uint32_t)FDCAN_CCCR_FDOE)                     /*!< FD mode without BitRate Switching */
#define FDCAN_FRAME_FD_BRS    ((uint32_t)(FDCAN_CCCR_FDOE | FDCAN_CCCR_BRSE)) /*!< FD mode with BitRate Switching    */

#define FDCAN_CLOCK_DIV1  ((uint32_t)0x00000000U) /*!< Divide kernel clock by 1  */
#define FDCAN_CLOCK_DIV2  ((uint32_t)0x00000001U) /*!< Divide kernel clock by 2  */
#define FDCAN_CLOCK_DIV4  ((uint32_t)0x00000002U) /*!< Divide kernel clock by 4  */
#define FDCAN_CLOCK_DIV6  ((uint32_t)0x00000003U) /*!< Divide kernel clock by 6  */
#define FDCAN_CLOCK_DIV8  ((uint32_t)0x00000004U) /*!< Divide kernel clock by 8  */
#define FDCAN_CLOCK_DIV10 ((uint32_t)0x00000005U) /*!< Divide kernel clock by 10 */
#define FDCAN_CLOCK_DIV12 ((uint32_t)0x00000006U) /*!< Divide kernel clock by 12 */
#define FDCAN_CLOCK_DIV14 ((uint32_t)0x00000007U) /*!< Divide kernel clock by 14 */
#define FDCAN_CLOCK_DIV16 ((uint32_t)0x00000008U) /*!< Divide kernel clock by 16 */
#define FDCAN_CLOCK_DIV18 ((uint32_t)0x00000009U) /*!< Divide kernel clock by 18 */
#define FDCAN_CLOCK_DIV20 ((uint32_t)0x0000000AU) /*!< Divide kernel clock by 20 */
#define FDCAN_CLOCK_DIV22 ((uint32_t)0x0000000BU) /*!< Divide kernel clock by 22 */
#define FDCAN_CLOCK_DIV24 ((uint32_t)0x0000000CU) /*!< Divide kernel clock by 24 */
#define FDCAN_CLOCK_DIV26 ((uint32_t)0x0000000DU) /*!< Divide kernel clock by 26 */
#define FDCAN_CLOCK_DIV28 ((uint32_t)0x0000000EU) /*!< Divide kernel clock by 28 */
#define FDCAN_CLOCK_DIV30 ((uint32_t)0x0000000FU) /*!< Divide kernel clock by 30 */

#define FDCAN_FILTER_DISABLE       ((uint32_t)0x00000000U) /*!< Disable filter element                                    */
#define FDCAN_FILTER_TO_RXFIFO0    ((uint32_t)0x00000001U) /*!< Store in Rx FIFO 0 if filter matches                      */
#define FDCAN_FILTER_TO_RXFIFO1    ((uint32_t)0x00000002U) /*!< Store in Rx FIFO 1 if filter matches                      */
#define FDCAN_FILTER_REJECT        ((uint32_t)0x00000003U) /*!< Reject ID if filter matches                               */
#define FDCAN_FILTER_HP            ((uint32_t)0x00000004U) /*!< Set high priority if filter matches                       */
#define FDCAN_FILTER_TO_RXFIFO0_HP ((uint32_t)0x00000005U) /*!< Set high priority and store in FIFO 0 if filter matches   */
#define FDCAN_FILTER_TO_RXFIFO1_HP ((uint32_t)0x00000006U) /*!< Set high priority and store in FIFO 1 if filter matches   */

#define FDCAN_RX_FIFO_BLOCKING  ((uint32_t)0x00000000U) /*!< Rx FIFO blocking mode  */
#define FDCAN_RX_FIFO_OVERWRITE ((uint32_t)0x00000001U) /*!< Rx FIFO overwrite mode */

#define FDCAN_ACCEPT_IN_RX_FIFO0 ((uint32_t)0x00000000U) /*!< Accept in Rx FIFO 0 */
#define FDCAN_ACCEPT_IN_RX_FIFO1 ((uint32_t)0x00000001U) /*!< Accept in Rx FIFO 1 */
#define FDCAN_REJECT             ((uint32_t)0x00000002U) /*!< Reject              */

#define FDCAN_FILTER_REMOTE ((uint32_t)0x00000000U) /*!< Filter remote frames */
#define FDCAN_REJECT_REMOTE ((uint32_t)0x00000001U) /*!< Reject all remote frames */

#define FDCAN_FILTER_RANGE         ((uint32_t)0x00000000U) //!< Range filter from FilterID1 to FilterID2
#define FDCAN_FILTER_DUAL          ((uint32_t)0x00000001U) //!< Dual ID filter for FilterID1 or FilterID2
#define FDCAN_FILTER_MASK          ((uint32_t)0x00000002U) //!< Classic filter: FilterID1 = filter, FilterID2 = mask
#define FDCAN_FILTER_RANGE_NO_EIDM ((uint32_t)0x00000003U) //!< Range filter from FilterID1 to FilterID2, EIDM mask not applied

#define FDCAN_ELMTS_ARRAY_SIZE			64
#define FDCAN_NOMINAL_PRESCALER			2 // 40MHz, tq = 25ns
#define FDCAN_NOMINAL_SYNC_JUMP_WIDTH	1
#define FDCAN_NOMINAL_TIMESEG_1			23
#define FDCAN_NOMINAL_TIMESEG_2			16

#define FDCAN_DATA_PRESCALER			4 // 20MHz, tq = 50ns
#define FDCAN_DATA_SYNC_JUMP_WIDTH		1
#define FDCAN_DATA_TIMESEG_1			2
#define FDCAN_DATA_TIMESEG_2			2

#define FDCAN_TDC_OFFSET				3
#define FDCAN_TDC_FILTER				4

//#define FDCAN_MESSAGE_RAM_BASE		((uint32_t)0x4000AC00U)
#define FDCAN_MESSAGE_RAM_BASE		SRAMCAN_BASE
#define FDCAN1_MESSAGE_RAM_OFFSET	((uint32_t)0x00000000U)
#define FDCAN2_MESSAGE_RAM_OFFSET	((uint32_t)0x00000354U)
#define FDCAN1_MESSAGE_RAM_START	((uint32_t)(FDCAN1_MESSAGE_RAM_OFFSET + FDCAN_MESSAGE_RAM_BASE))
#define FDCAN2_MESSAGE_RAM_START	((uint32_t)(FDCAN2_MESSAGE_RAM_OFFSET + FDCAN_MESSAGE_RAM_BASE))
#define FDCAN1_MESSAGE_RAM			((Can_Message_RAM *) FDCAN1_MESSAGE_RAM_START)
#define FDCAN2_MESSAGE_RAM			((Can_Message_RAM *) FDCAN2_MESSAGE_RAM_START)

typedef union {
	struct __attribute__((__packed__)) {
		volatile uint32_t SFID2 :11;		//bit: 0..10   Standard Filter ID 2
		volatile uint32_t :5;			//bit: 11..15  Reserved
		volatile uint32_t SFID1 :11;		//bit: 16..26  Standard Filter ID 1
		volatile uint32_t SFEC :3;		//bit: 27..29  Standard Filter Element Configuration
		volatile uint32_t SFT :2;			//bit: 30..31  Standard Filter Type
	} bit;
	volatile uint32_t reg;
} CAN_STANDARD_MESSAGE_FILTER_ELEMENT_S0_Type;

typedef struct __attribute__((__packed__)) {
	volatile CAN_STANDARD_MESSAGE_FILTER_ELEMENT_S0_Type S0;
} Can_Standard_Filter_Element;

typedef union {
	struct __attribute__((__packed__)) {
		uint32_t EFID1 :29;	//bit: 0..28   Extended Filter ID 1
		uint32_t EFEC :3;	//bit: 29..31  Extended Filter Element Configuration
	} bit;
	volatile uint32_t reg;
} CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F0_Type;

typedef union {
	struct __attribute__((__packed__)) {
		uint32_t EFID2 :29;	//bit: 0..28  Extended Filter ID 2
		uint32_t :1;		//bit: 29     Reserved
		uint32_t EFT :2;		//bit: 30..31 Extended Filter Type
	} bit;
	volatile uint32_t reg;
} CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F1_Type;

typedef struct __attribute__((__packed__)) {
	volatile CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F0_Type F0;
	volatile CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F1_Type F1;
} Can_Extended_Filter_Element;

typedef union {
	struct {
		uint32_t ID :29;		//bit:  0..28  Identifier
		uint32_t RTR :1;		//bit:  29     Remote Transmission Request
		uint32_t XTD :1;		//bit:  30     Extended Identifier
		uint32_t ESI :1;		//bit:  31     Error State Indicator
	} bit;
	uint32_t reg;
} CAN_RX_ELEMENT_R0_Type;

typedef union {
	struct {
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

typedef union {
	struct {
		uint32_t ID :29;		//bit:  0..28  Identifier
		uint32_t RTR :1;		//bit:  29     Remote Transmission Request
		uint32_t XTD :1;		//bit:  30     Extended Identifier
		uint32_t :1;		//bit:  31     Reserved
	} bit;
	uint32_t reg;
} CAN_TX_ELEMENT_T0_Type;

typedef union {
	struct {
		uint32_t :16;		//bit: 0..15   Reserved
		uint32_t DLC :4;		//bit: 16..19  Data Length Code
		uint32_t BRS :1;		//bit: 20      Bit Rate Switching
		uint32_t FDF :1;		//bit: 21      FD Format
		uint32_t :1;		//bit: 22      Reserved
		uint32_t EFCC :1;	//bit: 23      Event FIFO Control
		uint32_t MM :8;		//bit: 24..31  Message Marker
	} bit;
	uint32_t reg;
} CAN_TX_ELEMENT_T1_Type;

typedef union {
	struct {
		uint32_t ID :29;		//bit:  0..28  Identifier
		uint32_t RTR :1;		//bit:  29     Remote Transmission Request
		uint32_t XTD :1;		//bit:  30     Extended Identifier
		uint32_t ESI :1;		//bit:  31     Error State Indicator
	} bit;
	uint32_t reg;
} CAN_TX_EVENT_ELEMENT_E0_Type;

typedef union {
	struct {
		uint32_t TXTS :16;	//bit:  0..15  Tx Timestamp
		uint32_t DLC :4;	//bit:  16..19 Data length code
		uint32_t BRS :1;	//bit:  20     Bit rate switching
		uint32_t EDL :1;	//bit:  21     Extended data length
		uint32_t ET :2;		//bit:  22..23 Event type
		uint32_t MM :8;		//bit:  24..31 Message marker
	} bit;
	uint32_t reg;
} CAN_TX_EVENT_ELEMENT_E1_Type;

typedef union
{
	uint8_t byte[64];
	uint32_t word[64 / 4];
} CAN_RX_ELEMENT_DATA_Type;

typedef struct {
	CAN_RX_ELEMENT_R0_Type R0;
	CAN_RX_ELEMENT_R1_Type R1;
	CAN_RX_ELEMENT_DATA_Type data;
} Can_Rx_Element;

typedef union {
	uint8_t byte[64];
	uint32_t word[64 / 4];
} CAN_TX_ELEMENT_DATA_Type;

typedef volatile struct {
	CAN_TX_ELEMENT_T0_Type T0;
	CAN_TX_ELEMENT_T1_Type T1;
	CAN_TX_ELEMENT_DATA_Type data;
} Can_Tx_Element;

typedef volatile struct {
	CAN_TX_EVENT_ELEMENT_E0_Type E0;
	CAN_TX_EVENT_ELEMENT_E1_Type E1;
} Can_Tx_Event_Element;

typedef struct {
	__attribute__((__aligned__(4))) Can_Standard_Filter_Element std_filters[28];
	__attribute__((__aligned__(4))) Can_Extended_Filter_Element ext_filters[8];
	__attribute__((__aligned__(4))) Can_Rx_Element rx_fifo0[3];
	__attribute__((__aligned__(4))) Can_Rx_Element rx_fifo1[3];
	__attribute__((__aligned__(4))) Can_Tx_Event_Element tx_event_fifo[3];
	__attribute__((__aligned__(4))) Can_Tx_Element tx_buffer[3];

} Can_Message_RAM;


#ifdef __cplusplus
}
#endif

#endif
