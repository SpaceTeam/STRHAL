#ifndef LID_GPIO_H_
#define LID_GPIO_H_

#include <stm32g4xx.h>
#include <stm32g4xx_ll_gpio.h>

typedef enum {
	LID_GPIO_TYPE_OPP, //Output push pull
	LID_GPIO_TYPE_OOD, //Output open drain
	LID_GPIO_TYPE_IHZ, //input high Z
	LID_GPIO_TYPE_IPU, //input pull up
	LID_GPIO_TYPE_IPD // input pull down
} LID_GPIO_Type_t;

typedef struct {
	GPIO_TypeDef * port;
	uint8_t pin;
	LID_GPIO_Type_t type;
} LID_GPIO_t;

typedef struct {
	LID_GPIO_t *gpios;
	uint32_t n;
} LID_GPIO_Group_t;

typedef enum {
	LID_GPIO_VALUE_L = 0,
	LID_GPIO_VALUE_H = 1
} LID_GPIO_Value_t;

void LID_GPIO_Init();

void LID_GPIO_SingleInit(LID_GPIO_t *gpio, LID_GPIO_Type_t type);
void LID_GPIO_GroupInit(LID_GPIO_Group_t *gpios, LID_GPIO_Type_t type);

void LID_GPIO_Write(const LID_GPIO_t *gpio, LID_GPIO_Value_t value);
void LID_GPIO_GroupWrite(const LID_GPIO_Group_t *gpios, LID_GPIO_Value_t value);


LID_GPIO_Value_t LID_GPIO_Read(const LID_GPIO_t *gpio);
void LID_GPIO_GroupRead(const LID_GPIO_Group_t *gpios, LID_GPIO_Value_t *values);


#endif /* LID_GPIO_H_ */
