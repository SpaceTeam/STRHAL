#ifndef STRHAL_OPAMP_H
#define STRHAL_OPAMP_H

#include <stm32g4xx.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_opamp.h>
#include <stm32g4xx_ll_rcc.h>


#ifdef __cplusplus
extern "C" {
#endif

/***** DEFINES *****/



void STRHAL_OPAMP_Init();

void STRHAL_OPAMP_Run();


#ifdef __cplusplus
}
#endif

#endif
