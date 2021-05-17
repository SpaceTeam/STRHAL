#ifndef HARDWARE_BOARD_CONFIG_H_
#define HARDWARE_BOARD_CONFIG_H_

#define LCB 1
#define IOB 2

#define BOARD IOB

#if BOARD == LCB
#include "lcb.h"
#define MAIN_CAN_BUS LCB_MAIN_CAN_BUS
#elif BOARD == IOB
#include "iob.h"
#define MAIN_CAN_BUS IOB_MAIN_CAN_BUS
#endif
#define DEBUG_CAN_BUS 1



#define SPI_TIMEOUT_VALUE 1000
#define ADS_TIMEOUT_VALUE SPI_TIMEOUT_VALUE

#define ADS_nDRDY_Pin LL_GPIO_PIN_13
#define ADS_nDRDY_GPIO_Port GPIOC
#define ADS_nRESET_Pin LL_GPIO_PIN_12
#define ADS_nRESET_GPIO_Port GPIOC


#define PIN_SPEAKER_N_Pin LL_GPIO_PIN_8
#define PIN_SPEAKER_N_GPIO_Port GPIOE
#define PIN_SPEAKER_Pin LL_GPIO_PIN_9
#define PIN_SPEAKER_GPIO_Port GPIOE
#define BUTTON_Pin LL_GPIO_PIN_11
#define BUTTON_GPIO_Port GPIOB
#define LED_STATUS_1_Pin LL_GPIO_PIN_8
#define LED_STATUS_1_GPIO_Port GPIOD
#define LED_STATUS_2_Pin LL_GPIO_PIN_9
#define LED_STATUS_2_GPIO_Port GPIOD
#define LED_DEBUG_Pin LL_GPIO_PIN_10
#define LED_DEBUG_GPIO_Port GPIOD

#endif
