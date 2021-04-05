#include "ADS131.h"
#include "main.h"
#include "systick.h"
#include "measurement.h"

static Measurement measurements[8] = { 0 };
static uint32_t status = 0;
void ADS131_Init(void)
{

	LL_GPIO_ResetOutputPin(ADS_nRESET_GPIO_Port, ADS_nRESET_Pin);
	Systick_BusyWait(100);
	LL_GPIO_SetOutputPin(ADS_nRESET_GPIO_Port, ADS_nRESET_Pin);

	LL_TIM_EnableCounter(TIM4);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH3);
	for (uint8_t ch = 0; ch < 8; ch++)
		Measurement_Init(&measurements[ch]);

	//uint32_t data[22] = {0};
	//ADS131_readRegisters(ADS131_GAIN1, 10, data);
	//ADS131_readRegisters(ADS131_CH1_CFG, 15, data);

	//ADS131_WriteRegister(ADS131_GAIN1, 0x0000);
	//ADS131_WriteRegister(ADS131_GAIN2, 0x0000);

	ADS131_WriteRegister(ADS131_GAIN1, 0x1111);
	ADS131_WriteRegister(ADS131_GAIN2, 0x1111);

//	ADS131_WriteRegister(ADS131_CLOCK, 0xFF0E);
	ADS131_WriteRegister(ADS131_CLOCK, 0xFF16);


}

void ADS131_WriteRegister(uint32_t reg, uint32_t value)
{
	/*int32_t txData[2] = { (ADS131_WREG_OPCODE(0, reg)) << 8, value << 8 };
	int32_t rxData[2] = { 0 };

	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) txData, (uint8_t*) rxData, 2, 1000000);
	while (!HAL_GPIO_ReadPin(ADS_nDRDY_GPIO_Port, ADS_nDRDY_Pin))
	{
	};*/
}

void ADS131_ReadRegisters(uint32_t reg, uint32_t n, uint32_t data[])
{
/*	int32_t txData[256] = { (ADS131_RREG_OPCODE((n - 1), reg)) << 8, 0, 0 };

	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &txData[0], (uint8_t*) data, 1, 1000000);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &txData[1], (uint8_t*) data, n, 1000000);
	while (!HAL_GPIO_ReadPin(ADS_nDRDY_GPIO_Port, ADS_nDRDY_Pin))
	{
	};*/
}
void ADS131_UpdateData(void)
{
	/*static uint8_t sampling = 0;

	if (HAL_GPIO_ReadPin(ADS_nDRDY_GPIO_Port, ADS_nDRDY_Pin))
	{
		sampling = 1;
	//	HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, GPIO_PIN_RESET);
	}
	else if (sampling)
	{
		sampling = 0;
	//	HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, GPIO_PIN_SET);
		int32_t txData[9] = { 0 };
		int32_t rxData[9] = { 0 };

		//HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) txData, (uint8_t*) rxData, 9, 1000000);


		status = rxData[0];
		for (uint8_t ch = 0; ch < 8; ch++)
			Measurement_Add(&measurements[ch], (rxData[ch + 1] | ((rxData[ch + 1] & (1 << 23)) ? 0xFF000000 : 0)));

	}*/

}

int32_t ADS131_GetData(uint8_t ch)
{
	return Measurement_GetLatest(&measurements[ch], 0);
	//return GetAverageMeasurement(&measurements[ch]);
}
uint32_t ADS131_GetStatus(void)
{
	return status;
}

