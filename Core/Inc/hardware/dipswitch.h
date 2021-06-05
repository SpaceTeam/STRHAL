/*
 * dipswitch.h
 *
 *  Created on: Jun 5, 2021
 *      Author: andreas
 */

#ifndef INC_HARDWARE_DIPSWITCH_H_
#define INC_HARDWARE_DIPSWITCH_H_

#include "main.h"

__STATIC_INLINE uint32_t Dipswitch_Read()
{
  return (uint32_t)(READ_REG(GPIOD->IDR));
}

__STATIC_INLINE uint32_t NodeID_Get()
{
  return Dipswitch_Read() & 0x1F;
}

#endif /* INC_HARDWARE_DIPSWITCH_H_ */
