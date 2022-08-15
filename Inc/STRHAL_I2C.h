#ifndef STRHAL_I2C_H
#define STRHAL_I2C_H

#include <stm32g4xx.h>
#include <stm32g4xx_ll_i2c.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_dma.h>
#include <stm32g4xx_ll_dmamux.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

/***** DEFINES *****/
//DMA
#define STRHAL_I2C3_DMA_CHANNEL LL_DMA_CHANNEL_8
#define STRHAL_I2C3_DMA_CHANNEL_IRQ DMA1_Channel8_IRQn

#define STRHAL_I2C_DMA_PRIORITY LL_DMA_PRIORITY_LOW
#define STRHAL_I2C_DMA DMA1
#define STRHAL_I2C_BUF_SIZE 256

typedef enum
{
	STRHAL_I2C3,

	STRHAL_N_I2C
} STRHAL_I2C_Id_t;

typedef struct
{
		char data[STRHAL_I2C_BUF_SIZE];
		uint32_t h;
		uint32_t n;
		uint32_t n_dma;
} STRHAL_I2C_Rx_Buf_t;

void STRHAL_I2C_Init();
int STRHAL_I2C_Instance_Init(STRHAL_I2C_Id_t i2c_id);

int32_t STRHAL_I2C_Transmit(STRHAL_I2C_Id_t i2c_id, uint8_t address, uint8_t *data, uint8_t n);
int32_t STRHAL_I2C_Receive(STRHAL_I2C_Id_t i2c_id, uint8_t address, uint8_t *data, uint8_t n);

#ifdef __cplusplus
}
#endif

#endif
