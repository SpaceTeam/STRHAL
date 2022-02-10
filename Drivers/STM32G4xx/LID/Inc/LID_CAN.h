#ifndef LID_CAN_H
#define LID_CAN_H

#include "LID_CAN_Def.h"
#include "LID.h"
#include "LID_Oof.h"
#include <stm32g4xx.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_utils.h>

#ifdef __cplusplus
extern "C" {
#endif


/***** DEFINES *****/
#define LID_CAN_START_TOT 				16000000
#define LID_CAN_STD_FILTER_NUMBER		0
#define LID_CAN_EXT_FILTER_NUMBER		0
#define LID_CAN_RAM_N_FILTER			28

typedef enum {
	LID_FDCAN1,
	LID_FDCAN2,

	LID_N_FDCAN
} LID_FDCAN_Id_t;

typedef enum {
	LID_FDCAN_RX0,
	LID_FDCAN_RX1,

	LID_FDCAN_N_RX
} LID_FDCAN_Rx_Id_t;


typedef struct {
	uint32_t value;
	uint32_t mask;
} LID_FDCAN_Filter_t;


typedef void (*LID_CAN_Receptor_t) (uint32_t id, uint8_t *data, uint32_t n);

LID_Oof_t LID_CAN_Init();
int LID_CAN_Instance_Init(LID_FDCAN_Id_t fdcan_id);
int LID_CAN_Subscribe(LID_FDCAN_Id_t fdcan_id, LID_FDCAN_Rx_Id_t rx_id, LID_FDCAN_Filter_t *filter, uint8_t n, LID_CAN_Receptor_t receptor);
void LID_CAN_Run();
int32_t LID_CAN_Receive(LID_FDCAN_Id_t fdcan_id, uint32_t *id, uint8_t *data, uint32_t n);
int32_t LID_CAN_Send(LID_FDCAN_Id_t fdcan_id, uint32_t id, const uint8_t *data, uint32_t n);


#ifdef __cplusplus
}
#endif

#endif
