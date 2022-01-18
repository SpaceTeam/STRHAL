#include "foc/swdriver.h"
#include "main.h"


//@formatter:off
swdriver_t swdriver =
{
	.enable 		= { EN_GPIO_Port, EN_Pin },
	.status 		= { STATUS_GPIO_Port, STATUS_Pin },
	.fault 			= { FAULT_GPIO_Port, FAULT_Pin },
	.cs_controller 	= { CSN_CTR_GPIO_Port, CSN_CTR_Pin },
	.cs_fet_driver 	= { CSN_DRV_GPIO_Port, CSN_DRV_Pin },
	.cs_encoder 	= {
				{ CSN_ENC0_GPIO_Port, CSN_ENC0_Pin },
				{ CSN_ENC1_GPIO_Port, CSN_ENC1_Pin },
				{ CSN_ENC2_GPIO_Port, CSN_ENC2_Pin },
				{ CSN_ENC3_GPIO_Port, CSN_ENC3_Pin }
			  },
	.spi = SPI1,
	.ofs_i0 = OFFSET_I0,
	.ofs_i1 = OFFSET_I1,
	.ofs_enc_phim = OFFSET_ENC_PHIM,
	.ofs_phim_phie = OFFSET_PHIM_PHIE,
	.ofs_pos0 = OFFSET_POS0
};
//@formatter:on

void swdriver_setEnable(bool enabled)
{
	GPIO_WriteOutput(swdriver.enable.port, swdriver.enable.pin, enabled);
}

void swdriver_setCsnController(bool enabled)
{
	GPIO_WriteOutput(swdriver.cs_controller.port, swdriver.cs_controller.pin, enabled);
}

void swdriver_setCsnDriver(bool enabled)
{
	GPIO_WriteOutput(swdriver.cs_fet_driver.port, swdriver.cs_fet_driver.pin, enabled);
}

void swdriver_setCsnEncoder(uint8_t enc_index, bool enabled)
{
	GPIO_WriteOutput(swdriver.cs_encoder[enc_index].port, swdriver.cs_encoder[enc_index].pin, enabled);
}

bool swdriver_getStatus(void)
{
	return (LL_GPIO_ReadOutputPort(swdriver.status.port) & swdriver.status.pin) != 0;
}

bool swdriver_getFault(void)
{
	return (LL_GPIO_ReadOutputPort(swdriver.fault.port) & swdriver.fault.pin) != 0;
}
