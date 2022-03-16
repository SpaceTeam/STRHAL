#ifndef LID_SPI_H
#define LID_SIP_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
	LID_SPI_SPI1 = 0,
	LID_SPI_SPI2,
	LID_SPI_SPI3,

	LID_SPI_N_SPI
} LID_SPI_Id_t;

typedef enum {
	LID_SPI_SPI1_SCK_PA5,
	LID_SPI_SPI1_SCK_PB3,

	LID_SPI_SPI2_SCK_PB13,
	LID_SPI_SPI2_SCK_PF1,
	LID_SPI_SPI2_SCK_PF9,
	LID_SPI_SPI2_SCK_PF10,

	LID_SPI_SPI3_SCK_PB3,
	LID_SPI_SPI3_SCK_PC10,

	LID_SPI_N_SCK,
} LID_SPI_SCKId_t;

typedef enum {
	LID_SPI_SPI1_MISO_PA6,
	LID_SPI_SPI1_MISO_PB4,

	LID_SPI_SPI2_MISO_PA10,
	LID_SPI_SPI2_MISO_PB14,

	LID_SPI_SPI3_MISO_PB4,
	LID_SPI_SPI3_MISO_PC11,

	LID_SPI_N_MISO,
} LID_SPI_MISOId_t;

typedef enum {
	LID_SPI_SPI1_MOSI_PA7,
	LID_SPI_SPI1_MOSI_PB5,

	LID_SPI_SPI2_MOSI_PA11,
	LID_SPI_SPI2_MOSI_PB15,

	LID_SPI_SPI3_MOSI_PB5,
	LID_SPI_SPI3_MOSI_PC12,

	LID_SPI_N_MOSI,
} LID_SPI_MOSIId_t;


typedef enum {
	LID_SPI_SPI1_NSS_PA4,
	LID_SPI_SPI1_NSS_PA15,

	LID_SPI_SPI2_NSS_PB12,
	LID_SPI_SPI2_NSS_PD15,
	LID_SPI_SPI2_NSS_PF0,

	LID_SPI_SPI3_NSS_PA4,
	LID_SPI_SPI3_NSS_PA15,

	LID_SPI_N_NSS,
} LID_SPI_NSSId_t;


typedef enum {
	LID_SPI_MODE_MASTER,
	LID_SPI_MODE_SLAVE,
} LID_SPI_Mode_t;

typedef enum {
	LID_SPI_CPOL_CPHASE_LL = 0<<0,
	LID_SPI_CPOL_CPHASE_LH = 1<<0,
	LID_SPI_CPOL_CPHASE_HL = 1<<1,
	LID_SPI_CPOL_CPHASE_HH = 1<<1 | 1<<0,
} LID_SPI_CPOL_CPHASE_t;

typedef struct {
	LID_SPI_SCKId_t sck;
	LID_SPI_MISOId_t miso;
	LID_SPI_MOSIId_t mosi;
	LID_SPI_NSSId_t nss;

	LID_SPI_Mode_t mode;
	LID_SPI_CPOL_CPHASE_t c_pol_phase : 2;

	uint8_t psc : 3;
	uint8_t lsb_first : 1;
} LID_SPI_Config_t;

void LID_SPI_Init();

int32_t LID_SPI_Master_Init(LID_SPI_Id_t spi_id, const LID_SPI_Config_t *config);

void LID_SPI_Master_Run(LID_SPI_Id_t spi_id);
void LID_SPI_Master_Stop(LID_SPI_Id_t spi_id);

int32_t LID_SPI_Master_Transceive(LID_SPI_Id_t spi_id, const uint8_t *tx_data, uint32_t tx_n, uint32_t rx_skip_n, uint8_t *rx_data, uint32_t rx_n, uint16_t tot);
#ifdef __cplusplus
}
#endif

#endif /*LID_SPI_H*/
