#ifndef STRHAL_GPIO_H_
#define STRHAL_GPIO_H_

#include <stm32g4xx.h>
#include <stm32g4xx_ll_gpio.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	STRHAL_GPIO_TYPE_OPP, //Output push pull
	STRHAL_GPIO_TYPE_OOD, //Output open drain
	STRHAL_GPIO_TYPE_IHZ, //input high Z
	STRHAL_GPIO_TYPE_IPU, //input pull up
	STRHAL_GPIO_TYPE_IPD // input pull down
} STRHAL_GPIO_Type_t;

typedef struct {
	GPIO_TypeDef * port;
	uint8_t pin;
	STRHAL_GPIO_Type_t type;
} STRHAL_GPIO_t;

typedef struct {
	STRHAL_GPIO_t *gpios;
	uint32_t n;
} STRHAL_GPIO_Group_t;

typedef enum {
	STRHAL_GPIO_VALUE_L = 0,
	STRHAL_GPIO_VALUE_H = 1
} STRHAL_GPIO_Value_t;

void STRHAL_GPIO_Init();

void STRHAL_GPIO_SingleInit(STRHAL_GPIO_t *gpio, STRHAL_GPIO_Type_t type);
void STRHAL_GPIO_GroupInit(STRHAL_GPIO_Group_t *gpios, STRHAL_GPIO_Type_t type);

void STRHAL_GPIO_Write(const STRHAL_GPIO_t *gpio, STRHAL_GPIO_Value_t value);
void STRHAL_GPIO_GroupWrite(const STRHAL_GPIO_Group_t *gpios, STRHAL_GPIO_Value_t value);


STRHAL_GPIO_Value_t STRHAL_GPIO_Read(const STRHAL_GPIO_t *gpio);
STRHAL_GPIO_Value_t STRHAL_GPIO_ReadOutput(const STRHAL_GPIO_t *gpio);
void STRHAL_GPIO_GroupRead(const STRHAL_GPIO_Group_t *gpios, STRHAL_GPIO_Value_t *values);


#ifdef __cplusplus
}
#endif


#endif /* STRHAL_GPIO_H_ */
