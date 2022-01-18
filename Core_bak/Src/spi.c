#include "spi.h"
#include "systick.h"

static void SPI1_InitSpi(uint32_t datawidth, uint32_t cpha, uint32_t cpol, uint32_t nss, uint32_t baudrate)
{
	LL_SPI_InitTypeDef SPI_InitStruct =
	{ 0 };
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = datawidth;
	SPI_InitStruct.ClockPolarity = cpol;
	SPI_InitStruct.ClockPhase = cpha;
	SPI_InitStruct.NSS = nss;
	SPI_InitStruct.BaudRate = baudrate;
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	SPI_InitStruct.CRCPoly = 0UL;
	LL_SPI_Init(SPI1, &SPI_InitStruct);
}


void SPI1_Init(uint32_t datawidth, uint32_t cpha, uint32_t cpol, uint32_t nss, uint32_t baudrate)
{
	LL_RCC_SetSPIClockSource(LL_RCC_SPI123_CLKSOURCE_PLL1Q);
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
	SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOAEN);
	SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOBEN);
	//SPI1 GPIO Configuration
	//PA15 (JTDI)     ------> SPI1_NSS
	//PB3 (JTDO/TRACESWO)     ------> SPI1_SCK
	//PB4 (NJTRST)     ------> SPI1_MISO
	//PB5     ------> SPI1_MOSI

	LL_SPI_Disable(SPI1);

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
	if (nss == LL_SPI_NSS_SOFT)
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; //AF PP mode
	else
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE; //AF PP mode

	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODER_MODE0_1; //AF PP mode
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	NVIC_SetPriority(SPI1_IRQn, 0);

	NVIC_EnableIRQ(SPI1_IRQn);

	SPI1_InitSpi(datawidth, cpha, cpol, nss, baudrate);

	LL_SPI_DisableGPIOControl(SPI1);

	LL_SPI_DisableMasterRxAutoSuspend(SPI1);

	if (nss == LL_SPI_NSS_SOFT)
	{
		LL_SPI_Enable(SPI1);
		LL_SPI_StartMasterTransfer(SPI1);
	}

}

Result_t SPI_Transmit_Receive(SPI_TypeDef *SPI, uint8_t *txData, uint8_t *rxData, uint32_t count)
{
	uint32_t tickstart = 0;

	for (uint32_t i = 0; i < count; ++i)
	{
		// wait for space in fifo
		tickstart = Systick_GetTick();
		while (LL_SPI_IsActiveFlag_TXP(SPI) == 0)
		{
			if ((Systick_GetTick() - tickstart) > SPI_TIMEOUT_VALUE)
				return OOF_SPI_TXP_FULL;
		}

		LL_SPI_TransmitData8(SPI, txData[i]);

		// wait for rx data available
		tickstart = Systick_GetTick();
		while (!LL_SPI_IsActiveFlag_RXP(SPI))
		{
			if ((Systick_GetTick() - tickstart) > SPI_TIMEOUT_VALUE)
				return OOF_SPI_NO_EOT;
		}

		if (rxData)
			rxData[i] = LL_SPI_ReceiveData8(SPI);
		else
			LL_SPI_ReceiveData8(SPI);
	}
	for (volatile uint16_t c = 0; c < 0xFF; c++)
	{
	}
	return NOICE;
}

Result_t SPI_Transmit_Receive_HardNSS(SPI_TypeDef *SPI, int32_t txData[], int32_t rxData[], uint32_t count)
{
	uint32_t tickstart = 0;

	LL_SPI_SetTransferSize(SPI, count);
	LL_SPI_Enable(SPI);
	LL_SPI_StartMasterTransfer(SPI);

	for (uint32_t i = 0; i < count; ++i)
	{
		tickstart = Systick_GetTick();
		while (LL_SPI_IsActiveFlag_TXP(SPI) == 0)
		{
			if ((Systick_GetTick() - tickstart) > SPI_TIMEOUT_VALUE)
				return OOF_SPI_TXP_FULL;
		}
		LL_SPI_TransmitData32(SPI, txData[i]);
	}

	for (uint32_t i = 0; i < count; ++i)
	{
		tickstart = Systick_GetTick();
		while (LL_SPI_IsActiveFlag_RXP(SPI) == 0)
		{
			if ((Systick_GetTick() - tickstart) > SPI_TIMEOUT_VALUE)
				return OOF_SPI_RXP_FULL;
		}
		rxData[i] = LL_SPI_ReceiveData32(SPI);
	}

	tickstart = Systick_GetTick();
	while (LL_SPI_IsActiveFlag_EOT(SPI) == 0)
	{
		if ((Systick_GetTick() - tickstart) > SPI_TIMEOUT_VALUE)
			return OOF_SPI_NO_EOT;
	}

	LL_SPI_ClearFlag_EOT(SPI);
	LL_SPI_ClearFlag_TXTF(SPI);
	LL_SPI_SuspendMasterTransfer(SPI);
	LL_SPI_Disable(SPI);

	//return (LL_SPI_IsActiveFlag_OVR(SPI)) ? OOF : NOICE;
	return NOICE;
}
