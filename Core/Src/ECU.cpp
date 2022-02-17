#include "../Inc/ECU.h"
#include "../Inc/CANCOM.h"

/*
0	press_fueltank
1	press_oxtank
2	press_chamber
3	press_fuelinj
4	press_oxinj
6	temp_oxbottom
7	temp_oxmiddle
8	temp_oxtop
9	servo_fuelmain
11	servo_oxmain
12	pyro_igniter0
13	pyro_igniter1
15	solenoid_supercharge
 */

ECU::ECU(uint32_t node_id, uint32_t fw_version) :
	GenericChannel(node_id, fw_version),
	press0Ch{0, {ADC2, LID_ADC_CHANNEL_15}},
	press1Ch{1, {ADC3, LID_ADC_CHANNEL_5}},
	press2Ch{2, {ADC1, LID_ADC_CHANNEL_14}},
	press3Ch{3, {ADC3, LID_ADC_CHANNEL_7}},
	press4Ch{4, {ADC1, LID_ADC_CHANNEL_5}},
	press5Ch{5, {ADC1, LID_ADC_CHANNEL_11}},
	temp0Ch{6, {ADC1, LID_ADC_CHANNEL_6}},
	temp1Ch{7, {ADC1, LID_ADC_CHANNEL_7}},
	temp2Ch{8, {ADC1, LID_ADC_CHANNEL_8}}
{
	cancom = CANCOM::instance(this);
	registerChannel(&press0Ch);
	registerChannel(&press1Ch);
	registerChannel(&press2Ch);
	registerChannel(&press3Ch);
	registerChannel(&press4Ch);
	registerChannel(&temp0Ch);
	registerChannel(&temp1Ch);
	registerChannel(&temp2Ch);
}

int ECU::init() {
	if(LID_Init(LID_SYSCLK_SRC_EXT, 8000000) != LID_NOICE)
		return -1;

	if(cancom == nullptr)
		return -1;

	CANState = cancom->init();
	if(CANState != COMState::SBY)
		return -1;

	if(GenericChannel::init() != 0)
		return -1;

	return 0;
}

int ECU::exec() {
	while(1) {
		cancom->exec();
		//if(GenericChannel::exec() != 0)
			//return -1;
	}
	return 0;
}
