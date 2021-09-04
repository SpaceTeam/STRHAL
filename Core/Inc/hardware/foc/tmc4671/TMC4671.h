/*
 * TMC4671.h
 *
 *  Created on: 30.09.2016
 *      Author: ed
 */

#ifndef TMC_IC_TMC4671_H_
#define TMC_IC_TMC4671_H_

#include "foc/tmc_helpers/API_Header.h"
#include "foc/tmc4671/TMC4671_Register.h"
#include "foc/tmc4671/TMC4671_Constants.h"
#include "foc/tmc4671/TMC4671_Fields.h"

// spi access
#define BIT_0_TO_15   0
#define BIT_16_TO_31  1

// Helper macros
#define TMC4671_FIELD_READ(address, mask, shift) \
	FIELD_GET(tmc4671_readInt(address), mask, shift)
#define TMC4671_FIELD_UPDATE(address, mask, shift, value) \
	(tmc4671_writeInt(address, FIELD_SET(tmc4671_readInt(address), mask, shift, value)))

uint32_t tmc4671_readInt(uint8_t address);
void tmc4671_writeInt(uint8_t address, int32_t value);
//void tmc4671_writeInt_nonBlocking(uint8_t address, int32_t value);
uint16_t tmc4671_readRegister16BitValue(uint8_t address, uint8_t channel);
void tmc4671_writeRegister16BitValue(uint8_t address, uint8_t channel, uint16_t value);

// do cyclic tasks
void tmc4671_periodicJob(uint32_t actualSystick, uint8_t initMode, uint8_t *initState, uint16_t initWaitTime, uint16_t *actualInitWaitTime, uint16_t startVoltage);

// initialization functions
void tmc4671_startEncoderInitialization(uint8_t mode, uint8_t *initMode, uint8_t *initState);
void tmc4671_updatePhiSelectionAndInitialize(uint8_t actualPhiESelection, uint8_t desiredPhiESelection, uint8_t initMode, uint8_t *initState);

// === modes of operation ===
void tmc4671_switchToMotionMode(uint8_t mode);

// torque mode
void tmc4671_setTargetTorque_raw(int32_t targetTorque);
int32_t tmc4671_getTargetTorque_raw(void);
int32_t tmc4671_getActualTorque_raw(void);
int32_t tmc4671_getActualRampTorque_raw(void);

void tmc4671_setTargetTorque_mA(uint16_t torqueMeasurementFactor, int32_t targetTorque);
int32_t tmc4671_getTargetTorque_mA(uint16_t torqueMeasurementFactor);
int32_t tmc4671_getActualTorque_mA(uint16_t torqueMeasurementFactor);
int32_t tmc4671_getTargetTorqueFluxSum_mA(uint16_t torqueMeasurementFactor);
int32_t tmc4671_getActualTorqueFluxSum_mA(uint16_t torqueMeasurementFactor);
int32_t tmc4671_getActualRampTorque_mA(uint16_t torqueMeasurementFactor);

// flux
void tmc4671_setTargetFlux_raw(int32_t targetFlux);
int32_t tmc4671_getTargetFlux_raw(void);
int32_t tmc4671_getActualFlux_raw(void);

void tmc4671_setTargetFlux_mA(uint16_t torqueMeasurementFactor, int32_t targetFlux);
int32_t tmc4671_getTargetFlux_mA(uint16_t torqueMeasurementFactor);
int32_t tmc4671_getActualFlux_mA(uint16_t torqueMeasurementFactor);

// torque/flux limit
void tmc4671_setTorqueFluxLimit_mA(uint16_t torqueMeasurementFactor, int32_t max);
int32_t tmc4671_getTorqueFluxLimit_mA(uint16_t torqueMeasurementFactor);

// velocity mode
void tmc4671_setTargetVelocity(int32_t targetVelocity);
int32_t tmc4671_getTargetVelocity(void);
int32_t tmc4671_getActualVelocity(void);
int32_t tmc4671_getActualRampVelocity(void);

// position mode
void tmc4671_setAbsolutTargetPosition(int32_t targetPosition);
void tmc4671_setRelativeTargetPosition(int32_t relativePosition);
int32_t tmc4671_getTargetPosition(void);
void tmc4671_setActualPosition(int32_t actualPosition);
int32_t tmc4671_getActualPosition(void);
int32_t tmc4671_getActualRampPosition(void);

// pwm control
void tmc4671_disablePWM(void);

// motor settings
uint8_t tmc4671_getMotorType(void);
void tmc4671_setMotorType(uint8_t motorType);

uint8_t tmc4671_getPolePairs(void);
void tmc4671_setPolePairs(uint8_t polePairs);

uint16_t tmc4671_getAdcI0Offset(void);
void tmc4671_setAdcI0Offset(uint16_t offset);

uint16_t tmc4671_getAdcI1Offset(void);
void tmc4671_setAdcI1Offset(uint16_t offset);

void tmc4671_setTorqueFluxPI(uint16_t pParameter, uint16_t iParameter);
void tmc4671_setVelocityPI(uint16_t pParameter, uint16_t iParameter);
void tmc4671_setPositionPI(uint16_t pParameter, uint16_t iParameter);

int32_t tmc4671_readFieldWithDependency(uint8_t reg, uint8_t dependsReg, uint32_t dependsValue, uint32_t mask, uint8_t shift);

#endif /* TMC_IC_TMC4671_H_ */
