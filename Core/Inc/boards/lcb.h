#ifndef LCB_H_
#define LCB_H_

#define MAX_LCB_CHANNELS 8

//#define MAX_CHANNELS MAX_LCB_CHANNELS

#define LCB_MAIN_CAN_BUS 0



#define ADS_TIMEOUT_VALUE SPI_TIMEOUT_VALUE

#define ADS_nDRDY_GPIO_Port GPIOC
#define ADS_nDRDY_Pin LL_GPIO_PIN_13
#define ADS_nRESET_GPIO_Port GPIOC
#define ADS_nRESET_Pin LL_GPIO_PIN_12

void LCB_main(void);

#endif
