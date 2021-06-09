#include "ads131.h"
#include "main.h"
#include "systick.h"
#include "measurement.h"
#include "spi.h"
#include "tim.h"

#include "serial.h"
#include <stdio.h>

static Measurement measurements[8] =
{ 0 };
static uint8_t new_data[8] =
{ 0 };
static uint32_t status = 0;
Result_t Ads131_Init(void)
{
	SPI1_Init(LL_SPI_DATAWIDTH_24BIT, LL_SPI_PHASE_2EDGE, LL_SPI_POLARITY_LOW, LL_SPI_NSS_HARD_OUTPUT, LL_SPI_BAUDRATEPRESCALER_DIV32);

	TIM4_Init();

	LL_GPIO_ResetOutputPin(ADS_nRESET_GPIO_Port, ADS_nRESET_Pin);
	Systick_BusyWait(100);
	LL_GPIO_SetOutputPin(ADS_nRESET_GPIO_Port, ADS_nRESET_Pin);

	LL_TIM_EnableCounter(TIM4);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH3);
	for (uint8_t ch = 0; ch < 8; ch++)
		Measurement_Init(&measurements[ch]);

	if (Ads131_WriteRegister(ADS131_GAIN1, 0x1111) != NOICE)
		return OOF_ADS_INIT;
	if (Ads131_WriteRegister(ADS131_GAIN2, 0x1111) != NOICE)
		return OOF_ADS_INIT;
	if (Ads131_WriteRegister(ADS131_CLOCK, 0xFF0E) != NOICE)
		return OOF_ADS_INIT;

	int32_t data[4] =
	{ 0 };
	Result_t result = Ads131_ReadRegisters(ADS131_CLOCK, 3, data);
/*	char send_buffer[256] =
	{ 0 };
	Serial_PrintString("REGISTER");
	for (uint32_t i = 0; i < 4; ++i)
	{
		sprintf(send_buffer, "result: %i, data[%ld]: %ld\n\r", result, i, data[i]);
		Serial_PrintString(send_buffer);
	}*/

	return result;
}

Result_t Ads131_WriteRegister(uint32_t reg, uint32_t value)
{
	int32_t txData[2] =
	{ (ADS131_WREG_OPCODE(0, reg)) << 8, value << 8 };
	int32_t rxData[2] =
	{ 0 };
	uint32_t tickstart = 0;

	Result_t result = SPI_Transmit_Receive(SPI1, txData, rxData, 2);

	// uncomment to receive and print WREG response from ADS
	/*int32_t txData2[1] = { 0 };
	 int32_t rxData2[1] = { 0 };
	 char send_buffer[256] = { 0 };
	 SPI_Transmit_Receive(SPI1, txData2, rxData2, 1);
	 sprintf(send_buffer, "Response: %ld\n\r", rxData2[0]);
	 Serial_Print(send_buffer);*/

	tickstart = Systick_GetTick();
	while ((LL_GPIO_ReadInputPort(ADS_nDRDY_GPIO_Port) & ADS_nDRDY_Pin) == 0UL)
	{
		if ((Systick_GetTick() - tickstart) > ADS_TIMEOUT_VALUE)
			return OOF_ADS_NO_RDY;
	}

	return result;
}

// Requires data.length to be n+1, because ADS sends ack then n register values
Result_t Ads131_ReadRegisters(uint32_t reg, uint32_t n, int32_t data[])
{
	int32_t txData1[1] =
	{ (ADS131_RREG_OPCODE((n - 1), reg)) << 8 };
	int32_t txData2[256] =
	{ 0 };
	int32_t rxData[1] =
	{ 0 };
	uint32_t tickstart = 0;

	Result_t result = SPI_Transmit_Receive(SPI1, txData1, rxData, 1);
	Result_t result2 = SPI_Transmit_Receive(SPI1, txData2, data, n + 1);

	tickstart = Systick_GetTick();
	while ((LL_GPIO_ReadInputPort(ADS_nDRDY_GPIO_Port) & ADS_nDRDY_Pin) == 0UL)
	{
		if ((Systick_GetTick() - tickstart) > ADS_TIMEOUT_VALUE)
			return OOF_ADS_NO_RDY;
	}

	if ((result | result2) != 0)
	{
		return (result == 0) ? result2 : result;
	}
	else
	{
		return NOICE;
	}
}

Result_t Ads131_UpdateData(void)
{
	static uint8_t sampling = 0;
	if ((LL_GPIO_ReadInputPort(ADS_nDRDY_GPIO_Port) & ADS_nDRDY_Pin) != 0UL)
	{
		sampling = 1;
	}
	else if (sampling)
	{
		sampling = 0;
		int32_t txData[9] =
		{ 0 };
		int32_t rxData[9] =
		{ 0 };

		Result_t result = SPI_Transmit_Receive(SPI1, txData, rxData, 9);

		status = rxData[0];
		for (uint8_t ch = 0; ch < 8; ch++)
		{
			Measurement_Add(&measurements[ch], (rxData[ch + 1] | ((rxData[ch + 1] & (1 << 23)) ? 0xFF000000 : 0)));
			new_data[ch] = 1;
		}
		return result;
	}

	return NOICE;
}

int32_t Ads131_GetData(uint8_t ch)
{
	//return Measurement_GetLatest(&measurements[ch], 0);
	if (new_data[ch] == 1)
	{
		new_data[ch] = 0;
		return Measurement_GetAverage(&measurements[ch]);
	}
	return ADS131_NO_NEW_DATA;
}
uint32_t Ads131_GetStatus(void)
{
	return status;
}

