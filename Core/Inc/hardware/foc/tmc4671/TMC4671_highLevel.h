#ifndef TMC4671_TMC4671_HIGHLEVEL_H_
#define TMC4671_TMC4671_HIGHLEVEL_H_


#include <stdint.h>
#include <stdbool.h>


#define POLE_PAIRS 7


void TMC4671_highLevel_init();

void TMC4671_highLevel_pwmOff();
void TMC4671_highLevel_stoppedMode();
void TMC4671_highLevel_positionMode();
void TMC4671_highLevel_positionMode_fluxTorqueRamp();

void TMC4671_highLevel_setPosition(int32_t position);
void TMC4671_highLevel_setPosition_nonBlocking(int32_t position);

void TMC4671_highLevel_printOffsetAngle();
uint16_t TMC4671_getAdcRaw0();
uint16_t TMC4671_getAdcRaw1();
void TMC4671_highLevel_initEncoder();

void TMC4671_highLevel_initEncoder_new();
void TMC4671_highLevel_openLoopTest2();
void TMC4671_highLevel_positionMode2();
void TMC4671_highLevel_positionMode_rampToZero();

//set control parameters
void TMC4671_highLevel_setPositionFilter(bool status);
void TMC4671_highLevel_setCurrentLimit(uint16_t torque_flux_limit);
void TMC4671_highLevel_setIntegralPosition(uint16_t integral);

char* TMC4671_highLevel_getStatus();

//Get current values
int16_t TMC4671_highLevel_getTorqueTarget();
int16_t TMC4671_highLevel_getTorqueActual();
int16_t TMC4671_highLevel_getVelocityTarget();
int16_t TMC4671_highLevel_getVelocityActual();
int32_t TMC4671_highLevel_getPositionTarget();
int32_t TMC4671_highLevel_getPositionActual();

//Test functions
void TMC4671_highLevel_positionTest();
void TMC4671_highLevel_velocityTest();
void TMC4671_highLevel_torqueTest();
void TMC4671_highLevel_openLoopTest();

void TMC4671_highLevel_referenceEndStop();

#endif /* TMC4671_TMC4671_HIGHLEVEL_H_ */
