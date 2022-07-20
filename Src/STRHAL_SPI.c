#include "../Inc/STRHAL_SPI.h"
#include "../Inc/STRHAL_SysTick.h"

#include <stm32h7xx_ll_bus.h>
#include <stm32h7xx_ll_rcc.h>
#include <stm32h7xx_ll_gpio.h>

#include <stddef.h>

typedef struct {
	GPIO_TypeDef *port;
	uint32_t pin;
	uint32_t afn;
	STRHAL_SPI_Id_t spi;
} STRHAL_SPI_IO_t;

//TODO: add spi4,5,6
const STRHAL_SPI_IO_t _scks[STRHAL_SPI_N_SCK] = {
	[STRHAL_SPI_SPI1_SCK_PA5] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},
	[STRHAL_SPI_SPI1_SCK_PB3] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},
	[STRHAL_SPI_SPI1_SCK_PG11] = {
		.port = GPIOG,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},

	[STRHAL_SPI_SPI2_SCK_PA9] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_SCK_PA12] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_SCK_PB10] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_SCK_PB13] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_13,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_SCK_PD3] = {
		.port = GPIOD,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_SCK_PI1] = {
		.port = GPIOI,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},

	[STRHAL_SPI_SPI3_SCK_PB3] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_6,
		.spi = STRHAL_SPI_SPI3,
	},
	[STRHAL_SPI_SPI3_SCK_PC10] = {
		.port = GPIOC,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_6,
		.spi = STRHAL_SPI_SPI3,
	},
};

const STRHAL_SPI_IO_t _misos[STRHAL_SPI_N_MISO] = {
	[STRHAL_SPI_SPI1_MISO_PA6] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},
	[STRHAL_SPI_SPI1_MISO_PB4] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},
	[STRHAL_SPI_SPI1_MISO_PG9] = {
		.port = GPIOG,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},

	[STRHAL_SPI_SPI2_MISO_PC2] = {
		.port = GPIOC,
		.pin = LL_GPIO_PIN_2,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_MISO_PB14] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_14,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_MISO_PI2] = {
		.port = GPIOI,
		.pin = LL_GPIO_PIN_2,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},

	[STRHAL_SPI_SPI3_MISO_PB4] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_6,
		.spi = STRHAL_SPI_SPI3,
	},
	[STRHAL_SPI_SPI3_MISO_PC11] = {
		.port = GPIOC,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_6,
		.spi = STRHAL_SPI_SPI3,
	},
};

const STRHAL_SPI_IO_t _mosis[STRHAL_SPI_N_MOSI] = {
	[STRHAL_SPI_SPI1_MOSI_PA7] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},
	[STRHAL_SPI_SPI1_MOSI_PB5] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},
	[STRHAL_SPI_SPI1_MOSI_PD7] = {
		.port = GPIOD,
		.pin = LL_GPIO_PIN_7,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},

	[STRHAL_SPI_SPI2_MOSI_PB15] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_MOSI_PC1] = {
		.port = GPIOC,
		.pin = LL_GPIO_PIN_1,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_MOSI_PC3] = {
		.port = GPIOC,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_MOSI_PI3] = {
		.port = GPIOI,
		.pin = LL_GPIO_PIN_3,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},

	[STRHAL_SPI_SPI3_MOSI_PB2] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_2,
		.afn = LL_GPIO_AF_7,
		.spi = STRHAL_SPI_SPI3,
	},
	[STRHAL_SPI_SPI3_MOSI_PB5] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_5,
		.afn = LL_GPIO_AF_7,
		.spi = STRHAL_SPI_SPI3,
	},
	[STRHAL_SPI_SPI3_MOSI_PC12] = {
		.port = GPIOC,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_6,
		.spi = STRHAL_SPI_SPI3,
	},
	[STRHAL_SPI_SPI3_MOSI_PD6] = {
		.port = GPIOD,
		.pin = LL_GPIO_PIN_6,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI3,
	},
};

const STRHAL_SPI_IO_t _nsss[STRHAL_SPI_N_NSS] = {
	[STRHAL_SPI_SPI1_NSS_PA4] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},
	[STRHAL_SPI_SPI1_NSS_PA15] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},
	[STRHAL_SPI_SPI1_NSS_PG10] = {
		.port = GPIOG,
		.pin = LL_GPIO_PIN_10,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI1,
	},

	[STRHAL_SPI_SPI2_NSS_PA11] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_11,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_NSS_PB4] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_7,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_NSS_PB9] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_9,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_NSS_PB12] = {
		.port = GPIOB,
		.pin = LL_GPIO_PIN_12,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},
	[STRHAL_SPI_SPI2_NSS_PI0] = {
		.port = GPIOI,
		.pin = LL_GPIO_PIN_0,
		.afn = LL_GPIO_AF_5,
		.spi = STRHAL_SPI_SPI2,
	},

	[STRHAL_SPI_SPI3_NSS_PA4] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_4,
		.afn = LL_GPIO_AF_6,
		.spi = STRHAL_SPI_SPI3,
	},
	[STRHAL_SPI_SPI3_NSS_PA15] = {
		.port = GPIOA,
		.pin = LL_GPIO_PIN_15,
		.afn = LL_GPIO_AF_6,
		.spi = STRHAL_SPI_SPI3,
	},
};

typedef struct {
	SPI_TypeDef *spix;
	uint16_t psc;

	STRHAL_SPI_Mode_t mode;
	const STRHAL_SPI_IO_t * mosi, * miso, * sck, * nss;

} STRHAL_SPI_t;

STRHAL_SPI_t _spis[STRHAL_SPI_N_SPI] = {
	[STRHAL_SPI_SPI1] = {
		.spix = SPI1,
	},
	[STRHAL_SPI_SPI2] = {
		.spix = SPI2,
	},
	[STRHAL_SPI_SPI3] = {
		.spix = SPI3,
	},
	/*[STRHAL_SPI_SPI4] = {
		.spix = SPI4,
	},
	[STRHAL_SPI_SPI5] = {
		.spix = SPI5,
	},
	[STRHAL_SPI_SPI6] = {
		.spix = SPI6,
	},*/
};

static inline int _wait_for_txp(SPI_TypeDef *spix, uint16_t tot);
static inline int _wait_for_eot(SPI_TypeDef *spix, uint16_t tot);
static inline int _wait_for_rxp(SPI_TypeDef *spix, uint16_t tot);
static inline int _rx_flush(SPI_TypeDef *spix, uint16_t tot);


void STRHAL_SPI_Init() {
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
	//TODO: add gpiog etc(?)

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);

	/*
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI4);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI5);
	LL_APB4_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI6);
	*/

	LL_SPI_DeInit(SPI1);
	LL_SPI_DeInit(SPI2);
	LL_SPI_DeInit(SPI3);
	/*
	LL_SPI_DeInit(SPI4);
	LL_SPI_DeInit(SPI5);
	LL_SPI_DeInit(SPI6);
	*/
	LL_SPI_Disable(SPI1);
	LL_SPI_Disable(SPI2);
	LL_SPI_Disable(SPI3);
	/*
	LL_SPI_Disable(SPI4);
	LL_SPI_Disable(SPI5);
	LL_SPI_Disable(SPI6);
	*/
}

void STRHAL_SPI_Master_Run(STRHAL_SPI_Id_t spi_id) {
	if(_spis[spi_id].mode != STRHAL_SPI_MODE_MASTER)
		return;

	//LL_SPI_SetTransferSize(_spis[spi_id].spix->spix, 0);
	LL_SPI_Enable(_spis[spi_id].spix);
	LL_SPI_StartMasterTransfer(_spis[spi_id].spix);
}

void STRHAL_SPI_Master_Stop(STRHAL_SPI_Id_t spi_id) {
	if(_spis[spi_id].mode != STRHAL_SPI_MODE_MASTER)
		return;

	//TODO: sequence handling rm s. 2198
	LL_SPI_SuspendMasterTransfer(_spis[spi_id].spix);
	LL_SPI_Disable(_spis[spi_id].spix);
}

int32_t STRHAL_SPI_Master_Init(STRHAL_SPI_Id_t spi_id, const STRHAL_SPI_Config_t *config) {
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;

	GPIO_InitStruct.Pin = _mosis[config->mosi].pin;
	GPIO_InitStruct.Alternate = _mosis[config->mosi].afn;
	LL_GPIO_Init(_mosis[config->mosi].port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = _misos[config->miso].pin;
	GPIO_InitStruct.Alternate = _misos[config->miso].afn;
	LL_GPIO_Init(_misos[config->miso].port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = _scks[config->sck].pin;
	GPIO_InitStruct.Alternate = _scks[config->sck].afn;
	LL_GPIO_Init(_scks[config->sck].port, &GPIO_InitStruct);


	if(config->mode == STRHAL_SPI_MODE_MASTER) {
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.Pin = _nsss[config->nss].pin;
		LL_GPIO_SetOutputPin(_nsss[config->nss].port, _nsss[config->nss].pin);
		LL_GPIO_Init(_nsss[config->nss].port, &GPIO_InitStruct);
		LL_GPIO_SetOutputPin(_nsss[config->nss].port, _nsss[config->nss].pin);
	} else {
		GPIO_InitStruct.Pin = _nsss[config->nss].pin;
		GPIO_InitStruct.Alternate = _nsss[config->nss].afn;
		LL_GPIO_Init(_scks[config->nss].port, &GPIO_InitStruct);
	}

	LL_SPI_InitTypeDef SPI_InitStruct = {0};
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
	SPI_InitStruct.BitOrder = config->lsb_first << SPI_CFG2_LSBFRST_Pos;
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	SPI_InitStruct.CRCPoly = 10;
	SPI_InitStruct.BaudRate = config->psc << SPI_CFG1_MBR_Pos;
	SPI_InitStruct.ClockPhase = config->c_pol_phase & SPI_CFG2_CPHA_Msk;
	SPI_InitStruct.ClockPolarity = config->c_pol_phase & SPI_CFG2_CPOL_Msk;

	LL_SPI_Init(_spis[spi_id].spix, &SPI_InitStruct);

	_spis[spi_id].sck = &_scks[config->sck];
	_spis[spi_id].miso = &_misos[config->miso];
	_spis[spi_id].mosi = &_mosis[config->mosi];
	_spis[spi_id].nss = &_nsss[config->nss];
	_spis[spi_id].psc = config->psc;

	return SystemCoreClock / ((uint32_t) 1<<config->psc);
}

//TODO: test this
int32_t STRHAL_SPI_Master_Transceive(STRHAL_SPI_Id_t spi_id, const uint8_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, uint8_t *rx_data, uint32_t rx_n, uint16_t tot) {
	STRHAL_SPI_t *spi = &_spis[spi_id];

	if(LL_SPI_IsEnabled(spi->spix))
			return -1;

	LL_SPI_SetDataWidth(spi->spix, LL_SPI_DATAWIDTH_8BIT);
	LL_SPI_SetTransferSize(spi->spix, (rx_skip_n + rx_n));
	LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA);

	STRHAL_SPI_Master_Run(spi_id);

	LL_GPIO_ResetOutputPin(spi->nss->port, spi->nss->pin);

	uint32_t tx_counter=0, rx_counter=0, rx_n0=rx_n;
	uint64_t start = STRHAL_Systick_GetTick();
	while((rx_n+rx_skip_n) > 0) {
		if(LL_SPI_IsActiveFlag_TXP(spi->spix) && tx_n>0) {
			LL_SPI_TransmitData8(spi->spix, tx_data[tx_counter]);
			tx_n--;
			tx_counter++;
		}
		else if(tx_n == 0 && rx_n > 0) {
			LL_SPI_TransmitData8(spi->spix, 0);
		}

		if((LL_SPI_IsActiveFlag_RXWNE(spi->spix) || LL_SPI_IsActiveFlag_EOT(spi->spix)) && rx_n>0) {
			if(rx_skip_n == 0) {
				rx_data[rx_counter] = LL_SPI_ReceiveData8(spi->spix);
				rx_n--;
				rx_counter++;
			}
			else {
				uint32_t waste = LL_SPI_ReceiveData8(spi->spix);
				(void) waste;
				rx_skip_n--;
			}
		}

		if(STRHAL_Systick_GetTick() - start >= tot) {
			LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
			return -1;
		}
	}

	LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
	LL_SPI_ClearFlag_EOT(spi->spix);
	LL_SPI_ClearFlag_TXTF(spi->spix);
	STRHAL_SPI_Master_Stop(spi_id);

	return rx_n0;
}

//this works
int32_t STRHAL_SPI_Master_Transceive24(STRHAL_SPI_Id_t spi_id, const int32_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, int32_t *rx_data, uint32_t rx_n, uint16_t tot) {
	STRHAL_SPI_t *spi = &_spis[spi_id];

	if(LL_SPI_IsEnabled(spi->spix))
		return -1;

	LL_SPI_SetDataWidth(spi->spix, LL_SPI_DATAWIDTH_24BIT);
	LL_SPI_SetTransferSize(spi->spix, (rx_skip_n + rx_n));
	LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA);

	STRHAL_SPI_Master_Run(spi_id);

	LL_GPIO_ResetOutputPin(spi->nss->port, spi->nss->pin);

	uint32_t tx_counter=0, rx_counter=0, rx_n0=rx_n;
	uint64_t start = STRHAL_Systick_GetTick();
	while((rx_n+rx_skip_n) > 0) {
		if(LL_SPI_IsActiveFlag_TXP(spi->spix) && tx_n>0) {
			LL_SPI_TransmitData32(spi->spix, tx_data[tx_counter]);
			tx_n--;
			tx_counter++;
		}
		else if(tx_n == 0 && rx_n > 0) {
			LL_SPI_TransmitData32(spi->spix, 0);
		}

		if((LL_SPI_IsActiveFlag_RXWNE(spi->spix) || LL_SPI_IsActiveFlag_EOT(spi->spix)) && rx_n>0) {
			if(rx_skip_n == 0) {
				rx_data[rx_counter] = LL_SPI_ReceiveData32(spi->spix);
				rx_n--;
				rx_counter++;
			}
			else {
				uint32_t waste = LL_SPI_ReceiveData32(spi->spix);
				(void) waste;
				rx_skip_n--;
			}
		}

		if(STRHAL_Systick_GetTick() - start >= tot) {
			LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
			return -1;
		}
	}

	LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
	LL_SPI_ClearFlag_EOT(spi->spix);
	LL_SPI_ClearFlag_TXTF(spi->spix);
	STRHAL_SPI_Master_Stop(spi_id);

	return rx_n0;
}

int _wait_for_txp(SPI_TypeDef *spix, uint16_t tot) {
	uint64_t start = STRHAL_Systick_GetTick();
	while(!LL_SPI_IsActiveFlag_TXP(spix)) {
		if(STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return 0;
}

int _wait_for_eot(SPI_TypeDef *spix, uint16_t tot) {
	uint64_t start = STRHAL_Systick_GetTick();
	while(!LL_SPI_IsActiveFlag_EOT(spix)) {
		if(STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return _rx_flush(spix, tot);
}

int _wait_for_rxp(SPI_TypeDef *spix, uint16_t tot) {
	uint64_t start = STRHAL_Systick_GetTick();
	while(!LL_SPI_IsActiveFlag_RXP(spix)) {
		if(STRHAL_Systick_GetTick() - start > tot)
			return -1;
	}
	return 0;
}

int _rx_flush(SPI_TypeDef *spix, uint16_t tot) {
	uint64_t start = STRHAL_Systick_GetTick();

	while(LL_SPI_GetRxFIFOPackingLevel(spix) != LL_SPI_RX_FIFO_0PACKET) {
		if(STRHAL_Systick_GetTick() - start > tot)
			return -1;

		uint16_t x = spix->RXDR;
		(void)(x);
	}
	return 0;
}
