/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"
#include "systick.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

//SPI_HandleTypeDef hspi1;
//DMA_HandleTypeDef hdma_spi1_rx;

/* SPI1 init function */
void SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_24BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0x0;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }*/
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */
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

  LL_SPI_InitTypeDef SPI_InitStruct = { 0 };
  //LL_SPI_StructInit(SPI_InitStruct);
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode              = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_24BIT;
  SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS               = LL_SPI_NSS_HARD_OUTPUT;
  SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV32;
  SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly           = 0UL;

  LL_SPI_Init(SPI1, &SPI_InitStruct);

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

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
