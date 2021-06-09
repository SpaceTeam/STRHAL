#include "spi.h"
#include "systick.h"

static void SPI1_InitSpi(uint32_t datawidth, uint32_t cpha, uint32_t cpol, uint32_t nss, uint32_t baudrate)
{
	LL_SPI_InitTypeDef SPI_InitStruct =
	{ 0 };
	//LL_SPI_StructInit(SPI_InitStruct);
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = datawidth;
	SPI_InitStruct.ClockPolarity = cpol;
	SPI_InitStruct.ClockPhase = cpha;
	SPI_InitStruct.NSS = nss;
	SPI_InitStruct.BaudRate = baudrate;
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	SPI_InitStruct.CRCPoly = 7UL;
	LL_SPI_Init(SPI1, &SPI_InitStruct);
}

void SPI1_Init(uint32_t datawidth, uint32_t cpha, uint32_t cpol, uint32_t nss, uint32_t baudrate)
{
  LL_RCC_SetSPIClockSource(LL_RCC_SPI123_CLKSOURCE_PLL1Q);

  /* SPI1 clock enable */
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);

  SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOAEN);

  SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOBEN);
	/**SPI1 GPIO Configuration
	PA15 (JTDI)     ------> SPI1_NSS
	PB3 (JTDO/TRACESWO)     ------> SPI1_SCK
	PB4 (NJTRST)     ------> SPI1_MISO
	PB5     ------> SPI1_MOSI
	*/
  LL_GPIO_InitTypeDef GPIO_InitStruct  = {0};

  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODER_MODE0_1; //AF PP mode
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODER_MODE0_1; //AF PP mode
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* SPI1 DMA Init */
  /* SPI1_RX Init */
  /*hdma_spi1_rx.Instance = DMA1_Stream1;
  hdma_spi1_rx.Init.Request = DMA_REQUEST_SPI1_RX;
  hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_spi1_rx.Init.Mode = DMA_CIRCULAR;
  hdma_spi1_rx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_spi1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;*/

  /*LL_DMA_InitTypeDef DMA_InitStruct = {0};

  DMA_InitStruct.PeriphOrM2MSrcAddress  = 0x00000000U;
  DMA_InitStruct.MemoryOrM2MDstAddress  = 0x00000000U;
  DMA_InitStruct.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  DMA_InitStruct.Mode                   = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_NOINCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_WORD;
  DMA_InitStruct.NbData                 = 0x00000000U;
  DMA_InitStruct.PeriphRequest          = LL_DMAMUX1_REQ_SPI1_RX;
  DMA_InitStruct.Priority               = LL_DMA_PRIORITY_LOW;
  DMA_InitStruct.FIFOMode               = LL_DMA_FIFOMODE_DISABLE;
  DMA_InitStruct.FIFOThreshold          = LL_DMA_FIFOTHRESHOLD_1_4;
  DMA_InitStruct.MemBurst               = LL_DMA_MBURST_SINGLE;
  DMA_InitStruct.PeriphBurst            = LL_DMA_PBURST_SINGLE;*/

  //LL_DMA_Init(DMA_TypeDef *DMAx, DMA1_Stream1, DMA_InitStruct);
  //if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK)
  //{
  //  Error_Handler();
  //}

  //__HAL_LINKDMA(spiHandle,hdmarx,hdma_spi1_rx);

  /* SPI1 interrupt Init */
  /* Set priority for SPI1_IRQn */
  NVIC_SetPriority(SPI1_IRQn, 0);
  /* Enable SPI1_IRQn */
  NVIC_EnableIRQ(SPI1_IRQn);

  SPI1_InitSpi(datawidth, cpha, cpol, nss, baudrate);

  //LL_SPI_SetFIFOThreshold(SPI1,LL_SPI_FIFO_TH_04DATA);

  /* Lock GPIO for master to avoid glitches on the clock output */

  LL_SPI_DisableGPIOControl(SPI1);

  LL_SPI_DisableMasterRxAutoSuspend(SPI1);

  //LL_SPI_SetTransferSize(SPI1, 2);


  /* Enable RXP Interrupt */

  //LL_SPI_EnableIT_RXP(SPI1);

}

Result_t SPI_Transmit_Receive(SPI_TypeDef * SPI, int32_t txData[], int32_t rxData[], uint32_t count)
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
