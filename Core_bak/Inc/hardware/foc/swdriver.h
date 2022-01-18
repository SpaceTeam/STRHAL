#ifndef SWDRIVER_H_
#define SWDRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include "spi.h"

#define OFFSET_POS0			45640

#define OFFSET_I0 				38907 	// FIXME offsets are changing!?
#define OFFSET_I1 				36184
#define OFFSET_ENC_PHIM 	0 			// negative encoder zero mechanical angle (signed 16bit)
#define OFFSET_ENC_PHIE 	0 			// negative encoder zero electrical angle (signed 16bit)

#define OFFSET_PHIM_PHIE ((int16_t)((-7)*((int32_t)OFFSET_ENC_PHIE - (int32_t)OFFSET_ENC_PHIM)))

typedef struct
{
	GPIO_TypeDef* port;
	uint16_t      pin;
}encoder_t;

typedef struct swdriver_s
{
	GPIO_Pin_t enable;
	GPIO_Pin_t status;
	GPIO_Pin_t fault;
	GPIO_Pin_t cs_controller;
	GPIO_Pin_t cs_fet_driver;
	GPIO_Pin_t cs_encoder[4];
	SPI_TypeDef * spi;
	uint16_t ofs_i0;
	uint16_t ofs_i1;
	int16_t ofs_enc_phim;
	int16_t ofs_phim_phie;
	uint16_t ofs_pos0;
} swdriver_t;


extern swdriver_t swdriver;


//TODO: make those inline, but correctly
void swdriver_setEnable(bool enabled);
void swdriver_setCsnController(bool enabled);
void swdriver_setCsnDriver(bool enabled);
void swdriver_setCsnEncoder(uint8_t enc_index, bool enabled);
bool swdriver_getStatus(void);
bool swdriver_getFault(void);

#endif /* SWDRIVER_H_ */
