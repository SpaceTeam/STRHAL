#include <stm32g4xx_ll_bus.h>
#include <stddef.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include "stm32g4xx_ll_i2c.h"

#include <STRHAL_SysTick.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint32_t pin;
    uint32_t afn;
    STRHAL_I2C_Id_t i2c;
} STRHAL_I2C_IO_t;

const STRHAL_I2C_IO_t _sda = //PB7 AF4
{
    .port = GPIOB, .pin = LL_GPIO_PIN_7, .afn = LL_GPIO_AF_4, .i2c = STRHAL_I2C_I2C1
}

const STRHAL_I2C_IO_t _scl = 
{
    .port = GPIOB, .pin = LL_GPIO_PIN_8, .afn = LL_GPIO_AF_4, .i2c = STRHAL_I2C_I2C1
}

void STRHAL_I2C_Init()
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

    LL_I2C_DeInit(I2C1);

    LL_I2C_Disable(I2C1);
}

int32_t STRHAL_I2C_Master_Init(STRHAL_I2C_Id_t i2c_id, const STRHAL_I2C_Config_t *config)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = 
    { 0 };

    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;

    //SDA Config
    GPIO_InitStruct.pin = _sda.pin;
    GPIO_InitStruct.Alternate = _sda.afn;
    LL_GPIO_Init(_sda.port, &GPIO_InitStruct);

    //SCL Config
    GPIO_InitStruct.Pin = _scl.pin;
    GPIO_InitStruct.Alternate = _scl.afn;
    LL_GPIO_Init(_scl.port, &GPIO_InitStruct);

    LL_I2C_InitTypeDef I2C_InitStruct =
    { 0 };

    /* I2C1 interrupt Init */
    NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(I2C1_EV_IRQn);
    NVIC_SetPriority(I2C1_ER_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(I2C1_ER_IRQn);

      /** I2C Initialization
    */
    I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
    I2C_InitStruct.Timing = 0x00200205;
    I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_DISABLE;
    I2C_InitStruct.DigitalFilter = 0;
    I2C_InitStruct.OwnAddress1 = 0;
    I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
    I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
    LL_I2C_Init(I2C1, &I2C_InitStruct);
    LL_I2C_EnableAutoEndMode(I2C1);
    LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
    LL_I2C_DisableOwnAddress2(I2C1);
    LL_I2C_DisableGeneralCall(I2C1);
    LL_I2C_EnableClockStretching(I2C1);

    /** I2C Fast mode Plus enable
    */
    LL_SYSCFG_EnableFastModePlus(LL_SYSCFG_I2C_FASTMODEPLUS_I2C1);

}

uint32_t I2C1_EV_IRQn()
{

}

uint32_t I2C1_ER_IRQn()
{
    
}