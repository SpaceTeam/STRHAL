#include "ADS131.h"
#include "main.h"
#include "systick.h"
#include "measurement.h"

#include "serial.h"
#include <stdio.h>

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

	ADS131_WriteRegister(ADS131_GAIN1, 0x1111);
	ADS131_WriteRegister(ADS131_GAIN2, 0x1111);

	ADS131_WriteRegister(ADS131_CLOCK, 0xFF0E);
	//ADS131_writeRegister(ADS131_CLOCK, 0xFF16);
	uint32_t data[3] = { 0 };
	//ADS131_readRegisters(ADS131_CLOCK, 3, data);

	char send_buffer[256] = { 0 };
	Serial_Println("REGISTER");
	for (uint32_t i = 0; i < 3; ++i)
	{
		sprintf(send_buffer, "%ld\n\r", data[i]);
		Serial_Print(send_buffer);
	}
}

void ADS131_WriteRegister(uint32_t reg, uint32_t value)
{
	int32_t txData[2] = { (ADS131_WREG_OPCODE(0, reg)) << 8, value << 8 };
	int32_t rxData[2] = { 0 };

    LL_SPI_SetTransferSize(SPI1, 2);
    LL_SPI_Enable(SPI1);
    LL_SPI_StartMasterTransfer(SPI1);

    for (uint32_t i = 0; i < 2; ++i)
    {
        while (LL_SPI_IsActiveFlag_TXP(SPI1) == 0);
        LL_SPI_TransmitData32(SPI1, txData[i]);
    }

    for (uint32_t i = 0; i < 2; ++i)
    {
    	while (LL_SPI_IsActiveFlag_RXP(SPI1) == 0);
        rxData[i] = LL_SPI_ReceiveData32(SPI1);
    }

    while (LL_SPI_IsActiveFlag_EOT(SPI1) == 0);
    LL_SPI_ClearFlag_EOT(SPI1);
    LL_SPI_ClearFlag_TXTF(SPI1);
    LL_SPI_SuspendMasterTransfer(SPI1);
    LL_SPI_Disable(SPI1);

	while ((LL_GPIO_ReadInputPort(ADS_nDRDY_GPIO_Port) & ADS_nDRDY_Pin) == 0UL)
	{
	};
}

void ADS131_ReadRegisters(uint32_t reg, uint32_t n, uint32_t data[])
{
	int32_t txData[256] = { (ADS131_RREG_OPCODE((n - 1), reg)) << 8, 0, 0 };

	LL_SPI_SetTransferSize(SPI1, 1);
	LL_SPI_Enable(SPI1);
	LL_SPI_StartMasterTransfer(SPI1);

	for (uint32_t i = 0; i < n; ++i)
	{
		while (LL_SPI_IsActiveFlag_TXP(SPI1) == 0);
		LL_SPI_TransmitData32(SPI1, txData[i]);
		Serial_Println("DONE WRITING");
		while (LL_SPI_IsActiveFlag_RXP(SPI1) == 0);
		data[i] = LL_SPI_ReceiveData32(SPI1);
		Serial_Println("DONE READING");
	}
	LL_SPI_ClearFlag_EOT(SPI1);
	LL_SPI_ClearFlag_TXTF(SPI1);

	while (LL_SPI_IsActiveFlag_EOT(SPI1) == 0);

	LL_SPI_SuspendMasterTransfer(SPI1);
	LL_SPI_Disable(SPI1);

	while ((LL_GPIO_ReadInputPort(ADS_nDRDY_GPIO_Port) & ADS_nDRDY_Pin) == 0UL)
	{
	};
}
void ADS131_UpdateData(void)
{
	static uint8_t sampling = 0;
	if ((LL_GPIO_ReadInputPort(ADS_nDRDY_GPIO_Port) & ADS_nDRDY_Pin) != 0UL)
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

	    LL_SPI_SetTransferSize(SPI1, 9);
	    LL_SPI_Enable(SPI1);
	    LL_SPI_StartMasterTransfer(SPI1);

	    for (uint32_t i = 0; i < 9; ++i)
	    {
	        while (LL_SPI_IsActiveFlag_TXP(SPI1) == 0);
	        LL_SPI_TransmitData32(SPI1, txData[i]);
	    }

	    for (uint32_t i = 0; i < 9; ++i)
	    {
			while (LL_SPI_IsActiveFlag_RXP(SPI1) == 0);
	        rxData[i] = LL_SPI_ReceiveData32(SPI1);
	    }

	    while (LL_SPI_IsActiveFlag_EOT(SPI1) == 0);
	    LL_SPI_ClearFlag_EOT(SPI1);
	    LL_SPI_ClearFlag_TXTF(SPI1);
	    LL_SPI_SuspendMasterTransfer(SPI1);
	    LL_SPI_Disable(SPI1);

		status = rxData[0];
		for (uint8_t ch = 0; ch < 8; ch++)
			Measurement_Add(&measurements[ch], (rxData[ch + 1] | ((rxData[ch + 1] & (1 << 23)) ? 0xFF000000 : 0)));

	}
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

