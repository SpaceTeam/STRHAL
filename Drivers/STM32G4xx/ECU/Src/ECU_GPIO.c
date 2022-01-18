#include "ECU_GPIO.h"


#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_bus.h>

ECU_GPIO_t ECU_GPIO_PYRO_0_CTRL = {.port = GPIOA, .pin=LL_GPIO_PIN_9};
ECU_GPIO_t ECU_GPIO_PYRO_1_CTRL = {.port = GPIOC, .pin=LL_GPIO_PIN_9};
ECU_GPIO_t ECU_GPIO_PYRO_2_CTRL = {.port = GPIOC, .pin=LL_GPIO_PIN_7};

void ECU_GPIO_Init() {
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);


  LL_GPIO_ResetOutputPin(ECU_GPIO_PYRO_0_CTRL.port, ECU_GPIO_PYRO_0_CTRL.pin);

  GPIO_InitStruct.Pin = ECU_GPIO_PYRO_0_CTRL.pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ECU_GPIO_PYRO_0_CTRL.port, &GPIO_InitStruct);


  LL_GPIO_ResetOutputPin(ECU_GPIO_PYRO_1_CTRL.port, ECU_GPIO_PYRO_1_CTRL.pin);

  GPIO_InitStruct.Pin = ECU_GPIO_PYRO_1_CTRL.pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ECU_GPIO_PYRO_1_CTRL.port, &GPIO_InitStruct);


  LL_GPIO_ResetOutputPin(ECU_GPIO_PYRO_2_CTRL.port, ECU_GPIO_PYRO_2_CTRL.pin);

  GPIO_InitStruct.Pin = ECU_GPIO_PYRO_2_CTRL.pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ECU_GPIO_PYRO_2_CTRL.port, &GPIO_InitStruct);
}

void ECU_GPIO_Write(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint8_t enable) {
	uint32_t x = LL_GPIO_ReadOutputPort(GPIOx);

	x = enable ? x & ~PinMask : x | PinMask;

	LL_GPIO_WriteOutputPort(GPIOx, x);
}

uint32_t ECU_GPIO_Read(GPIO_TypeDef *GPIOx, uint32_t mask) {
	return LL_GPIO_ReadInputPort(GPIOx) & mask;
}
