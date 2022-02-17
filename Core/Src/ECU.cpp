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
	GenericChannel(node_id, fw_version)	{
	channels[n_sens++] = new PressureChannel(0, {.ADCx = ADC2, .channelId = LID_ADC_CHANNEL_15});
	channels[n_sens++] = new PressureChannel(1, {.ADCx = ADC3, .channelId = LID_ADC_CHANNEL_4});
	channels[n_sens++] = new PressureChannel(2, {.ADCx = ADC1, .channelId = LID_ADC_CHANNEL_14});
	channels[n_sens++] = new PressureChannel(3, {.ADCx = ADC3, .channelId = LID_ADC_CHANNEL_7});
	channels[n_sens++] = new PressureChannel(4, {.ADCx = ADC1, .channelId = LID_ADC_CHANNEL_5});
	channels[n_sens++] = new TempChannel(6, {.ADCx = ADC1, .channelId = LID_ADC_CHANNEL_6});
	channels[n_sens++] = new TempChannel(7, {.ADCx = ADC1, .channelId = LID_ADC_CHANNEL_7});
	channels[n_sens++] = new TempChannel(8, {.ADCx = ADC1, .channelId = LID_ADC_CHANNEL_8});
}

int ECU::init() {
	if(GenericChannel::init() != 0)
		return -1;

	return 0;
}

int ECU::exec() {
	if(GenericChannel::exec() != 0)
		return -1;

	while(1) {

	}
	return 0;
}
