#ifndef ECU_GPIO_H_
#define GPIO_H_

#include <stm32g4xx.h>
#include <stm32g4xx_ll_gpio.h>

typedef struct {
	GPIO_TypeDef * port;
	uint32_t pin;
} ECU_GPIO_t;

extern ECU_GPIO_t ECU_GPIO_PYRO_0_CTRL;
extern ECU_GPIO_t ECU_GPIO_PYRO_1_CTRL;
extern ECU_GPIO_t ECU_GPIO_PYRO_2_CTRL;

void ECU_GPIO_Init();

void ECU_GPIO_Write(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint8_t enable);
uint32_t ECU_GPIO_Read(GPIO_TypeDef *GPIOx, uint32_t PinMask);


#endif /* ECU_GPIO_H_ */
