#ifndef CAN_DEF_H__
#define CAN_DEF_H__
#include <stdint.h>
#include "can_config.h"

#define FDCAN_ERROR_NONE            ((uint32_t)0x00000000U) //!< No error
#define FDCAN_ERROR_TIMEOUT         ((uint32_t)0x00000001U) //!< Timeout error
#define FDCAN_ERROR_NOT_INITIALIZED ((uint32_t)0x00000002U) //!< Peripheral not initialized
#define FDCAN_ERROR_NOT_READY       ((uint32_t)0x00000004U) //!< Peripheral not ready
#define FDCAN_ERROR_NOT_STARTED     ((uint32_t)0x00000008U) //!< Peripheral not started
#define FDCAN_ERROR_NOT_SUPPORTED   ((uint32_t)0x00000010U) //!< Mode not supported
#define FDCAN_ERROR_PARAM           ((uint32_t)0x00000020U) //!< Parameter error
#define FDCAN_ERROR_PENDING         ((uint32_t)0x00000040U) //!< Pending operation
#define FDCAN_ERROR_RAM_ACCESS      ((uint32_t)0x00000080U) //!< Message RAM Access Failure
#define FDCAN_ERROR_FIFO_EMPTY      ((uint32_t)0x00000100U) //!< Put element in full FIFO
#define FDCAN_ERROR_FIFO_FULL       ((uint32_t)0x00000200U) //!< Get element from empty FIFO
#define FDCAN_ERROR_LOG_OVERFLOW    FDCAN_IR_ELO            //!< Overflow of CAN Error Logging Counter
#define FDCAN_ERROR_RAM_WDG         FDCAN_IR_WDI            //!< Message RAM Watchdog event occurred
#define FDCAN_ERROR_PROTOCOL_ARBT   FDCAN_IR_PEA            //!< Protocol Error in Arbitration Phase (Nominal Bit Time is used)
#define FDCAN_ERROR_PROTOCOL_DATA   FDCAN_IR_PED            //!< Protocol Error in Data Phase (Data Bit Time is used)
#define FDCAN_ERROR_RESERVED_AREA   FDCAN_IR_ARA            //!< Access to Reserved Address
#define FDCAN_ERROR_TT_GLOBAL_TIME  FDCAN_TTIR_GTE          //!< Global Time Error : Synchronization deviation exceeded limit
#define FDCAN_ERROR_TT_TX_UNDERFLOW FDCAN_TTIR_TXU          //!< Tx Count Underflow : Less Tx trigger than expected in one matrix cycle
#define FDCAN_ERROR_TT_TX_OVERFLOW  FDCAN_TTIR_TXO          //!< Tx Count Overflow : More Tx trigger than expected in one matrix cycle
#define FDCAN_ERROR_TT_SCHEDULE1    FDCAN_TTIR_SE1          //!< Scheduling error 1
#define FDCAN_ERROR_TT_SCHEDULE2    FDCAN_TTIR_SE2          //!< Scheduling error 2
#define FDCAN_ERROR_TT_NO_INIT_REF  FDCAN_TTIR_IWT          //!< No system startup due to missing reference message
#define FDCAN_ERROR_TT_NO_REF       FDCAN_TTIR_WT           //!< Missing reference message
#define FDCAN_ERROR_TT_APPL_WDG     FDCAN_TTIR_AW           //!< Application watchdog not served in time
#define FDCAN_ERROR_TT_CONFIG       FDCAN_TTIR_CER          //!< Error found in trigger list

#define FDCAN_FRAME_CLASSIC   ((uint32_t)0x00000000U)                         //!< Classic mode
#define FDCAN_FRAME_FD_NO_BRS ((uint32_t)FDCAN_CCCR_FDOE)                     //!< FD mode without BitRate Switching
#define FDCAN_FRAME_FD_BRS    ((uint32_t)(FDCAN_CCCR_FDOE | FDCAN_CCCR_BRSE)) //!< FD mode with BitRate Switching

#define FDCAN_MODE_NORMAL               ((uint32_t)0x00000000U) //!< Normal mode
#define FDCAN_MODE_RESTRICTED_OPERATION ((uint32_t)0x00000001U) //!< Restricted Operation mode
#define FDCAN_MODE_BUS_MONITORING       ((uint32_t)0x00000002U) //!< Bus Monitoring mode
#define FDCAN_MODE_INTERNAL_LOOPBACK    ((uint32_t)0x00000003U) //!< Internal LoopBack mode
#define FDCAN_MODE_EXTERNAL_LOOPBACK    ((uint32_t)0x00000004U) //!< External LoopBack mode

#define FDCAN_CLOCK_CALIBRATION_DISABLE ((uint32_t)0x00000000U) //!< Disable Clock Calibration
#define FDCAN_CLOCK_CALIBRATION_ENABLE  ((uint32_t)0x00000001U) //!< Enable Clock Calibration

#define FDCAN_CLOCK_DIV1  ((uint32_t)0x00000000U) //!< Divide kernel clock by 1
#define FDCAN_CLOCK_DIV2  ((uint32_t)0x00010000U) //!< Divide kernel clock by 2
#define FDCAN_CLOCK_DIV4  ((uint32_t)0x00020000U) //!< Divide kernel clock by 4
#define FDCAN_CLOCK_DIV6  ((uint32_t)0x00030000U) //!< Divide kernel clock by 6
#define FDCAN_CLOCK_DIV8  ((uint32_t)0x00040000U) //!< Divide kernel clock by 8
#define FDCAN_CLOCK_DIV10 ((uint32_t)0x00050000U) //!< Divide kernel clock by 10
#define FDCAN_CLOCK_DIV12 ((uint32_t)0x00060000U) //!< Divide kernel clock by 12
#define FDCAN_CLOCK_DIV14 ((uint32_t)0x00070000U) //!< Divide kernel clock by 14
#define FDCAN_CLOCK_DIV16 ((uint32_t)0x00080000U) //!< Divide kernel clock by 16
#define FDCAN_CLOCK_DIV18 ((uint32_t)0x00090000U) //!< Divide kernel clock by 18
#define FDCAN_CLOCK_DIV20 ((uint32_t)0x000A0000U) //!< Divide kernel clock by 20
#define FDCAN_CLOCK_DIV22 ((uint32_t)0x000B0000U) //!< Divide kernel clock by 22
#define FDCAN_CLOCK_DIV24 ((uint32_t)0x000C0000U) //!< Divide kernel clock by 24
#define FDCAN_CLOCK_DIV26 ((uint32_t)0x000D0000U) //!< Divide kernel clock by 26
#define FDCAN_CLOCK_DIV28 ((uint32_t)0x000E0000U) //!< Divide kernel clock by 28
#define FDCAN_CLOCK_DIV30 ((uint32_t)0x000F0000U) //!< Divide kernel clock by 30

#define FDCAN_CALIB_FIELD_LENGTH_32 ((uint32_t)0x00000000U)       //!< Calibration field length is 32 bits
#define FDCAN_CALIB_FIELD_LENGTH_64 ((uint32_t)FDCANCCU_CCFG_CFL) //!< Calibration field length is 64 bits

#define FDCAN_CLOCK_NOT_CALIBRATED       ((uint32_t)0x00000000U) //!< Clock not calibrated
#define FDCAN_CLOCK_BASIC_CALIBRATED     ((uint32_t)0x40000000U) //!< Clock basic calibrated
#define FDCAN_CLOCK_PRECISION_CALIBRATED ((uint32_t)0x80000000U) //!< Clock precision calibrated

#define FDCAN_CALIB_TIME_QUANTA_COUNTER  ((uint32_t)0x00000000U) //!< Time Quanta Counter
#define FDCAN_CALIB_CLOCK_PERIOD_COUNTER ((uint32_t)0x00000001U) //!< Oscillator Clock Period Counter
#define FDCAN_CALIB_WATCHDOG_COUNTER     ((uint32_t)0x00000002U) //!< Calibration Watchdog Counter

#define FDCAN_DATA_BYTES_8  ((uint32_t)0x00000004U) //!< 8 bytes data field
#define FDCAN_DATA_BYTES_12 ((uint32_t)0x00000005U) //!< 12 bytes data field
#define FDCAN_DATA_BYTES_16 ((uint32_t)0x00000006U) //!< 16 bytes data field
#define FDCAN_DATA_BYTES_20 ((uint32_t)0x00000007U) //!< 20 bytes data field
#define FDCAN_DATA_BYTES_24 ((uint32_t)0x00000008U) //!< 24 bytes data field
#define FDCAN_DATA_BYTES_32 ((uint32_t)0x0000000AU) //!< 32 bytes data field
#define FDCAN_DATA_BYTES_48 ((uint32_t)0x0000000EU) //!< 48 bytes data field
#define FDCAN_DATA_BYTES_64 ((uint32_t)0x00000012U) //!< 64 bytes data field

#define FDCAN_TX_FIFO_OPERATION  ((uint32_t)0x00000000U)     //!< FIFO mode
#define FDCAN_TX_QUEUE_OPERATION ((uint32_t)FDCAN_TXBC_TFQM) //!< Queue mode

#define FDCAN_STANDARD_ID ((uint32_t)0x00000000U) //!< Standard ID element
#define FDCAN_EXTENDED_ID ((uint32_t)0x40000000U) //!< Extended ID element

#define FDCAN_DATA_FRAME   ((uint32_t)0x00000000U)  //!< Data frame
#define FDCAN_REMOTE_FRAME ((uint32_t)0x20000000U)  //!< Remote frame

#define FDCAN_DLC_BYTES_0  ((uint32_t)0x00000000U) //!< 0 bytes data field
#define FDCAN_DLC_BYTES_1  ((uint32_t)0x00010000U) //!< 1 bytes data field
#define FDCAN_DLC_BYTES_2  ((uint32_t)0x00020000U) //!< 2 bytes data field
#define FDCAN_DLC_BYTES_3  ((uint32_t)0x00030000U) //!< 3 bytes data field
#define FDCAN_DLC_BYTES_4  ((uint32_t)0x00040000U) //!< 4 bytes data field
#define FDCAN_DLC_BYTES_5  ((uint32_t)0x00050000U) //!< 5 bytes data field
#define FDCAN_DLC_BYTES_6  ((uint32_t)0x00060000U) //!< 6 bytes data field
#define FDCAN_DLC_BYTES_7  ((uint32_t)0x00070000U) //!< 7 bytes data field
#define FDCAN_DLC_BYTES_8  ((uint32_t)0x00080000U) //!< 8 bytes data field
#define FDCAN_DLC_BYTES_12 ((uint32_t)0x00090000U) //!< 12 bytes data field
#define FDCAN_DLC_BYTES_16 ((uint32_t)0x000A0000U) //!< 16 bytes data field
#define FDCAN_DLC_BYTES_20 ((uint32_t)0x000B0000U) //!< 20 bytes data field
#define FDCAN_DLC_BYTES_24 ((uint32_t)0x000C0000U) //!< 24 bytes data field
#define FDCAN_DLC_BYTES_32 ((uint32_t)0x000D0000U) //!< 32 bytes data field
#define FDCAN_DLC_BYTES_48 ((uint32_t)0x000E0000U) //!< 48 bytes data field
#define FDCAN_DLC_BYTES_64 ((uint32_t)0x000F0000U) //!< 64 bytes data field

#define FDCAN_ESI_ACTIVE  ((uint32_t)0x00000000U) //!< Transmitting node is error active
#define FDCAN_ESI_PASSIVE ((uint32_t)0x80000000U) //!< Transmitting node is error passive

#define FDCAN_BRS_OFF ((uint32_t)0x00000000U) //!< FDCAN frames transmitted/received without bit rate switching
#define FDCAN_BRS_ON  ((uint32_t)0x00100000U) //!< FDCAN frames transmitted/received with bit rate switching

#define FDCAN_CLASSIC_CAN ((uint32_t)0x00000000U) //!< Frame transmitted/received in Classic CAN format
#define FDCAN_FD_CAN      ((uint32_t)0x00200000U) //!< Frame transmitted/received in FDCAN format

#define FDCAN_NO_TX_EVENTS    ((uint32_t)0x00000000U) //!< Do not store Tx events
#define FDCAN_STORE_TX_EVENTS ((uint32_t)0x00800000U) //!< Store Tx events

#define FDCAN_FILTER_RANGE         ((uint32_t)0x00000000U) //!< Range filter from FilterID1 to FilterID2
#define FDCAN_FILTER_DUAL          ((uint32_t)0x00000001U) //!< Dual ID filter for FilterID1 or FilterID2
#define FDCAN_FILTER_MASK          ((uint32_t)0x00000002U) //!< Classic filter: FilterID1 = filter, FilterID2 = mask
#define FDCAN_FILTER_RANGE_NO_EIDM ((uint32_t)0x00000003U) //!< Range filter from FilterID1 to FilterID2, EIDM mask not applied

#define FDCAN_FILTER_DISABLE       ((uint32_t)0x00000000U) //!< Disable filter element
#define FDCAN_FILTER_TO_RXFIFO0    ((uint32_t)0x00000001U) //!< Store in Rx FIFO 0 if filter matches
#define FDCAN_FILTER_TO_RXFIFO1    ((uint32_t)0x00000002U) //!< Store in Rx FIFO 1 if filter matches
#define FDCAN_FILTER_REJECT        ((uint32_t)0x00000003U) //!< Reject ID if filter matches
#define FDCAN_FILTER_HP            ((uint32_t)0x00000004U) //!< Set high priority if filter matches
#define FDCAN_FILTER_TO_RXFIFO0_HP ((uint32_t)0x00000005U) //!< Set high priority and store in FIFO 0 if filter matches
#define FDCAN_FILTER_TO_RXFIFO1_HP ((uint32_t)0x00000006U) //!< Set high priority and store in FIFO 1 if filter matches
#define FDCAN_FILTER_TO_RXBUFFER   ((uint32_t)0x00000007U) //!< Store into Rx Buffer, configuration of FilterType ignored

#define FDCAN_TX_BUFFER0  ((uint32_t)0x00000001U) //!< Add message to Tx Buffer 0
#define FDCAN_TX_BUFFER1  ((uint32_t)0x00000002U) //!< Add message to Tx Buffer 1
#define FDCAN_TX_BUFFER2  ((uint32_t)0x00000004U) //!< Add message to Tx Buffer 2
#define FDCAN_TX_BUFFER3  ((uint32_t)0x00000008U) //!< Add message to Tx Buffer 3
#define FDCAN_TX_BUFFER4  ((uint32_t)0x00000010U) //!< Add message to Tx Buffer 4
#define FDCAN_TX_BUFFER5  ((uint32_t)0x00000020U) //!< Add message to Tx Buffer 5
#define FDCAN_TX_BUFFER6  ((uint32_t)0x00000040U) //!< Add message to Tx Buffer 6
#define FDCAN_TX_BUFFER7  ((uint32_t)0x00000080U) //!< Add message to Tx Buffer 7
#define FDCAN_TX_BUFFER8  ((uint32_t)0x00000100U) //!< Add message to Tx Buffer 8
#define FDCAN_TX_BUFFER9  ((uint32_t)0x00000200U) //!< Add message to Tx Buffer 9
#define FDCAN_TX_BUFFER10 ((uint32_t)0x00000400U) //!< Add message to Tx Buffer 10
#define FDCAN_TX_BUFFER11 ((uint32_t)0x00000800U) //!< Add message to Tx Buffer 11
#define FDCAN_TX_BUFFER12 ((uint32_t)0x00001000U) //!< Add message to Tx Buffer 12
#define FDCAN_TX_BUFFER13 ((uint32_t)0x00002000U) //!< Add message to Tx Buffer 13
#define FDCAN_TX_BUFFER14 ((uint32_t)0x00004000U) //!< Add message to Tx Buffer 14
#define FDCAN_TX_BUFFER15 ((uint32_t)0x00008000U) //!< Add message to Tx Buffer 15
#define FDCAN_TX_BUFFER16 ((uint32_t)0x00010000U) //!< Add message to Tx Buffer 16
#define FDCAN_TX_BUFFER17 ((uint32_t)0x00020000U) //!< Add message to Tx Buffer 17
#define FDCAN_TX_BUFFER18 ((uint32_t)0x00040000U) //!< Add message to Tx Buffer 18
#define FDCAN_TX_BUFFER19 ((uint32_t)0x00080000U) //!< Add message to Tx Buffer 19
#define FDCAN_TX_BUFFER20 ((uint32_t)0x00100000U) //!< Add message to Tx Buffer 20
#define FDCAN_TX_BUFFER21 ((uint32_t)0x00200000U) //!< Add message to Tx Buffer 21
#define FDCAN_TX_BUFFER22 ((uint32_t)0x00400000U) //!< Add message to Tx Buffer 22
#define FDCAN_TX_BUFFER23 ((uint32_t)0x00800000U) //!< Add message to Tx Buffer 23
#define FDCAN_TX_BUFFER24 ((uint32_t)0x01000000U) //!< Add message to Tx Buffer 24
#define FDCAN_TX_BUFFER25 ((uint32_t)0x02000000U) //!< Add message to Tx Buffer 25
#define FDCAN_TX_BUFFER26 ((uint32_t)0x04000000U) //!< Add message to Tx Buffer 26
#define FDCAN_TX_BUFFER27 ((uint32_t)0x08000000U) //!< Add message to Tx Buffer 27
#define FDCAN_TX_BUFFER28 ((uint32_t)0x10000000U) //!< Add message to Tx Buffer 28
#define FDCAN_TX_BUFFER29 ((uint32_t)0x20000000U) //!< Add message to Tx Buffer 29
#define FDCAN_TX_BUFFER30 ((uint32_t)0x40000000U) //!< Add message to Tx Buffer 30
#define FDCAN_TX_BUFFER31 ((uint32_t)0x80000000U) //!< Add message to Tx Buffer 31

#define FDCAN_RX_FIFO0    ((uint32_t)0x00000040U) //!< Get received message from Rx FIFO 0
#define FDCAN_RX_FIFO1    ((uint32_t)0x00000041U) //!< Get received message from Rx FIFO 1
#define FDCAN_RX_BUFFER0  ((uint32_t)0x00000000U) //!< Get received message from Rx Buffer 0
#define FDCAN_RX_BUFFER1  ((uint32_t)0x00000001U) //!< Get received message from Rx Buffer 1
#define FDCAN_RX_BUFFER2  ((uint32_t)0x00000002U) //!< Get received message from Rx Buffer 2
#define FDCAN_RX_BUFFER3  ((uint32_t)0x00000003U) //!< Get received message from Rx Buffer 3
#define FDCAN_RX_BUFFER4  ((uint32_t)0x00000004U) //!< Get received message from Rx Buffer 4
#define FDCAN_RX_BUFFER5  ((uint32_t)0x00000005U) //!< Get received message from Rx Buffer 5
#define FDCAN_RX_BUFFER6  ((uint32_t)0x00000006U) //!< Get received message from Rx Buffer 6
#define FDCAN_RX_BUFFER7  ((uint32_t)0x00000007U) //!< Get received message from Rx Buffer 7
#define FDCAN_RX_BUFFER8  ((uint32_t)0x00000008U) //!< Get received message from Rx Buffer 8
#define FDCAN_RX_BUFFER9  ((uint32_t)0x00000009U) //!< Get received message from Rx Buffer 9
#define FDCAN_RX_BUFFER10 ((uint32_t)0x0000000AU) //!< Get received message from Rx Buffer 10
#define FDCAN_RX_BUFFER11 ((uint32_t)0x0000000BU) //!< Get received message from Rx Buffer 11
#define FDCAN_RX_BUFFER12 ((uint32_t)0x0000000CU) //!< Get received message from Rx Buffer 12
#define FDCAN_RX_BUFFER13 ((uint32_t)0x0000000DU) //!< Get received message from Rx Buffer 13
#define FDCAN_RX_BUFFER14 ((uint32_t)0x0000000EU) //!< Get received message from Rx Buffer 14
#define FDCAN_RX_BUFFER15 ((uint32_t)0x0000000FU) //!< Get received message from Rx Buffer 15
#define FDCAN_RX_BUFFER16 ((uint32_t)0x00000010U) //!< Get received message from Rx Buffer 16
#define FDCAN_RX_BUFFER17 ((uint32_t)0x00000011U) //!< Get received message from Rx Buffer 17
#define FDCAN_RX_BUFFER18 ((uint32_t)0x00000012U) //!< Get received message from Rx Buffer 18
#define FDCAN_RX_BUFFER19 ((uint32_t)0x00000013U) //!< Get received message from Rx Buffer 19
#define FDCAN_RX_BUFFER20 ((uint32_t)0x00000014U) //!< Get received message from Rx Buffer 20
#define FDCAN_RX_BUFFER21 ((uint32_t)0x00000015U) //!< Get received message from Rx Buffer 21
#define FDCAN_RX_BUFFER22 ((uint32_t)0x00000016U) //!< Get received message from Rx Buffer 22
#define FDCAN_RX_BUFFER23 ((uint32_t)0x00000017U) //!< Get received message from Rx Buffer 23
#define FDCAN_RX_BUFFER24 ((uint32_t)0x00000018U) //!< Get received message from Rx Buffer 24
#define FDCAN_RX_BUFFER25 ((uint32_t)0x00000019U) //!< Get received message from Rx Buffer 25
#define FDCAN_RX_BUFFER26 ((uint32_t)0x0000001AU) //!< Get received message from Rx Buffer 26
#define FDCAN_RX_BUFFER27 ((uint32_t)0x0000001BU) //!< Get received message from Rx Buffer 27
#define FDCAN_RX_BUFFER28 ((uint32_t)0x0000001CU) //!< Get received message from Rx Buffer 28
#define FDCAN_RX_BUFFER29 ((uint32_t)0x0000001DU) //!< Get received message from Rx Buffer 29
#define FDCAN_RX_BUFFER30 ((uint32_t)0x0000001EU) //!< Get received message from Rx Buffer 30
#define FDCAN_RX_BUFFER31 ((uint32_t)0x0000001FU) //!< Get received message from Rx Buffer 31
#define FDCAN_RX_BUFFER32 ((uint32_t)0x00000020U) //!< Get received message from Rx Buffer 32
#define FDCAN_RX_BUFFER33 ((uint32_t)0x00000021U) //!< Get received message from Rx Buffer 33
#define FDCAN_RX_BUFFER34 ((uint32_t)0x00000022U) //!< Get received message from Rx Buffer 34
#define FDCAN_RX_BUFFER35 ((uint32_t)0x00000023U) //!< Get received message from Rx Buffer 35
#define FDCAN_RX_BUFFER36 ((uint32_t)0x00000024U) //!< Get received message from Rx Buffer 36
#define FDCAN_RX_BUFFER37 ((uint32_t)0x00000025U) //!< Get received message from Rx Buffer 37
#define FDCAN_RX_BUFFER38 ((uint32_t)0x00000026U) //!< Get received message from Rx Buffer 38
#define FDCAN_RX_BUFFER39 ((uint32_t)0x00000027U) //!< Get received message from Rx Buffer 39
#define FDCAN_RX_BUFFER40 ((uint32_t)0x00000028U) //!< Get received message from Rx Buffer 40
#define FDCAN_RX_BUFFER41 ((uint32_t)0x00000029U) //!< Get received message from Rx Buffer 41
#define FDCAN_RX_BUFFER42 ((uint32_t)0x0000002AU) //!< Get received message from Rx Buffer 42
#define FDCAN_RX_BUFFER43 ((uint32_t)0x0000002BU) //!< Get received message from Rx Buffer 43
#define FDCAN_RX_BUFFER44 ((uint32_t)0x0000002CU) //!< Get received message from Rx Buffer 44
#define FDCAN_RX_BUFFER45 ((uint32_t)0x0000002DU) //!< Get received message from Rx Buffer 45
#define FDCAN_RX_BUFFER46 ((uint32_t)0x0000002EU) //!< Get received message from Rx Buffer 46
#define FDCAN_RX_BUFFER47 ((uint32_t)0x0000002FU) //!< Get received message from Rx Buffer 47
#define FDCAN_RX_BUFFER48 ((uint32_t)0x00000030U) //!< Get received message from Rx Buffer 48
#define FDCAN_RX_BUFFER49 ((uint32_t)0x00000031U) //!< Get received message from Rx Buffer 49
#define FDCAN_RX_BUFFER50 ((uint32_t)0x00000032U) //!< Get received message from Rx Buffer 50
#define FDCAN_RX_BUFFER51 ((uint32_t)0x00000033U) //!< Get received message from Rx Buffer 51
#define FDCAN_RX_BUFFER52 ((uint32_t)0x00000034U) //!< Get received message from Rx Buffer 52
#define FDCAN_RX_BUFFER53 ((uint32_t)0x00000035U) //!< Get received message from Rx Buffer 53
#define FDCAN_RX_BUFFER54 ((uint32_t)0x00000036U) //!< Get received message from Rx Buffer 54
#define FDCAN_RX_BUFFER55 ((uint32_t)0x00000037U) //!< Get received message from Rx Buffer 55
#define FDCAN_RX_BUFFER56 ((uint32_t)0x00000038U) //!< Get received message from Rx Buffer 56
#define FDCAN_RX_BUFFER57 ((uint32_t)0x00000039U) //!< Get received message from Rx Buffer 57
#define FDCAN_RX_BUFFER58 ((uint32_t)0x0000003AU) //!< Get received message from Rx Buffer 58
#define FDCAN_RX_BUFFER59 ((uint32_t)0x0000003BU) //!< Get received message from Rx Buffer 59
#define FDCAN_RX_BUFFER60 ((uint32_t)0x0000003CU) //!< Get received message from Rx Buffer 60
#define FDCAN_RX_BUFFER61 ((uint32_t)0x0000003DU) //!< Get received message from Rx Buffer 61
#define FDCAN_RX_BUFFER62 ((uint32_t)0x0000003EU) //!< Get received message from Rx Buffer 62
#define FDCAN_RX_BUFFER63 ((uint32_t)0x0000003FU) //!< Get received message from Rx Buffer 63

#define FDCAN_TX_EVENT             ((uint32_t)0x00400000U) //!< Tx event
#define FDCAN_TX_IN_SPITE_OF_ABORT ((uint32_t)0x00800000U) //!< Transmission in spite of cancellation

#define FDCAN_HP_STORAGE_NO_FIFO  ((uint32_t)0x00000000U) //!< No FIFO selected
#define FDCAN_HP_STORAGE_MSG_LOST ((uint32_t)0x00000040U) //!< FIFO message lost
#define FDCAN_HP_STORAGE_RXFIFO0  ((uint32_t)0x00000080U) //!< Message stored in FIFO 0
#define FDCAN_HP_STORAGE_RXFIFO1  ((uint32_t)0x000000C0U) //!< Message stored in FIFO 1

#define FDCAN_PROTOCOL_ERROR_NONE      ((uint32_t)0x00000000U) //!< No error occurred
#define FDCAN_PROTOCOL_ERROR_STUFF     ((uint32_t)0x00000001U) //!< Stuff error
#define FDCAN_PROTOCOL_ERROR_FORM      ((uint32_t)0x00000002U) //!< Form error
#define FDCAN_PROTOCOL_ERROR_ACK       ((uint32_t)0x00000003U) //!< Acknowledge error
#define FDCAN_PROTOCOL_ERROR_BIT1      ((uint32_t)0x00000004U) //!< Bit 1 (recessive) error
#define FDCAN_PROTOCOL_ERROR_BIT0      ((uint32_t)0x00000005U) //!< Bit 0 (dominant) error
#define FDCAN_PROTOCOL_ERROR_CRC       ((uint32_t)0x00000006U) //!< CRC check sum error
#define FDCAN_PROTOCOL_ERROR_NO_CHANGE ((uint32_t)0x00000007U) //!< No change since last read

#define FDCAN_COM_STATE_SYNC ((uint32_t)0x00000000U) //!< Node is synchronizing on CAN communication
#define FDCAN_COM_STATE_IDLE ((uint32_t)0x00000008U) //!< Node is neither receiver nor transmitter
#define FDCAN_COM_STATE_RX   ((uint32_t)0x00000010U) //!< Node is operating as receiver
#define FDCAN_COM_STATE_TX   ((uint32_t)0x00000018U) //!< Node is operating as transmitter

#define FDCAN_CFG_TX_EVENT_FIFO ((uint32_t)0x00000000U) //!< Tx event FIFO
#define FDCAN_CFG_RX_FIFO0      ((uint32_t)0x00000001U) //!< Rx FIFO0
#define FDCAN_CFG_RX_FIFO1      ((uint32_t)0x00000002U) //!< Rx FIFO1

#define FDCAN_RX_FIFO_BLOCKING  ((uint32_t)0x00000000U) //!< Rx FIFO blocking mode
#define FDCAN_RX_FIFO_OVERWRITE ((uint32_t)0x80000000U) //!< Rx FIFO overwrite mode

#define FDCAN_ACCEPT_IN_RX_FIFO0 ((uint32_t)0x00000000U) //!< Accept in Rx FIFO 0
#define FDCAN_ACCEPT_IN_RX_FIFO1 ((uint32_t)0x00000001U) //!< Accept in Rx FIFO 1
#define FDCAN_REJECT             ((uint32_t)0x00000002U) //!< Reject

#define FDCAN_FILTER_REMOTE ((uint32_t)0x00000000U) //!< Filter remote frames
#define FDCAN_REJECT_REMOTE ((uint32_t)0x00000001U) //!< Reject all remote frames

#define FDCAN_INTERRUPT_LINE0 ((uint32_t)0x00000001U) //!< Interrupt Line 0
#define FDCAN_INTERRUPT_LINE1 ((uint32_t)0x00000002U) //!< Interrupt Line 1

#define FDCAN_TIMESTAMP_INTERNAL ((uint32_t)0x00000001U) //!< Timestamp counter value incremented according to TCP
#define FDCAN_TIMESTAMP_EXTERNAL ((uint32_t)0x00000002U) //!< External timestamp counter value used

#define FDCAN_TIMESTAMP_PRESC_1  ((uint32_t)0x00000000U) //!< Timestamp counter time unit in equal to CAN bit time
#define FDCAN_TIMESTAMP_PRESC_2  ((uint32_t)0x00010000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 2
#define FDCAN_TIMESTAMP_PRESC_3  ((uint32_t)0x00020000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 3
#define FDCAN_TIMESTAMP_PRESC_4  ((uint32_t)0x00030000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 4
#define FDCAN_TIMESTAMP_PRESC_5  ((uint32_t)0x00040000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 5
#define FDCAN_TIMESTAMP_PRESC_6  ((uint32_t)0x00050000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 6
#define FDCAN_TIMESTAMP_PRESC_7  ((uint32_t)0x00060000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 7
#define FDCAN_TIMESTAMP_PRESC_8  ((uint32_t)0x00070000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 8
#define FDCAN_TIMESTAMP_PRESC_9  ((uint32_t)0x00080000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 9
#define FDCAN_TIMESTAMP_PRESC_10 ((uint32_t)0x00090000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 10
#define FDCAN_TIMESTAMP_PRESC_11 ((uint32_t)0x000A0000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 11
#define FDCAN_TIMESTAMP_PRESC_12 ((uint32_t)0x000B0000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 12
#define FDCAN_TIMESTAMP_PRESC_13 ((uint32_t)0x000C0000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 13
#define FDCAN_TIMESTAMP_PRESC_14 ((uint32_t)0x000D0000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 14
#define FDCAN_TIMESTAMP_PRESC_15 ((uint32_t)0x000E0000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 15
#define FDCAN_TIMESTAMP_PRESC_16 ((uint32_t)0x000F0000U) //!< Timestamp counter time unit in equal to CAN bit time multipled by 16

#define FDCAN_TIMEOUT_CONTINUOUS    ((uint32_t)0x00000000U) //!< Timeout continuous operation
#define FDCAN_TIMEOUT_TX_EVENT_FIFO ((uint32_t)0x00000002U) //!< Timeout controlled by Tx Event FIFO
#define FDCAN_TIMEOUT_RX_FIFO0      ((uint32_t)0x00000004U) //!< Timeout controlled by Rx FIFO 0
#define FDCAN_TIMEOUT_RX_FIFO1      ((uint32_t)0x00000006U) //!< Timeout controlled by Rx FIFO 1

#define FDCAN_TT_REF_MESSAGE_NO_PAYLOAD  ((uint32_t)0x00000000U)      //!< Reference message has no additional payload
#define FDCAN_TT_REF_MESSAGE_ADD_PAYLOAD ((uint32_t)FDCAN_TTRMC_RMPS) //!< Additional payload is taken from Tx Buffer 0

#define FDCAN_TT_REPEAT_EVERY_CYCLE      ((uint32_t)0x00000000U) //!< Trigger valid for all cycles
#define FDCAN_TT_REPEAT_EVERY_2ND_CYCLE  ((uint32_t)0x00000002U) //!< Trigger valid every 2dn cycle
#define FDCAN_TT_REPEAT_EVERY_4TH_CYCLE  ((uint32_t)0x00000004U) //!< Trigger valid every 4th cycle
#define FDCAN_TT_REPEAT_EVERY_8TH_CYCLE  ((uint32_t)0x00000008U) //!< Trigger valid every 8th cycle
#define FDCAN_TT_REPEAT_EVERY_16TH_CYCLE ((uint32_t)0x00000010U) //!< Trigger valid every 16th cycle
#define FDCAN_TT_REPEAT_EVERY_32ND_CYCLE ((uint32_t)0x00000020U) //!< Trigger valid every 32nd cycle
#define FDCAN_TT_REPEAT_EVERY_64TH_CYCLE ((uint32_t)0x00000040U) //!< Trigger valid every 64th cycle

#define FDCAN_TT_TX_REF_TRIGGER         ((uint32_t)0x00000000U) //!< Transmit reference message in strictly time-triggered operation
#define FDCAN_TT_TX_REF_TRIGGER_GAP     ((uint32_t)0x00000001U) //!< Transmit reference message in external event-synchronized time-triggered operation
#define FDCAN_TT_TX_TRIGGER_SINGLE      ((uint32_t)0x00000002U) //!< Start a single transmission in an exclusive time window
#define FDCAN_TT_TX_TRIGGER_CONTINUOUS  ((uint32_t)0x00000003U) //!< Start a continuous transmission in an exclusive time window
#define FDCAN_TT_TX_TRIGGER_ARBITRATION ((uint32_t)0x00000004U) //!< Start a transmission in an arbitration time window
#define FDCAN_TT_TX_TRIGGER_MERGED      ((uint32_t)0x00000005U) //!< Start a merged arbitration window
#define FDCAN_TT_WATCH_TRIGGER          ((uint32_t)0x00000006U) //!< Check for missing reference messages in strictly time-triggered operation
#define FDCAN_TT_WATCH_TRIGGER_GAP      ((uint32_t)0x00000007U) //!< Check for missing reference messages in external event-synchronized time-triggered operation
#define FDCAN_TT_RX_TRIGGER             ((uint32_t)0x00000008U) //!< Check for the reception of periodic messages in exclusive time windows
#define FDCAN_TT_TIME_BASE_TRIGGER      ((uint32_t)0x00000009U) //!< Generate internal/external events depending on TmEventInt/TmEventExt configuration
#define FDCAN_TT_END_OF_LIST            ((uint32_t)0x0000000AU) //!< Illegal trigger, to be assigned to the unused triggers after a FDCAN_TT_WATCH_TRIGGER or FDCAN_TT_WATCH_TRIGGER_GAP

#define FDCAN_TT_TM_NO_INTERNAL_EVENT  ((uint32_t)0x00000000U) //!< No action
#define FDCAN_TT_TM_GEN_INTERNAL_EVENT ((uint32_t)0x00000020U) //!< Internal event is generated when trigger becomes active

#define FDCAN_TT_TM_NO_EXTERNAL_EVENT  ((uint32_t)0x00000000U) //!< No action
#define FDCAN_TT_TM_GEN_EXTERNAL_EVENT ((uint32_t)0x00000010U) //!< External event (pulse) is generated when trigger becomes active

#define FDCAN_TT_COMMUNICATION_LEVEL1    ((uint32_t)0x00000001U) //!< Time triggered communication, level 1
#define FDCAN_TT_COMMUNICATION_LEVEL2    ((uint32_t)0x00000002U) //!< Time triggered communication, level 2
#define FDCAN_TT_COMMUNICATION_LEVEL0    ((uint32_t)0x00000003U) //!< Time triggered communication, level 0

#define FDCAN_STRICTLY_TT_OPERATION     ((uint32_t)0x00000000U)     //!< Strictly time-triggered operation
#define FDCAN_EXT_EVT_SYNC_TT_OPERATION ((uint32_t)FDCAN_TTOCF_GEN) //!< External event-synchronized time-triggered operation

#define FDCAN_TT_SLAVE            ((uint32_t)0x00000000U)    //!< Time slave
#define FDCAN_TT_POTENTIAL_MASTER ((uint32_t)FDCAN_TTOCF_TM) //!< Potential time master

#define FDCAN_TT_EXT_CLK_SYNC_DISABLE ((uint32_t)0x00000000U)      //!< External clock synchronization in Level 0,2 disabled
#define FDCAN_TT_EXT_CLK_SYNC_ENABLE  ((uint32_t)FDCAN_TTOCF_EECS) //!< External clock synchronization in Level 0,2 enabled

#define FDCAN_TT_GLOB_TIME_FILT_DISABLE ((uint32_t)0x00000000U)      //!< Global time filtering in Level 0,2 disabled
#define FDCAN_TT_GLOB_TIME_FILT_ENABLE  ((uint32_t)FDCAN_TTOCF_EGTF) //!< Global time filtering in Level 0,2 enabled

#define FDCAN_TT_AUTO_CLK_CALIB_DISABLE ((uint32_t)0x00000000U)     //!< Automatic clock calibration in Level 0,2 disabled
#define FDCAN_TT_AUTO_CLK_CALIB_ENABLE  ((uint32_t)FDCAN_TTOCF_ECC) //!< Automatic clock calibration in Level 0,2 enabled

#define FDCAN_TT_EVT_TRIG_POL_RISING  ((uint32_t)0x00000000U)      //!< Rising edge trigger
#define FDCAN_TT_EVT_TRIG_POL_FALLING ((uint32_t)FDCAN_TTOCF_EVTP) //!< Falling edge trigger

#define FDCAN_TT_CYCLES_PER_MATRIX_1  ((uint32_t)0x00000000U) //!< 1 Basic Cycle per Matrix
#define FDCAN_TT_CYCLES_PER_MATRIX_2  ((uint32_t)0x00000001U) //!< 2 Basic Cycles per Matrix
#define FDCAN_TT_CYCLES_PER_MATRIX_4  ((uint32_t)0x00000003U) //!< 4 Basic Cycles per Matrix
#define FDCAN_TT_CYCLES_PER_MATRIX_8  ((uint32_t)0x00000007U) //!< 8 Basic Cycles per Matrix
#define FDCAN_TT_CYCLES_PER_MATRIX_16 ((uint32_t)0x0000000FU) //!< 16 Basic Cycles per Matrix
#define FDCAN_TT_CYCLES_PER_MATRIX_32 ((uint32_t)0x0000001FU) //!< 32 Basic Cycles per Matrix
#define FDCAN_TT_CYCLES_PER_MATRIX_64 ((uint32_t)0x0000003FU) //!< 64 Basic Cycles per Matrix

#define FDCAN_TT_NO_SYNC_PULSE          ((uint32_t)0x00000000U) //!< No sync pulse
#define FDCAN_TT_SYNC_BASIC_CYCLE_START ((uint32_t)0x00000040U) //!< Sync pulse at start of basic cycle
#define FDCAN_TT_SYNC_MATRIX_START      ((uint32_t)0x00000080U) //!< Sync pulse at start of matrix

#define FDCAN_TT_STOP_WATCH_TRIGGER_0 ((uint32_t)0x00000000U) //!< TIM2 selected as stop watch trigger
#define FDCAN_TT_STOP_WATCH_TRIGGER_1 ((uint32_t)0x00000001U) //!< TIM3 selected as stop watch trigger
#define FDCAN_TT_STOP_WATCH_TRIGGER_2 ((uint32_t)0x00000002U) //!< ETH selected as stop watch trigger
#define FDCAN_TT_STOP_WATCH_TRIGGER_3 ((uint32_t)0x00000003U) //!< HRTIM selected as stop watch trigger

#define FDCAN_TT_EVENT_TRIGGER_0 ((uint32_t)0x00000000U) //!< TIM2 selected as event trigger
#define FDCAN_TT_EVENT_TRIGGER_1 ((uint32_t)0x00000010U) //!< TIM3 selected as event trigger
#define FDCAN_TT_EVENT_TRIGGER_2 ((uint32_t)0x00000020U) //!< ETH selected as event trigger
#define FDCAN_TT_EVENT_TRIGGER_3 ((uint32_t)0x00000030U) //!< HRTIM selected as event trigger

#define FDCAN_TT_STOP_WATCH_DISABLED    ((uint32_t)0x00000000U) //!< Stop Watch disabled
#define FDCAN_TT_STOP_WATCH_CYCLE_TIME  ((uint32_t)0x00000008U) //!< Actual value of cycle time is copied to Capture Time register (TTCPT.SWV)
#define FDCAN_TT_STOP_WATCH_LOCAL_TIME  ((uint32_t)0x00000010U) //!< Actual value of local time is copied to Capture Time register (TTCPT.SWV)
#define FDCAN_TT_STOP_WATCH_GLOBAL_TIME ((uint32_t)0x00000018U) //!< Actual value of global time is copied to Capture Time register (TTCPT.SWV)

#define FDCAN_TT_STOP_WATCH_RISING  ((uint32_t)0x00000000U) //!< Selected stop watch source is captured at rising edge of fdcan1_swt
#define FDCAN_TT_STOP_WATCH_FALLING ((uint32_t)0x00000004U) //!< Selected stop watch source is captured at falling edge of fdcan1_swt

#define FDCAN_TT_REG_TIMEMARK_DIABLED  ((uint32_t)0x00000000U) //!< No Register Time Mark Interrupt generated
#define FDCAN_TT_REG_TIMEMARK_CYC_TIME ((uint32_t)0x00000040U) //!< Register Time Mark Interrupt if Time Mark = cycle time
#define FDCAN_TT_REG_TIMEMARK_LOC_TIME ((uint32_t)0x00000080U) //!< Register Time Mark Interrupt if Time Mark = local time
#define FDCAN_TT_REG_TIMEMARK_GLO_TIME ((uint32_t)0x000000C0U) //!< Register Time Mark Interrupt if Time Mark = global time

#define FDCAN_TT_NO_ERROR     ((uint32_t)0x00000000U) //!< Severity 0 - No Error
#define FDCAN_TT_WARNING      ((uint32_t)0x00000001U) //!< Severity 1 - Warning
#define FDCAN_TT_ERROR        ((uint32_t)0x00000002U) //!< Severity 2 - Error
#define FDCAN_TT_SEVERE_ERROR ((uint32_t)0x00000003U) //!< Severity 3 - Severe Error

#define FDCAN_TT_MASTER_OFF          ((uint32_t)0x00000000U) //!< Master_Off, no master properties relevant
#define FDCAN_TT_TIME_SLAVE          ((uint32_t)0x00000004U) //!< Operating as Time Slave
#define FDCAN_TT_BACKUP_TIME_MASTER  ((uint32_t)0x00000008U) //!< Operating as Backup Time Master
#define FDCAN_TT_CURRENT_TIME_MASTER ((uint32_t)0x0000000CU) //!< Operating as current Time Master

#define FDCAN_TT_OUT_OF_SYNC   ((uint32_t)0x00000000U) //!< Out of Synchronization
#define FDCAN_TT_SYNCHRONIZING ((uint32_t)0x00000010U) //!< Synchronizing to communication
#define FDCAN_TT_IN_GAP        ((uint32_t)0x00000020U) //!< Schedule suspended by Gap
#define FDCAN_TT_IN_SCHEDULE   ((uint32_t)0x00000030U) //!< Synchronized to schedule

#define FDCAN_IR_MASK ((uint32_t)0x3FCFFFFFU) //!< FDCAN interrupts mask
#define CCU_IR_MASK   ((uint32_t)0xC0000000U) //!< CCU interrupts mask

#define FDCAN_FLAG_TX_COMPLETE             FDCAN_IR_TC             //!< Transmission Completed
#define FDCAN_FLAG_TX_ABORT_COMPLETE       FDCAN_IR_TCF            //!< Transmission Cancellation Finished
#define FDCAN_FLAG_TX_FIFO_EMPTY           FDCAN_IR_TFE            //!< Tx FIFO Empty
#define FDCAN_FLAG_RX_HIGH_PRIORITY_MSG    FDCAN_IR_HPM            //!< High priority message received
#define FDCAN_FLAG_RX_BUFFER_NEW_MESSAGE   FDCAN_IR_DRX            //!< At least one received message stored into a Rx Buffer
#define FDCAN_FLAG_TX_EVT_FIFO_ELT_LOST    FDCAN_IR_TEFL           //!< Tx Event FIFO element lost
#define FDCAN_FLAG_TX_EVT_FIFO_FULL        FDCAN_IR_TEFF           //!< Tx Event FIFO full
#define FDCAN_FLAG_TX_EVT_FIFO_WATERMARK   FDCAN_IR_TEFW           //!< Tx Event FIFO fill level reached watermark
#define FDCAN_FLAG_TX_EVT_FIFO_NEW_DATA    FDCAN_IR_TEFN           //!< Tx Handler wrote Tx Event FIFO element
#define FDCAN_FLAG_RX_FIFO0_MESSAGE_LOST   FDCAN_IR_RF0L           //!< Rx FIFO 0 message lost
#define FDCAN_FLAG_RX_FIFO0_FULL           FDCAN_IR_RF0F           //!< Rx FIFO 0 full
#define FDCAN_FLAG_RX_FIFO0_WATERMARK      FDCAN_IR_RF0W           //!< Rx FIFO 0 fill level reached watermark
#define FDCAN_FLAG_RX_FIFO0_NEW_MESSAGE    FDCAN_IR_RF0N           //!< New message written to Rx FIFO 0
#define FDCAN_FLAG_RX_FIFO1_MESSAGE_LOST   FDCAN_IR_RF1L           //!< Rx FIFO 1 message lost
#define FDCAN_FLAG_RX_FIFO1_FULL           FDCAN_IR_RF1F           //!< Rx FIFO 1 full
#define FDCAN_FLAG_RX_FIFO1_WATERMARK      FDCAN_IR_RF1W           //!< Rx FIFO 1 fill level reached watermark
#define FDCAN_FLAG_RX_FIFO1_NEW_MESSAGE    FDCAN_IR_RF1N           //!< New message written to Rx FIFO 1
#define FDCAN_FLAG_RAM_ACCESS_FAILURE      FDCAN_IR_MRAF           //!< Message RAM access failure occurred
#define FDCAN_FLAG_ERROR_LOGGING_OVERFLOW  FDCAN_IR_ELO            //!< Overflow of FDCAN Error Logging Counter occurred
#define FDCAN_FLAG_ERROR_PASSIVE           FDCAN_IR_EP             //!< Error_Passive status changed
#define FDCAN_FLAG_ERROR_WARNING           FDCAN_IR_EW             //!< Error_Warning status changed
#define FDCAN_FLAG_BUS_OFF                 FDCAN_IR_BO             //!< Bus_Off status changed
#define FDCAN_FLAG_RAM_WATCHDOG            FDCAN_IR_WDI            //!< Message RAM Watchdog event due to missing READY
#define FDCAN_FLAG_ARB_PROTOCOL_ERROR      FDCAN_IR_PEA            //!< Protocol error in arbitration phase detected
#define FDCAN_FLAG_DATA_PROTOCOL_ERROR     FDCAN_IR_PED            //!< Protocol error in data phase detected
#define FDCAN_FLAG_RESERVED_ADDRESS_ACCESS FDCAN_IR_ARA            //!< Access to reserved address occurred
#define FDCAN_FLAG_TIMESTAMP_WRAPAROUND    FDCAN_IR_TSW            //!< Timestamp counter wrapped around
#define FDCAN_FLAG_TIMEOUT_OCCURRED        FDCAN_IR_TOO            //!< Timeout reached
#define FDCAN_FLAG_CALIB_STATE_CHANGED     (FDCANCCU_IR_CSC << 30) //!< Clock calibration state changed
#define FDCAN_FLAG_CALIB_WATCHDOG_EVENT    (FDCANCCU_IR_CWE << 30) //!< Clock calibration watchdog event occurred

#define FDCAN_IT_TX_COMPLETE           FDCAN_IE_TCE   //!< Transmission Completed
#define FDCAN_IT_TX_ABORT_COMPLETE     FDCAN_IE_TCFE  //!< Transmission Cancellation Finished
#define FDCAN_IT_TX_FIFO_EMPTY         FDCAN_IE_TFEE  //!< Tx FIFO Empty

#define FDCAN_IT_RX_HIGH_PRIORITY_MSG  FDCAN_IE_HPME  //!< High priority message received
#define FDCAN_IT_RX_BUFFER_NEW_MESSAGE FDCAN_IE_DRXE  //!< At least one received message stored into a Rx Buffer

#define FDCAN_IT_TIMESTAMP_WRAPAROUND  FDCAN_IE_TSWE  //!< Timestamp counter wrapped around
#define FDCAN_IT_TIMEOUT_OCCURRED      FDCAN_IE_TOOE  //!< Timeout reached

#define FDCAN_IT_CALIB_STATE_CHANGED  (FDCANCCU_IE_CSCE << 30) //!< Clock calibration state changed
#define FDCAN_IT_CALIB_WATCHDOG_EVENT (FDCANCCU_IE_CWEE << 30) //!< Clock calibration watchdog event occurred

#define FDCAN_IT_TX_EVT_FIFO_ELT_LOST  FDCAN_IE_TEFLE //!< Tx Event FIFO element lost
#define FDCAN_IT_TX_EVT_FIFO_FULL      FDCAN_IE_TEFFE //!< Tx Event FIFO full
#define FDCAN_IT_TX_EVT_FIFO_WATERMARK FDCAN_IE_TEFWE //!< Tx Event FIFO fill level reached watermark
#define FDCAN_IT_TX_EVT_FIFO_NEW_DATA  FDCAN_IE_TEFNE //!< Tx Handler wrote Tx Event FIFO element

#define FDCAN_IT_RX_FIFO0_MESSAGE_LOST FDCAN_IE_RF0LE //!< Rx FIFO 0 message lost
#define FDCAN_IT_RX_FIFO0_FULL         FDCAN_IE_RF0FE //!< Rx FIFO 0 full
#define FDCAN_IT_RX_FIFO0_WATERMARK    FDCAN_IE_RF0WE //!< Rx FIFO 0 fill level reached watermark
#define FDCAN_IT_RX_FIFO0_NEW_MESSAGE  FDCAN_IE_RF0NE //!< New message written to Rx FIFO 0

#define FDCAN_IT_RX_FIFO1_MESSAGE_LOST FDCAN_IE_RF1LE //!< Rx FIFO 1 message lost
#define FDCAN_IT_RX_FIFO1_FULL         FDCAN_IE_RF1FE //!< Rx FIFO 1 full
#define FDCAN_IT_RX_FIFO1_WATERMARK    FDCAN_IE_RF1WE //!< Rx FIFO 1 fill level reached watermark
#define FDCAN_IT_RX_FIFO1_NEW_MESSAGE  FDCAN_IE_RF1NE //!< New message written to Rx FIFO 1

#define FDCAN_IT_RAM_ACCESS_FAILURE      FDCAN_IE_MRAFE //!< Message RAM access failure occurred
#define FDCAN_IT_ERROR_LOGGING_OVERFLOW  FDCAN_IE_ELOE  //!< Overflow of FDCAN Error Logging Counter occurred
#define FDCAN_IT_RAM_WATCHDOG            FDCAN_IE_WDIE  //!< Message RAM Watchdog event due to missing READY
#define FDCAN_IT_ARB_PROTOCOL_ERROR      FDCAN_IE_PEAE  //!< Protocol error in arbitration phase detected
#define FDCAN_IT_DATA_PROTOCOL_ERROR     FDCAN_IE_PEDE  //!< Protocol error in data phase detected
#define FDCAN_IT_RESERVED_ADDRESS_ACCESS FDCAN_IE_ARAE  //!< Access to reserved address occurred

#define FDCAN_IT_ERROR_PASSIVE FDCAN_IE_EPE //!< Error_Passive status changed
#define FDCAN_IT_ERROR_WARNING FDCAN_IE_EWE //!< Error_Warning status changed
#define FDCAN_IT_BUS_OFF       FDCAN_IE_BOE //!< Bus_Off status changed

#define FDCAN_TT_FLAG_BASIC_CYCLE_START    FDCAN_TTIR_SBC  //!< Start of Basic Cycle
#define FDCAN_TT_FLAG_MATRIX_CYCLE_START   FDCAN_TTIR_SMC  //!< Start of Matrix Cycle
#define FDCAN_TT_FLAG_SYNC_MODE_CHANGE     FDCAN_TTIR_CSM  //!< Change of Synchronization Mode
#define FDCAN_TT_FLAG_START_OF_GAP         FDCAN_TTIR_SOG  //!< Start of Gap
#define FDCAN_TT_FLAG_REG_TIME_MARK        FDCAN_TTIR_RTMI //!< Register Time Mark Interrupt
#define FDCAN_TT_FLAG_TRIG_TIME_MARK       FDCAN_TTIR_TTMI //!< Trigger Time Mark Event Internal
#define FDCAN_TT_FLAG_STOP_WATCH           FDCAN_TTIR_SWE  //!< Stop Watch Event
#define FDCAN_TT_FLAG_GLOBAL_TIME_WRAP     FDCAN_TTIR_GTW  //!< Global Time Wrap
#define FDCAN_TT_FLAG_GLOBAL_TIME_DISC     FDCAN_TTIR_GTD  //!< Global Time Discontinuity
#define FDCAN_TT_FLAG_GLOBAL_TIME_ERROR    FDCAN_TTIR_GTE  //!< Global Time Error
#define FDCAN_TT_FLAG_TX_COUNT_UNDERFLOW   FDCAN_TTIR_TXU  //!< Tx Count Underflow
#define FDCAN_TT_FLAG_TX_COUNT_OVERFLOW    FDCAN_TTIR_TXO  //!< Tx Count Overflow
#define FDCAN_TT_FLAG_SCHEDULING_ERROR_1   FDCAN_TTIR_SE1  //!< Scheduling Error 1
#define FDCAN_TT_FLAG_SCHEDULING_ERROR_2   FDCAN_TTIR_SE2  //!< Scheduling Error 2
#define FDCAN_TT_FLAG_ERROR_LEVEL_CHANGE   FDCAN_TTIR_ELC  //!< Error Level Changed
#define FDCAN_TT_FLAG_INIT_WATCH_TRIGGER   FDCAN_TTIR_IWT  //!< Initialization Watch Trigger
#define FDCAN_TT_FLAG_WATCH_TRIGGER        FDCAN_TTIR_WT   //!< Watch Trigger
#define FDCAN_TT_FLAG_APPLICATION_WATCHDOG FDCAN_TTIR_AW   //!< Application Watchdog
#define FDCAN_TT_FLAG_CONFIG_ERROR         FDCAN_TTIR_CER  //!< Configuration Error

#define FDCAN_TT_IT_BASIC_CYCLE_START  FDCAN_TTIE_SBCE //!< Start of Basic Cycle
#define FDCAN_TT_IT_MATRIX_CYCLE_START FDCAN_TTIE_SMCE //!< Start of Matrix Cycle
#define FDCAN_TT_IT_SYNC_MODE_CHANGE   FDCAN_TTIE_CSME //!< Change of Synchronization Mode
#define FDCAN_TT_IT_START_OF_GAP       FDCAN_TTIE_SOGE //!< Start of Gap

#define FDCAN_TT_IT_REG_TIME_MARK  FDCAN_TTIE_RTMIE //!< Register Time Mark Interrupt
#define FDCAN_TT_IT_TRIG_TIME_MARK FDCAN_TTIE_TTMIE //!< Trigger Time Mark Event Internal

#define FDCAN_TT_IT_STOP_WATCH FDCAN_TTIE_SWEE //!< Stop Watch Event

#define FDCAN_TT_IT_GLOBAL_TIME_WRAP FDCAN_TTIE_GTWE  //!< Global Time Wrap
#define FDCAN_TT_IT_GLOBAL_TIME_DISC FDCAN_TTIE_GTDE  //!< Global Time Discontinuity

#define FDCAN_TT_IT_GLOBAL_TIME_ERROR  FDCAN_TTIE_GTEE //!< Global Time Error
#define FDCAN_TT_IT_TX_COUNT_UNDERFLOW FDCAN_TTIE_TXUE //!< Tx Count Underflow
#define FDCAN_TT_IT_TX_COUNT_OVERFLOW  FDCAN_TTIE_TXOE //!< Tx Count Overflow
#define FDCAN_TT_IT_SCHEDULING_ERROR_1 FDCAN_TTIE_SE1E //!< Scheduling Error 1
#define FDCAN_TT_IT_SCHEDULING_ERROR_2 FDCAN_TTIE_SE2E //!< Scheduling Error 2
#define FDCAN_TT_IT_ERROR_LEVEL_CHANGE FDCAN_TTIE_ELCE //!< Error Level Changed

#define FDCAN_TT_IT_INIT_WATCH_TRIGGER   FDCAN_TTIE_IWTE //!< Initialization Watch Trigger
#define FDCAN_TT_IT_WATCH_TRIGGER        FDCAN_TTIE_WTE  //!< Watch Trigger
#define FDCAN_TT_IT_APPLICATION_WATCHDOG FDCAN_TTIE_AWE  //!< Application Watchdog
#define FDCAN_TT_IT_CONFIG_ERROR         FDCAN_TTIE_CERE //!< Configuration Error



#if USE_FDCAN_REGISTER_CALLBACKS == 1
#define __FDCAN_RESET_HANDLE_STATE(__HANDLE__) do{                                                \
                                                      (__HANDLE__)->State = FDCAN_STATE_RESET;    \
                                                      (__HANDLE__)->MspInitCallback = NULL;           \
                                                      (__HANDLE__)->MspDeInitCallback = NULL;         \
                                                     } while(0)
#else
#define __FDCAN_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = FDCAN_STATE_RESET)
#endif // USE_FDCAN_REGISTER_CALLBACKS

#define __FDCAN_ENABLE_IT(__HANDLE__, __INTERRUPT__)             \
  do{                                                                \
    (__HANDLE__)->Instance->IE |= ((__INTERRUPT__) & FDCAN_IR_MASK); \
    FDCAN_CCU->IE |= (((__INTERRUPT__) & CCU_IR_MASK) >> 30);        \
  }while(0)

#define __FDCAN_DISABLE_IT(__HANDLE__, __INTERRUPT__)               \
  do{                                                                   \
    ((__HANDLE__)->Instance->IE) &= ~((__INTERRUPT__) & FDCAN_IR_MASK); \
    FDCAN_CCU->IE &= ~(((__INTERRUPT__) & CCU_IR_MASK) >> 30);          \
  }while(0)

#define __FDCAN_GET_IT(__HANDLE__, __INTERRUPT__) (((__INTERRUPT__) < FDCAN_IT_CALIB_WATCHDOG_EVENT) ? ((__HANDLE__)->Instance->IR & (__INTERRUPT__)) : ((FDCAN_CCU->IR << 30) & (__INTERRUPT__)))

#define __FDCAN_CLEAR_IT(__HANDLE__, __INTERRUPT__)               \
do{                                                                   \
    ((__HANDLE__)->Instance->IR) = ((__INTERRUPT__) & FDCAN_IR_MASK); \
    FDCAN_CCU->IR = (((__INTERRUPT__) & CCU_IR_MASK) >> 30);          \
  }while(0)

#define __FDCAN_GET_FLAG(__HANDLE__, __FLAG__) (((__FLAG__) < FDCAN_FLAG_CALIB_WATCHDOG_EVENT) ? ((__HANDLE__)->Instance->IR & (__FLAG__)) : ((FDCAN_CCU->IR << 30) & (__FLAG__)))

#define __FDCAN_CLEAR_FLAG(__HANDLE__, __FLAG__)             \
do{                                                              \
    ((__HANDLE__)->Instance->IR) = ((__FLAG__) & FDCAN_IR_MASK); \
    FDCAN_CCU->IR = (((__FLAG__) & CCU_IR_MASK) >> 30);          \
  }while(0)

#define __FDCAN_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) (((__INTERRUPT__) < FDCAN_IT_CALIB_WATCHDOG_EVENT) ? ((__HANDLE__)->Instance->IE & (__INTERRUPT__)) : ((FDCAN_CCU->IE << 30) & (__INTERRUPT__)))

#define __FDCAN_TT_ENABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->ttcan->TTIE) |= (__INTERRUPT__))

#define __FDCAN_TT_DISABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->ttcan->TTIE) &= ~(__INTERRUPT__))

#define __FDCAN_TT_GET_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->ttcan->TTIR) & (__INTERRUPT__))

#define __FDCAN_TT_CLEAR_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->ttcan->TTIR) = (__INTERRUPT__))

#define __FDCAN_TT_GET_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->ttcan->TTIR) & (__FLAG__))

#define __FDCAN_TT_CLEAR_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->ttcan->TTIR) = (__FLAG__))

#define __FDCAN_TT_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->ttcan->TTIE) & (__INTERRUPT__))


#define IS_FDCAN_FRAME_FORMAT(FORMAT) (((FORMAT) == FDCAN_FRAME_CLASSIC  ) || \
                                       ((FORMAT) == FDCAN_FRAME_FD_NO_BRS) || \
                                       ((FORMAT) == FDCAN_FRAME_FD_BRS   ))
#define IS_FDCAN_MODE(MODE) (((MODE) == FDCAN_MODE_NORMAL              ) || \
                             ((MODE) == FDCAN_MODE_RESTRICTED_OPERATION) || \
                             ((MODE) == FDCAN_MODE_BUS_MONITORING      ) || \
                             ((MODE) == FDCAN_MODE_INTERNAL_LOOPBACK   ) || \
                             ((MODE) == FDCAN_MODE_EXTERNAL_LOOPBACK   ))

#define IS_FDCAN_CLOCK_CALIBRATION(CALIBRATION) (((CALIBRATION) == FDCAN_CLOCK_CALIBRATION_DISABLE) || \
                                                 ((CALIBRATION) == FDCAN_CLOCK_CALIBRATION_ENABLE ))

#define IS_FDCAN_CKDIV(CKDIV) (((CKDIV) == FDCAN_CLOCK_DIV1 ) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV2 ) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV4 ) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV6 ) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV8 ) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV10) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV12) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV14) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV16) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV18) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV20) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV22) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV24) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV26) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV28) || \
                               ((CKDIV) == FDCAN_CLOCK_DIV30))
#define IS_FDCAN_NOMINAL_PRESCALER(PRESCALER) (((PRESCALER) >= 1U) && ((PRESCALER) <= 512U))
#define IS_FDCAN_NOMINAL_SJW(SJW) (((SJW) >= 1U) && ((SJW) <= 128U))
#define IS_FDCAN_NOMINAL_TSEG1(TSEG1) (((TSEG1) >= 1U) && ((TSEG1) <= 256U))
#define IS_FDCAN_NOMINAL_TSEG2(TSEG2) (((TSEG2) >= 1U) && ((TSEG2) <= 128U))
#define IS_FDCAN_DATA_PRESCALER(PRESCALER) (((PRESCALER) >= 1U) && ((PRESCALER) <= 32U))
#define IS_FDCAN_DATA_SJW(SJW) (((SJW) >= 1U) && ((SJW) <= 16U))
#define IS_FDCAN_DATA_TSEG1(TSEG1) (((TSEG1) >= 1U) && ((TSEG1) <= 32U))
#define IS_FDCAN_DATA_TSEG2(TSEG2) (((TSEG2) >= 1U) && ((TSEG2) <= 16U))
#define IS_FDCAN_MAX_VALUE(VALUE, MAX) ((VALUE) <= (MAX))
#define IS_FDCAN_MIN_VALUE(VALUE, MIN) ((VALUE) >= (MIN))
#define IS_FDCAN_DATA_SIZE(SIZE) (((SIZE) == FDCAN_DATA_BYTES_8 ) || \
                                  ((SIZE) == FDCAN_DATA_BYTES_12) || \
                                  ((SIZE) == FDCAN_DATA_BYTES_16) || \
                                  ((SIZE) == FDCAN_DATA_BYTES_20) || \
                                  ((SIZE) == FDCAN_DATA_BYTES_24) || \
                                  ((SIZE) == FDCAN_DATA_BYTES_32) || \
                                  ((SIZE) == FDCAN_DATA_BYTES_48) || \
                                  ((SIZE) == FDCAN_DATA_BYTES_64))
#define IS_FDCAN_TX_FIFO_QUEUE_MODE(MODE) (((MODE) == FDCAN_TX_FIFO_OPERATION ) || \
                                           ((MODE) == FDCAN_TX_QUEUE_OPERATION))
#define IS_FDCAN_ID_TYPE(ID_TYPE) (((ID_TYPE) == FDCAN_STANDARD_ID) || \
                                   ((ID_TYPE) == FDCAN_EXTENDED_ID))
#define IS_FDCAN_FILTER_CFG(CONFIG) (((CONFIG) == FDCAN_FILTER_DISABLE      ) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXFIFO0   ) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXFIFO1   ) || \
                                     ((CONFIG) == FDCAN_FILTER_REJECT       ) || \
                                     ((CONFIG) == FDCAN_FILTER_HP           ) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXFIFO0_HP) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXFIFO1_HP) || \
                                     ((CONFIG) == FDCAN_FILTER_TO_RXBUFFER  ))
#define IS_FDCAN_TX_LOCATION(LOCATION) (((LOCATION) == FDCAN_TX_BUFFER0 ) || ((LOCATION) == FDCAN_TX_BUFFER1 ) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER2 ) || ((LOCATION) == FDCAN_TX_BUFFER3 ) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER4 ) || ((LOCATION) == FDCAN_TX_BUFFER5 ) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER6 ) || ((LOCATION) == FDCAN_TX_BUFFER7 ) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER8 ) || ((LOCATION) == FDCAN_TX_BUFFER9 ) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER10) || ((LOCATION) == FDCAN_TX_BUFFER11) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER12) || ((LOCATION) == FDCAN_TX_BUFFER13) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER14) || ((LOCATION) == FDCAN_TX_BUFFER15) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER16) || ((LOCATION) == FDCAN_TX_BUFFER17) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER18) || ((LOCATION) == FDCAN_TX_BUFFER19) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER20) || ((LOCATION) == FDCAN_TX_BUFFER21) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER22) || ((LOCATION) == FDCAN_TX_BUFFER23) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER24) || ((LOCATION) == FDCAN_TX_BUFFER25) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER26) || ((LOCATION) == FDCAN_TX_BUFFER27) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER28) || ((LOCATION) == FDCAN_TX_BUFFER29) || \
                                        ((LOCATION) == FDCAN_TX_BUFFER30) || ((LOCATION) == FDCAN_TX_BUFFER31))
#define IS_FDCAN_RX_FIFO(FIFO) (((FIFO) == FDCAN_RX_FIFO0) || \
                                ((FIFO) == FDCAN_RX_FIFO1))
#define IS_FDCAN_RX_FIFO_MODE(MODE) (((MODE) == FDCAN_RX_FIFO_BLOCKING ) || \
                                     ((MODE) == FDCAN_RX_FIFO_OVERWRITE))
#define IS_FDCAN_STD_FILTER_TYPE(TYPE) (((TYPE) == FDCAN_FILTER_RANGE) || \
                                        ((TYPE) == FDCAN_FILTER_DUAL ) || \
                                        ((TYPE) == FDCAN_FILTER_MASK ))
#define IS_FDCAN_EXT_FILTER_TYPE(TYPE) (((TYPE) == FDCAN_FILTER_RANGE        ) || \
                                        ((TYPE) == FDCAN_FILTER_DUAL         ) || \
                                        ((TYPE) == FDCAN_FILTER_MASK         ) || \
                                        ((TYPE) == FDCAN_FILTER_RANGE_NO_EIDM))
#define IS_FDCAN_FRAME_TYPE(TYPE) (((TYPE) == FDCAN_DATA_FRAME  ) || \
                                   ((TYPE) == FDCAN_REMOTE_FRAME))
#define IS_FDCAN_DLC(DLC) (((DLC) == FDCAN_DLC_BYTES_0 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_1 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_2 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_3 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_4 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_5 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_6 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_7 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_8 ) || \
                           ((DLC) == FDCAN_DLC_BYTES_12) || \
                           ((DLC) == FDCAN_DLC_BYTES_16) || \
                           ((DLC) == FDCAN_DLC_BYTES_20) || \
                           ((DLC) == FDCAN_DLC_BYTES_24) || \
                           ((DLC) == FDCAN_DLC_BYTES_32) || \
                           ((DLC) == FDCAN_DLC_BYTES_48) || \
                           ((DLC) == FDCAN_DLC_BYTES_64))
#define IS_FDCAN_ESI(ESI) (((ESI) == FDCAN_ESI_ACTIVE ) || \
                           ((ESI) == FDCAN_ESI_PASSIVE))
#define IS_FDCAN_BRS(BRS) (((BRS) == FDCAN_BRS_OFF) || \
                           ((BRS) == FDCAN_BRS_ON ))
#define IS_FDCAN_FDF(FDF) (((FDF) == FDCAN_CLASSIC_CAN) || \
                           ((FDF) == FDCAN_FD_CAN     ))
#define IS_FDCAN_EFC(EFC) (((EFC) == FDCAN_NO_TX_EVENTS   ) || \
                           ((EFC) == FDCAN_STORE_TX_EVENTS))
#define IS_FDCAN_IT(IT) (((IT) & ~(FDCAN_IR_MASK | CCU_IR_MASK)) == 0U)
#define IS_FDCAN_TT_IT(IT) (((IT) & 0xFFF80000U) == 0U)
#define IS_FDCAN_FIFO_WATERMARK(FIFO) (((FIFO) == FDCAN_CFG_TX_EVENT_FIFO) || \
                                       ((FIFO) == FDCAN_CFG_RX_FIFO0     ) || \
                                       ((FIFO) == FDCAN_CFG_RX_FIFO1     ))
#define IS_FDCAN_NON_MATCHING(DESTINATION) (((DESTINATION) == FDCAN_ACCEPT_IN_RX_FIFO0) || \
                                            ((DESTINATION) == FDCAN_ACCEPT_IN_RX_FIFO1) || \
                                            ((DESTINATION) == FDCAN_REJECT            ))
#define IS_FDCAN_REJECT_REMOTE(DESTINATION) (((DESTINATION) == FDCAN_FILTER_REMOTE) || \
                                             ((DESTINATION) == FDCAN_REJECT_REMOTE))
#define IS_FDCAN_IT_LINE(IT_LINE) (((IT_LINE) == FDCAN_INTERRUPT_LINE0) || \
                                   ((IT_LINE) == FDCAN_INTERRUPT_LINE1))
#define IS_FDCAN_TIMESTAMP(OPERATION) (((OPERATION) == FDCAN_TIMESTAMP_INTERNAL) || \
                                       ((OPERATION) == FDCAN_TIMESTAMP_EXTERNAL))
#define IS_FDCAN_TIMESTAMP_PRESCALER(PRESCALER) (((PRESCALER) == FDCAN_TIMESTAMP_PRESC_1 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_2 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_3 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_4 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_5 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_6 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_7 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_8 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_9 ) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_10) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_11) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_12) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_13) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_14) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_15) || \
                                                 ((PRESCALER) == FDCAN_TIMESTAMP_PRESC_16))
#define IS_FDCAN_TIMEOUT(OPERATION) (((OPERATION) == FDCAN_TIMEOUT_CONTINUOUS   ) || \
                                     ((OPERATION) == FDCAN_TIMEOUT_TX_EVENT_FIFO) || \
                                     ((OPERATION) == FDCAN_TIMEOUT_RX_FIFO0     ) || \
                                     ((OPERATION) == FDCAN_TIMEOUT_RX_FIFO1     ))
#define IS_FDCAN_CALIBRATION_FIELD_LENGTH(LENGTH) (((LENGTH) == FDCAN_CALIB_FIELD_LENGTH_32) || \
                                                   ((LENGTH) == FDCAN_CALIB_FIELD_LENGTH_64))
#define IS_FDCAN_CALIBRATION_COUNTER(COUNTER) (((COUNTER) == FDCAN_CALIB_TIME_QUANTA_COUNTER ) || \
                                               ((COUNTER) == FDCAN_CALIB_CLOCK_PERIOD_COUNTER) || \
                                               ((COUNTER) == FDCAN_CALIB_WATCHDOG_COUNTER    ))
#define IS_FDCAN_TT_REFERENCE_MESSAGE_PAYLOAD(PAYLOAD) (((PAYLOAD) == FDCAN_TT_REF_MESSAGE_NO_PAYLOAD ) || \
                                                        ((PAYLOAD) == FDCAN_TT_REF_MESSAGE_ADD_PAYLOAD))
#define IS_FDCAN_TT_REPEAT_FACTOR(FACTOR) (((FACTOR) == FDCAN_TT_REPEAT_EVERY_CYCLE     ) || \
                                           ((FACTOR) == FDCAN_TT_REPEAT_EVERY_2ND_CYCLE ) || \
                                           ((FACTOR) == FDCAN_TT_REPEAT_EVERY_4TH_CYCLE ) || \
                                           ((FACTOR) == FDCAN_TT_REPEAT_EVERY_8TH_CYCLE ) || \
                                           ((FACTOR) == FDCAN_TT_REPEAT_EVERY_16TH_CYCLE) || \
                                           ((FACTOR) == FDCAN_TT_REPEAT_EVERY_32ND_CYCLE) || \
                                           ((FACTOR) == FDCAN_TT_REPEAT_EVERY_64TH_CYCLE))
#define IS_FDCAN_TT_TRIGGER_TYPE(TYPE) (((TYPE) == FDCAN_TT_TX_REF_TRIGGER        ) || \
                                        ((TYPE) == FDCAN_TT_TX_REF_TRIGGER_GAP    ) || \
                                        ((TYPE) == FDCAN_TT_TX_TRIGGER_SINGLE     ) || \
                                        ((TYPE) == FDCAN_TT_TX_TRIGGER_CONTINUOUS ) || \
                                        ((TYPE) == FDCAN_TT_TX_TRIGGER_ARBITRATION) || \
                                        ((TYPE) == FDCAN_TT_TX_TRIGGER_MERGED     ) || \
                                        ((TYPE) == FDCAN_TT_WATCH_TRIGGER         ) || \
                                        ((TYPE) == FDCAN_TT_WATCH_TRIGGER_GAP     ) || \
                                        ((TYPE) == FDCAN_TT_RX_TRIGGER            ) || \
                                        ((TYPE) == FDCAN_TT_TIME_BASE_TRIGGER     ) || \
                                        ((TYPE) == FDCAN_TT_END_OF_LIST           ))
#define IS_FDCAN_TT_TM_EVENT_INTERNAL(EVENT) (((EVENT) == FDCAN_TT_TM_NO_INTERNAL_EVENT ) || \
                                              ((EVENT) == FDCAN_TT_TM_GEN_INTERNAL_EVENT))
#define IS_FDCAN_TT_TM_EVENT_EXTERNAL(EVENT) (((EVENT) == FDCAN_TT_TM_NO_EXTERNAL_EVENT ) || \
                                              ((EVENT) == FDCAN_TT_TM_GEN_EXTERNAL_EVENT))
#define IS_FDCAN_OPERATION_MODE(MODE) (((MODE) == FDCAN_TT_COMMUNICATION_LEVEL1 ) || \
                                       ((MODE) == FDCAN_TT_COMMUNICATION_LEVEL2 ) || \
                                       ((MODE) == FDCAN_TT_COMMUNICATION_LEVEL0 ))
#define IS_FDCAN_TT_OPERATION(OPERATION) (((OPERATION) == FDCAN_STRICTLY_TT_OPERATION      ) || \
                                          ((OPERATION) == FDCAN_EXT_EVT_SYNC_TT_OPERATION))
#define IS_FDCAN_TT_TIME_MASTER(FUNCTION) (((FUNCTION) == FDCAN_TT_SLAVE           ) || \
                                           ((FUNCTION) == FDCAN_TT_POTENTIAL_MASTER))
#define IS_FDCAN_TT_EXTERNAL_CLK_SYNC(SYNC) (((SYNC) == FDCAN_TT_EXT_CLK_SYNC_DISABLE) || \
                                             ((SYNC) == FDCAN_TT_EXT_CLK_SYNC_ENABLE ))
#define IS_FDCAN_TT_GLOBAL_TIME_FILTERING(FILTERING) (((FILTERING) == FDCAN_TT_GLOB_TIME_FILT_DISABLE) || \
                                                      ((FILTERING) == FDCAN_TT_GLOB_TIME_FILT_ENABLE ))
#define IS_FDCAN_TT_AUTO_CLK_CALIBRATION(CALIBRATION) (((CALIBRATION) == FDCAN_TT_AUTO_CLK_CALIB_DISABLE) || \
                                                       ((CALIBRATION) == FDCAN_TT_AUTO_CLK_CALIB_ENABLE ))
#define IS_FDCAN_TT_EVENT_TRIGGER_POLARITY(POLARITY) (((POLARITY) == FDCAN_TT_EVT_TRIG_POL_RISING ) || \
                                                      ((POLARITY) == FDCAN_TT_EVT_TRIG_POL_FALLING))
#define IS_FDCAN_TT_BASIC_CYCLES_NUMBER(NUMBER) (((NUMBER) == FDCAN_TT_CYCLES_PER_MATRIX_1 ) || \
                                                 ((NUMBER) == FDCAN_TT_CYCLES_PER_MATRIX_2 ) || \
                                                 ((NUMBER) == FDCAN_TT_CYCLES_PER_MATRIX_4 ) || \
                                                 ((NUMBER) == FDCAN_TT_CYCLES_PER_MATRIX_8 ) || \
                                                 ((NUMBER) == FDCAN_TT_CYCLES_PER_MATRIX_16) || \
                                                 ((NUMBER) == FDCAN_TT_CYCLES_PER_MATRIX_32) || \
                                                 ((NUMBER) == FDCAN_TT_CYCLES_PER_MATRIX_64))
#define IS_FDCAN_TT_CYCLE_START_SYNC(SYNC) (((SYNC) == FDCAN_TT_NO_SYNC_PULSE         ) || \
                                            ((SYNC) == FDCAN_TT_SYNC_BASIC_CYCLE_START) || \
                                            ((SYNC) == FDCAN_TT_SYNC_MATRIX_START     ))
#define IS_FDCAN_TT_TX_ENABLE_WINDOW(NTU) (((NTU) >= 1U) && ((NTU) <= 16U))
#define IS_FDCAN_TT_TUR_NUMERATOR(NUMERATOR) (((NUMERATOR) >= 0x10000U) && ((NUMERATOR) <= 0x1FFFFU))
#define IS_FDCAN_TT_TUR_DENOMINATOR(DENOMINATOR) (((DENOMINATOR) >= 0x0001U) && ((DENOMINATOR) <= 0x3FFFU))
#define IS_FDCAN_TT_TUR_LEVEL_1(NC,DC) ((NC) >= (4U * (DC)))
#define IS_FDCAN_TT_TUR_LEVEL_0_2(NC,DC) ((NC) >= (8U * (DC)))
#define IS_FDCAN_TT_STOP_WATCH_TRIGGER(TRIGGER) (((TRIGGER) == FDCAN_TT_STOP_WATCH_TRIGGER_0) || \
                                                 ((TRIGGER) == FDCAN_TT_STOP_WATCH_TRIGGER_1) || \
                                                 ((TRIGGER) == FDCAN_TT_STOP_WATCH_TRIGGER_2) || \
                                                 ((TRIGGER) == FDCAN_TT_STOP_WATCH_TRIGGER_3))
#define IS_FDCAN_TT_EVENT_TRIGGER(TRIGGER) (((TRIGGER) == FDCAN_TT_EVENT_TRIGGER_0) || \
                                            ((TRIGGER) == FDCAN_TT_EVENT_TRIGGER_1) || \
                                            ((TRIGGER) == FDCAN_TT_EVENT_TRIGGER_2) || \
                                            ((TRIGGER) == FDCAN_TT_EVENT_TRIGGER_3))
#define IS_FDCAN_TT_TIME_PRESET(TIME) (((TIME) <= 0xFFFFU) && ((TIME) != 0x8000U))
#define IS_FDCAN_TT_STOP_WATCH_SOURCE(SOURCE) (((SOURCE) == FDCAN_TT_STOP_WATCH_DISABLED   ) || \
                                               ((SOURCE) == FDCAN_TT_STOP_WATCH_CYCLE_TIME ) || \
                                               ((SOURCE) == FDCAN_TT_STOP_WATCH_LOCAL_TIME ) || \
                                               ((SOURCE) == FDCAN_TT_STOP_WATCH_GLOBAL_TIME))
#define IS_FDCAN_TT_STOP_WATCH_POLARITY(POLARITY) (((POLARITY) == FDCAN_TT_STOP_WATCH_RISING ) || \
                                                   ((POLARITY) == FDCAN_TT_STOP_WATCH_FALLING))
#define IS_FDCAN_TT_REGISTER_TIME_MARK_SOURCE(SOURCE) (((SOURCE) == FDCAN_TT_REG_TIMEMARK_DIABLED ) || \
                                                       ((SOURCE) == FDCAN_TT_REG_TIMEMARK_CYC_TIME) || \
                                                       ((SOURCE) == FDCAN_TT_REG_TIMEMARK_LOC_TIME) || \
                                                       ((SOURCE) == FDCAN_TT_REG_TIMEMARK_GLO_TIME))


typedef union
{
	struct __attribute__((__packed__))
	{
		volatile uint32_t SFID2 :11;		//bit: 0..10   Standard Filter ID 2
		volatile uint32_t :5;			//bit: 11..15  Reserved
		volatile uint32_t SFID1 :11;		//bit: 16..26  Standard Filter ID 1
		volatile uint32_t SFEC :3;		//bit: 27..29  Standard Filter Element Configuration
		volatile uint32_t SFT :2;			//bit: 30..31  Standard Filter Type
	} bit;
	volatile uint32_t reg;
} CAN_STANDARD_MESSAGE_FILTER_ELEMENT_S0_Type;
typedef struct __attribute__((__packed__))
{
	volatile CAN_STANDARD_MESSAGE_FILTER_ELEMENT_S0_Type S0;
} Can_Standard_Filter_Element;

typedef union
{
	struct __attribute__((__packed__))
	{
		uint32_t EFID1 :29;	//bit: 0..28   Extended Filter ID 1
		uint32_t EFEC :3;	//bit: 29..31  Extended Filter Element Configuration
	} bit;
	volatile uint32_t reg;
} CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F0_Type;
typedef union
{
	struct __attribute__((__packed__))
	{
		uint32_t EFID2 :29;	//bit: 0..28  Extended Filter ID 2
		uint32_t :1;		//bit: 29     Reserved
		uint32_t EFT :2;		//bit: 30..31 Extended Filter Type
	} bit;
	volatile uint32_t reg;
} CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F1_Type;
typedef struct __attribute__((__packed__))
{
	volatile CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F0_Type F0;
	volatile CAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F1_Type F1;
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

typedef union __attribute__((__packed__))
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

typedef union
{
	struct
	{
		uint32_t ID :29;		//bit:  0..28  Identifier
		uint32_t RTR :1;		//bit:  29     Remote Transmission Request
		uint32_t XTD :1;		//bit:  30     Extended Identifier
		uint32_t :1;		//bit:  31     Reserved
	} bit;
	uint32_t reg;
} CAN_TX_ELEMENT_T0_Type;
typedef union
{
	struct
	{
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



typedef union
{
	uint8_t uint8[CAN_ELMTS_ARRAY_SIZE];
	uint32_t uint32[CAN_ELMTS_ARRAY_SIZE / 4];
} CAN_TX_ELEMENT_DATA_Type;

typedef volatile struct
{
	CAN_TX_ELEMENT_T0_Type T0;
	CAN_TX_ELEMENT_T1_Type T1;
	CAN_TX_ELEMENT_DATA_Type data;
} Can_Tx_Element;

typedef struct
{
	__attribute__((__aligned__(4))) Can_Standard_Filter_Element std_filters[CAN_STD_FILTER_NUMBER];
	__attribute__((__aligned__(4))) Can_Extended_Filter_Element ext_filters[CAN_EXT_FILTER_NUMBER];
	__attribute__((__aligned__(4))) Can_Rx_Element rx_buffer[CAN_RX_BUFFER_NUMBER];
	__attribute__((__aligned__(4))) Can_Rx_Element rx_fifo0[CAN_RX_FIFO0_ELMTS_NUMBER];
	__attribute__((__aligned__(4))) Can_Rx_Element rx_fifo1[CAN_RX_FIFO1_ELMTS_NUMBER];
	__attribute__((__aligned__(4))) Can_Tx_Element tx_buffer[CAN_TX_BUFFER_NUMBER];
	__attribute__((__aligned__(4))) Can_Tx_Element tx_fifo[CAN_TX_FIFO_QUEUE_ELMTS_NUMBER];

} Can_Message_RAM;

#endif
