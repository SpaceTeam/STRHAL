#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_ll_adc.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_crs.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_cortex.h"
#include "stm32h7xx_ll_utils.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_spi.h"
#include <can_def.h>
#include "git_version.h"

#include "board_config.h"
#define UINT14_MAX ((1<<14) - 1)

void Error_Handler(void);

#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

typedef enum
{
	NOICE = 0,
	OOF_TOO_LITTLE,
	OOF_TOO_MUCH,
	OOF,
	OOF_NOT_EQUAL,
	OOF_NO_NEW_DATA,
	OOF_CAN_INIT,
	OOF_CAN_TX_FULL,
	OOF_NO_OP,
	OOF_UNKNOWN_CMD,
	OOF_WRONG_CHANNEL_TYPE,
	OOF_NOT_IMPLEMENTED,
	OOF_SPI_TXP_FULL,
	OOF_SPI_RXP_FULL,
	OOF_SPI_NO_EOT,
	OOF_ADS_NO_RDY,
	OOF_ADS_INIT
}Result_t;

#ifdef __cplusplus
}
#endif

#endif
