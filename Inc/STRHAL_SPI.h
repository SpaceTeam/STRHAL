#ifndef STRHAL_SPI_H
#define STRHAL_SIP_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
	STRHAL_SPI_SPI1 = 0,
	STRHAL_SPI_SPI2,
	STRHAL_SPI_SPI3,

	STRHAL_SPI_N_SPI
} STRHAL_SPI_Id_t;

typedef enum {
	STRHAL_SPI_SPI1_SCK_PA5,
	STRHAL_SPI_SPI1_SCK_PB3,

	STRHAL_SPI_SPI2_SCK_PB13,
	STRHAL_SPI_SPI2_SCK_PF1,
	STRHAL_SPI_SPI2_SCK_PF9,
	STRHAL_SPI_SPI2_SCK_PF10,

	STRHAL_SPI_SPI3_SCK_PB3,
	STRHAL_SPI_SPI3_SCK_PC10,

	STRHAL_SPI_N_SCK,
} STRHAL_SPI_SCKId_t;

typedef enum {
	STRHAL_SPI_SPI1_MISO_PA6,
	STRHAL_SPI_SPI1_MISO_PB4,

	STRHAL_SPI_SPI2_MISO_PA10,
	STRHAL_SPI_SPI2_MISO_PB14,

	STRHAL_SPI_SPI3_MISO_PB4,
	STRHAL_SPI_SPI3_MISO_PC11,

	STRHAL_SPI_N_MISO,
} STRHAL_SPI_MISOId_t;

typedef enum {
	STRHAL_SPI_SPI1_MOSI_PA7,
	STRHAL_SPI_SPI1_MOSI_PB5,

	STRHAL_SPI_SPI2_MOSI_PA11,
	STRHAL_SPI_SPI2_MOSI_PB15,

	STRHAL_SPI_SPI3_MOSI_PB5,
	STRHAL_SPI_SPI3_MOSI_PC12,

	STRHAL_SPI_N_MOSI,
} STRHAL_SPI_MOSIId_t;


typedef enum {
	STRHAL_SPI_SPI1_NSS_PA4,
	STRHAL_SPI_SPI1_NSS_PA15,

	STRHAL_SPI_SPI2_NSS_PB12,
	STRHAL_SPI_SPI2_NSS_PD15,
	STRHAL_SPI_SPI2_NSS_PF0,

	STRHAL_SPI_SPI3_NSS_PA4,
	STRHAL_SPI_SPI3_NSS_PA15,

	STRHAL_SPI_N_NSS,
} STRHAL_SPI_NSSId_t;


typedef enum {
	STRHAL_SPI_MODE_MASTER,
	STRHAL_SPI_MODE_SLAVE,
} STRHAL_SPI_Mode_t;

typedef enum {
	STRHAL_SPI_CPOL_CPHASE_LL = 0<<0,
	STRHAL_SPI_CPOL_CPHASE_LH = 1<<0,
	STRHAL_SPI_CPOL_CPHASE_HL = 1<<1,
	STRHAL_SPI_CPOL_CPHASE_HH = 1<<1 | 1<<0,
} STRHAL_SPI_CPOL_CPHASE_t;

typedef struct {
	STRHAL_SPI_SCKId_t sck;
	STRHAL_SPI_MISOId_t miso;
	STRHAL_SPI_MOSIId_t mosi;
	STRHAL_SPI_NSSId_t nss;

	STRHAL_SPI_Mode_t mode;
	STRHAL_SPI_CPOL_CPHASE_t c_pol_phase : 2;

	uint8_t psc : 3;
	uint8_t lsb_first : 1;
} STRHAL_SPI_Config_t;

void STRHAL_SPI_Init();

int32_t STRHAL_SPI_Master_Init(STRHAL_SPI_Id_t spi_id, const STRHAL_SPI_Config_t *config);

void STRHAL_SPI_Master_Run(STRHAL_SPI_Id_t spi_id);
void STRHAL_SPI_Master_Stop(STRHAL_SPI_Id_t spi_id);

int32_t STRHAL_SPI_Master_Transceive(STRHAL_SPI_Id_t spi_id, const uint8_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, uint8_t *rx_data, uint32_t rx_n, uint16_t tot);
#ifdef __cplusplus
}
#endif

#endif /*STRHAL_SPI_H*/
