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
	//LL_SPI_StartMasterTransfer(_spis[spi_id].spix);
	LL_SPI_Enable(_spis[spi_id].spix);
}

void STRHAL_SPI_Master_Stop(STRHAL_SPI_Id_t spi_id) {
	if(_spis[spi_id].mode != STRHAL_SPI_MODE_MASTER)
		return;

	//TODO: sequence handling rm s. 2198
	//LL_SPI_SuspendMasterTransfer(_spis[spi_id].spix);
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
	SPI_InitStruct.ClockPhase = config->c_pol_phase & 0x1;
	SPI_InitStruct.ClockPolarity = config->c_pol_phase & 0x2;

	LL_SPI_Init(_spis[spi_id].spix, &SPI_InitStruct);

	_spis[spi_id].sck = &_scks[config->sck];
	_spis[spi_id].miso = &_misos[config->miso];
	_spis[spi_id].mosi = &_mosis[config->mosi];
	_spis[spi_id].nss = &_nsss[config->nss];
	_spis[spi_id].psc = config->psc;

	return SystemCoreClock / ((uint32_t) 1<<config->psc);
}
/*
int32_t STRHAL_SPI_Master_Transceive24(STRHAL_SPI_Id_t spi_id, const int32_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, int32_t *rx_data, uint32_t rx_n, uint8_t word_size, uint16_t tot) {

	static uint32_t i;
	i=0;

	STRHAL_SPI_t *spi = &_spis[spi_id];
	if(spi->mode != STRHAL_SPI_MODE_MASTER)
		return -1;

	if(rx_skip_n + rx_n < tx_n) {
		return -1;
	}

	if(rx_skip_n > tx_n) {
		rx_skip_n = tx_n;
		rx_n = 0;
	}

	LL_SPI_SetTransferSize(spi->spix, rx_skip_n + rx_n);
	LL_SPI_StartMasterTransfer(spi->spix);
	STRHAL_SPI_Master_Run(spi_id);

	LL_GPIO_ResetOutputPin(spi->nss->port, spi->nss->pin);

	uint32_t rx_n0 = rx_n;

	while((tx_n + rx_n) > 0) {
		LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA); // reset FIFO threshold to common full 16bit transceptions

		if(_wait_for_txp(spi->spix, tot) < 0) {
			LL_GPIO_SetOutputPin(_spis[spi_id].nss->port, _spis[spi_id].nss->pin);
			return -1;
		}

		if(tx_n >= 1) {
			//more than one byte left to transmit
			LL_SPI_TransmitData8(spi->spix, ((uint8_t) ((*tx_data)>>((word_size*8-8)-(i%word_size)*8)) & 0xFF));
			tx_n -= 1; //decrement tx byte counter by 2 bytes

			if(tx_n == 1 && (rx_n + rx_skip_n) > 1) {
				LL_SPI_TransmitData16(spi->spix, (uint16_t) *(tx_data++) | 0x69);
			}
		}
		else if(tx_n == 1){
			//last tx byte left to transmit
			if((rx_n + rx_skip_n) > 1) {
				//there is still data left to read without specified tx data => append dummy byte filling 2byte DR
				LL_SPI_TransmitData16(spi->spix, (uint16_t) *(tx_data++) | 0x6900);
			} else {
				//last transmission is tx only byte => reset FIFO Threshold to monitor reception of single byte
				LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA);
				LL_SPI_TransmitData8(spi->spix, *tx_data++);
			}
			tx_n --; //decrement tx_n <=> tx_n = 0
		}
		else if(rx_n > 1) {
			//there is are more than two bytes ready to ONLY READ (without corresponding tx data) => load DR with dummy data for transmission
			LL_SPI_TransmitData16(spi->spix, 0x6969);
		}
		else {
			//there is only one byte left to ONLY READ (without corresponding tx data) => reset FIFO Threshold to monitor reception of single byte and load dummy byte for transmission
			LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA);
			LL_SPI_TransmitData8(spi->spix, 0x69);
		}

		//wait for reception of 1 or 2 bytes depending on FIFO-threshold
		if(_wait_for_rxp(spi->spix, tot) < 0) {
			LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
			return -1;
		}

		//full read on DR regardless of actual reception size (1 or 2 bytes)

		if(rx_skip_n > 1) {
			uint16_t waste = LL_SPI_ReceiveData16(spi->spix);
			(void) waste;
			rx_skip_n -= sizeof(uint16_t);
		}
		else if(rx_skip_n == 1) {
			rx_skip_n--;
			if(rx_n > 0) {
				*(rx_data++) = (uint8_t) ((LL_SPI_ReceiveData16(spi->spix) >> 8) & 0xFF);
				rx_n --;
			} else {
				uint8_t waste = *((__IO uint8_t *) &spi->spix->RXDR);
				(void) waste;
			}
		}
		else if(rx_skip_n == 0 && rx_n > 1) {
			*((uint16_t *) rx_data) =  LL_SPI_ReceiveData16(spi->spix);
			rx_data += 2;
			rx_n -= 2;
		}
		else if(rx_skip_n == 0 && rx_n == 1) {
			*(rx_data++) = LL_SPI_ReceiveData8(spi->spix);
			rx_n --;
		}
		i++;
		if(i%word_size == 0) {
			tx_data++;//increment data address head
		}

	}

	if(_wait_for_eot(spi->spix, tot) < 0) {
		LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
		return -1;
	}

	LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
	LL_SPI_SuspendMasterTransfer(spi->spix);
	STRHAL_SPI_Master_Stop(spi_id);
	return (int32_t) rx_n0;
}
*//*
int32_t STRHAL_SPI_Master_Transceive(STRHAL_SPI_Id_t spi_id, const uint8_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, uint8_t *rx_data, uint32_t rx_n, uint16_t tot) {
	STRHAL_SPI_t *spi = &_spis[spi_id];
	if(spi->mode != STRHAL_SPI_MODE_MASTER)
		return -1;

	if(rx_skip_n + rx_n < tx_n) {
		return -1;
	}

	if(rx_skip_n > tx_n) {
		rx_skip_n = tx_n;
		rx_n = 0;
	}

	uint32_t rx_n0 = rx_n;

	//TODO: insert start master transfer

	while((tx_n + rx_n) > 0) {
		LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_02DATA); // reset FIFO threshold to common full 16bit transceptions

		if(_wait_for_txp(spi->spix, tot) < 0) {
			LL_GPIO_SetOutputPin(_spis[spi_id].nss->port, _spis[spi_id].nss->pin);
			return -1;
		}

		if(tx_n > 1) {
			//more than one byte left to transmit
			LL_SPI_TransmitData16(spi->spix, *((uint16_t *) tx_data));
			LL_SPI_StartMasterTransfer(spi->spix);
			tx_n -= 2; //decrement tx byte counter by 2 bytes
			tx_data += 2;//increment data address head
		}
		else if(tx_n == 1){
			//last tx byte left to transmit
			if((rx_n + rx_skip_n) > 1) {
				//there is still data left to read without specified tx data => append dummy byte filling 2byte DR
				LL_SPI_TransmitData16(spi->spix, (uint16_t) *(tx_data++) | 0x6900);
			} else {
				//last transmission is tx only byte => reset FIFO Threshold to monitor reception of single byte
				LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA);
				LL_SPI_TransmitData8(spi->spix, *tx_data++);
			}
			tx_n --; //decrement tx_n <=> tx_n = 0
		}
		else if(rx_n > 1) {
			//there is are more than two bytes ready to ONLY READ (without corresponding tx data) => load DR with dummy data for transmission
			LL_SPI_TransmitData16(spi->spix, 0x6969);
		}
		else {
			//there is only one byte left to ONLY READ (without corresponding tx data) => reset FIFO Threshold to monitor reception of single byte and load dummy byte for transmission
			LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA);
			LL_SPI_TransmitData8(spi->spix, 0x69);
		}

		//wait for reception of 1 or 2 bytes depending on FIFO-threshold
		if(_wait_for_rxp(spi->spix, tot) < 0) {
			LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
			return -1;
		}

		//full read on DR regardless of actual reception size (1 or 2 bytes)

		if(rx_skip_n > 1) {
			uint16_t waste = LL_SPI_ReceiveData16(spi->spix);
			(void) waste;
			rx_skip_n -= sizeof(uint16_t);
		}
		else if(rx_skip_n == 1) {
			rx_skip_n--;
			if(rx_n > 0) {
				*(rx_data++) = (uint8_t) ((LL_SPI_ReceiveData16(spi->spix) >> 8) & 0xFF);
				rx_n --;
			} else {
				uint8_t waste = *((__IO uint8_t *) &spi->spix->RXDR);
				(void) waste;
			}
		}
		else if(rx_skip_n == 0 && rx_n > 1) {
			*((uint16_t *) rx_data) =  LL_SPI_ReceiveData16(spi->spix);
			rx_data += 2;
			rx_n -= 2;
		}
		else if(rx_skip_n == 0 && rx_n == 1) {
			*(rx_data++) = LL_SPI_ReceiveData8(spi->spix);
			rx_n --;
		}
	}


	return (int32_t) rx_n0;
}*/
/*
int32_t STRHAL_SPI_Master_Transceive(STRHAL_SPI_Id_t spi_id, const uint8_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, uint8_t *rx_data, uint32_t rx_n, uint16_t tot) {
	STRHAL_SPI_t *spi = &_spis[spi_id];
	if(spi->mode != STRHAL_SPI_MODE_MASTER)
		return -1;

	if(rx_skip_n + rx_n < tx_n) {
		return -1;
	}

	if(rx_skip_n > tx_n) {
		rx_skip_n = tx_n;
		rx_n = 0;
	}

	uint32_t rx_n0 = rx_n;

	//TODO: insert start master transfer

	while((tx_n + rx_n) > 0) {

		if(_wait_for_txp(spi->spix, tot) < 0) {
			LL_GPIO_SetOutputPin(_spis[spi_id].nss->port, _spis[spi_id].nss->pin);
			return -1;
		}

		if(tx_n > 1) {
			//more than one byte left to transmit
			LL_SPI_TransmitData8(spi->spix, *(tx_data));
			LL_SPI_StartMasterTransfer(spi->spix);
			tx_n -= 1; //decrement tx byte counter by 2 bytes
			tx_data ++;//increment data address head
		}
		else if(tx_n == 1){
			//last tx byte left to transmit
			if((rx_n + rx_skip_n) > 1) {
				//there is still data left to read without specified tx data => append dummy byte filling 2byte DR
				LL_SPI_TransmitData16(spi->spix, (uint16_t) *(tx_data++) | 0x69);
			} else {
				//last transmission is tx only byte => reset FIFO Threshold to monitor reception of single byte

				LL_SPI_TransmitData8(spi->spix, *tx_data++);
			}
			tx_n --; //decrement tx_n <=> tx_n = 0
		}
		else if(rx_n > 1) {
			//there is are more than two bytes ready to ONLY READ (without corresponding tx data) => load DR with dummy data for transmission
			LL_SPI_TransmitData16(spi->spix, 0x6969);
		}
		else {
			//there is only one byte left to ONLY READ (without corresponding tx data)
			LL_SPI_TransmitData8(spi->spix, 0x69);
		}

		//wait for reception of 1 or 2 bytes depending on FIFO-threshold
		if(_wait_for_rxp(spi->spix, tot) < 0) {
			LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
			return -1;
		}

		//full read on DR regardless of actual reception size (1 or 2 bytes)

		if(rx_skip_n > 1) {
			uint16_t waste = LL_SPI_ReceiveData16(spi->spix);
			(void) waste;
			rx_skip_n -= sizeof(uint16_t);
		}
		else if(rx_skip_n == 1) {
			rx_skip_n--;
			if(rx_n > 0) {
				*(rx_data++) = (uint8_t) ((LL_SPI_ReceiveData16(spi->spix) >> 8) & 0xFF);
				rx_n --;
			} else {
				uint8_t waste = *((__IO uint8_t *) &spi->spix->RXDR);
				(void) waste;
			}
		}
		else if(rx_skip_n == 0 && rx_n > 1) {
			*((uint16_t *) rx_data) =  LL_SPI_ReceiveData16(spi->spix);
			rx_data += 2;
			rx_n -= 2;
		}
		else if(rx_skip_n == 0 && rx_n == 1) {
			*(rx_data++) = LL_SPI_ReceiveData8(spi->spix);
			rx_n --;
		}
	}


	return (int32_t) rx_n0;
}

//transcieve 24bit frames via 3*8bit frames, MSB first
int32_t STRHAL_SPI_Master_Transceive24(STRHAL_SPI_Id_t spi_id, const int32_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, int32_t *rx_data, uint32_t rx_n, uint16_t tot) {
	static uint8_t txdata8[3] = {0};
	static uint8_t rxdata8[3] = {0};

	STRHAL_SPI_t *spi = &_spis[spi_id];
	LL_SPI_SetTransferSize(spi->spix, (rx_skip_n + rx_n)*3);
	LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA); // reset FIFO threshold to common full 8bit transceptions


	STRHAL_SPI_Master_Run(spi_id);

	LL_GPIO_ResetOutputPin(spi->nss->port, spi->nss->pin);

	for(uint32_t i=0; i<rx_n+rx_skip_n; i++) {

		rxdata8[0] = 0;
		rxdata8[1] = 0;
		rxdata8[2] = 0;
		if(i<tx_n) {

			txdata8[0] = (tx_data[i]>>16) & 0xFF;
			txdata8[1] = (tx_data[i]>>8) & 0xFF;
			txdata8[2] = (tx_data[i]) & 0xFF;
		}
		else {
			txdata8[0] = 0;
			txdata8[1] = 0;
			txdata8[2] = 0;
		}

		if(STRHAL_SPI_Master_Transceive(spi_id, txdata8, 3, 0, rxdata8, 3, tot) != 3)
			return -1;

		if(i>=rx_skip_n) {
			rx_data[i-rx_skip_n] = (rxdata8[0]<<16) | (rxdata8[1]<<8) | (rxdata8[2]);
		}
		else {
			uint32_t waste = (rxdata8[0]<<16) | (rxdata8[1]<<8) | (rxdata8[2]);
			(void) waste;
		}
	}
	if(_wait_for_eot(spi->spix, tot) < 0) {
		LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
		return -1;
	}

	LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
	LL_SPI_SuspendMasterTransfer(spi->spix);
	STRHAL_SPI_Master_Stop(spi_id);

	return rx_n;
}
*/
//transcieve 24bit frames via 3*8bit frames, MSB first
int32_t STRHAL_SPI_Master_Transceive24(STRHAL_SPI_Id_t spi_id, const int32_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, int32_t *rx_data, uint32_t rx_n, uint16_t tot) {
	static uint8_t txdata8[3] = {0};
	static uint8_t rxdata8[3] = {0};

	uint32_t rx_n0 = rx_n;

	STRHAL_SPI_t *spi = &_spis[spi_id];
	LL_SPI_SetTransferSize(spi->spix, (rx_skip_n + rx_n)*3);
	LL_SPI_SetFIFOThreshold(spi->spix, LL_SPI_FIFO_TH_01DATA); // reset FIFO threshold to common full 24bit transceptions


	STRHAL_SPI_Master_Run(spi_id);

	LL_GPIO_ResetOutputPin(spi->nss->port, spi->nss->pin);

	for(uint32_t i=0; i<rx_n+rx_skip_n; i++) {

		rxdata8[0] = 0;
		rxdata8[1] = 0;
		rxdata8[2] = 0;
		if(i<tx_n) {

			txdata8[0] = (tx_data[i]>>16) & 0xFF;
			txdata8[1] = (tx_data[i]>>8) & 0xFF;
			txdata8[2] = (tx_data[i]) & 0xFF;
		}
		else {
			txdata8[0] = 0;
			txdata8[1] = 0;
			txdata8[2] = 0;
		}
		//----------------------
		if(_wait_for_txp(spi->spix, tot) < 0) {
			LL_GPIO_SetOutputPin(_spis[spi_id].nss->port, _spis[spi_id].nss->pin);
			return -1;
		}

		LL_SPI_TransmitData8(spi->spix, *(txdata8));
		LL_SPI_TransmitData8(spi->spix, *(txdata8+1));
		LL_SPI_TransmitData8(spi->spix, *(txdata8+2));
		LL_SPI_StartMasterTransfer(spi->spix);



		if(_wait_for_rxp(spi->spix, tot) < 0) {
			LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
			return -1;
		}


		*(rxdata8) = LL_SPI_ReceiveData8(spi->spix);
		*(rxdata8+1) = LL_SPI_ReceiveData8(spi->spix);
		*(rxdata8+2) = LL_SPI_ReceiveData8(spi->spix);

		//----------------------
		if(i>=rx_skip_n) {
			rx_data[i-rx_skip_n] = (rxdata8[0]<<16) | (rxdata8[1]<<8) | (rxdata8[2]);
		}
		else {
			uint32_t waste = (rxdata8[0]<<16) | (rxdata8[1]<<8) | (rxdata8[2]);
			(void) waste;
		}
	}
	if(_wait_for_eot(spi->spix, tot) < 0) {
		LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
		return -1;
	}

	LL_GPIO_SetOutputPin(spi->nss->port, spi->nss->pin);
	LL_SPI_SuspendMasterTransfer(spi->spix);
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
