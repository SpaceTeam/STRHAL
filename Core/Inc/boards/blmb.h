#ifndef BLMB_H_
#define BLMB_H_


#define MAX_BLMB_CHANNELS 1


#define BLMB_MAIN_CAN_BUS 0



//GPIO3
#define EN_GPIO_Port		GPIOC
#define EN_Pin				LL_GPIO_PIN_13
//GPIO8
#define STATUS_GPIO_Port	GPIOA
#define STATUS_Pin			LL_GPIO_PIN_10
//GPIO9
#define FAULT_GPIO_Port		GPIOE
#define FAULT_Pin			LL_GPIO_PIN_10
//GPIO2
#define CSN_CTR_GPIO_Port	GPIOC
#define CSN_CTR_Pin			LL_GPIO_PIN_12
//GPIO10
#define CSN_DRV_GPIO_Port	GPIOE
#define CSN_DRV_Pin			LL_GPIO_PIN_15
//GPIO4
#define CSN_ENC0_GPIO_Port	GPIOE
#define CSN_ENC0_Pin		LL_GPIO_PIN_0
//GPIO5
#define CSN_ENC1_GPIO_Port	GPIOE
#define CSN_ENC1_Pin		LL_GPIO_PIN_1
//GPIO6
#define CSN_ENC2_GPIO_Port	GPIOE
#define CSN_ENC2_Pin		LL_GPIO_PIN_3
//GPIO7
#define CSN_ENC3_GPIO_Port	GPIOE
#define CSN_ENC3_Pin		LL_GPIO_PIN_7


void BLMB_main(void);

#endif
