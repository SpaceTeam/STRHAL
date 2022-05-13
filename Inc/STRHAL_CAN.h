#ifndef STRHAL_CAN_H
#define STRHAL_CAN_H

#include <stm32g4xx.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_utils.h>
#include <STRHAL.h>
#include <STRHAL_CAN_Def.h>
#include <STRHAL_Oof.h>

#ifdef __cplusplus
extern "C" {
#endif


/***** DEFINES *****/
#define STRHAL_CAN_START_TOT 				16000000
#define STRHAL_CAN_STD_FILTER_NUMBER		0
#define STRHAL_CAN_EXT_FILTER_NUMBER		0
#define STRHAL_CAN_RAM_N_FILTER			28

typedef enum {
	STRHAL_FDCAN1,
	STRHAL_FDCAN2,

	STRHAL_N_FDCAN
} STRHAL_FDCAN_Id_t;

typedef enum {
	STRHAL_FDCAN_RX0,
	STRHAL_FDCAN_RX1,

	STRHAL_FDCAN_N_RX
} STRHAL_FDCAN_Rx_Id_t;


typedef struct {
	uint32_t value_id1;
	uint32_t mask_id2;
	uint32_t type;
} STRHAL_FDCAN_Filter_t;


typedef void (*STRHAL_CAN_Receptor_t) (uint32_t id, uint8_t *data, uint32_t n);

STRHAL_Oof_t STRHAL_CAN_Init();
int STRHAL_CAN_Instance_Init(STRHAL_FDCAN_Id_t fdcan_id);
int STRHAL_CAN_Subscribe(STRHAL_FDCAN_Id_t fdcan_id, STRHAL_FDCAN_Rx_Id_t rx_id, STRHAL_FDCAN_Filter_t *filter, uint8_t n, STRHAL_CAN_Receptor_t receptor);
void STRHAL_CAN_Run();
int32_t STRHAL_CAN_Receive(STRHAL_FDCAN_Id_t fdcan_id, uint32_t *id, uint8_t *data, uint32_t n);
int32_t STRHAL_CAN_Send(STRHAL_FDCAN_Id_t fdcan_id, uint32_t id, const uint8_t *data, uint32_t n);


#ifdef __cplusplus
}
#endif

#endif
