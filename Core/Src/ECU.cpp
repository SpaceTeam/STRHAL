#include "../Inc/ECU.h"
#include "../Inc/CANCOM.h"

ECU::ECU(uint32_t node_id, uint32_t fw_version) :
	GenericChannel(node_id, fw_version),
	press_0(0, {ADC2, LID_ADC_CHANNEL_15}),
	press_1(1, {ADC3, LID_ADC_CHANNEL_5}),
	press_2(2, {ADC1, LID_ADC_CHANNEL_14}),
	press_3(3, {ADC3, LID_ADC_CHANNEL_7}),
	press_4(4, {ADC1, LID_ADC_CHANNEL_5}),
	press_5(5, {ADC1, LID_ADC_CHANNEL_11}),
	temp_0(6, {ADC1, LID_ADC_CHANNEL_6}),
	temp_1(7, {ADC1, LID_ADC_CHANNEL_7}),
	temp_2(8, {ADC1, LID_ADC_CHANNEL_8}),
	servo_0(9, LID_TIM_TIM4, LID_TIM_TIM4_CH2_PB7, {ADC1, LID_ADC_CHANNEL_9}, {ADC1, LID_ADC_CHANNEL_1}, {GPIOC, 13, LID_GPIO_TYPE_OPP}),
	servo_1(10, LID_TIM_TIM4, LID_TIM_TIM4_CH3_PB8, {ADC1, LID_ADC_CHANNEL_2}, {ADC1, LID_ADC_CHANNEL_3}, {GPIOC, 14, LID_GPIO_TYPE_OPP}),
	servo_2(11, LID_TIM_TIM4, LID_TIM_TIM4_CH4_PB9, {ADC1, LID_ADC_CHANNEL_4}, {ADC2, LID_ADC_CHANNEL_17}, {GPIOC, 15, LID_GPIO_TYPE_OPP}),
	pyro0_cont(13, {GPIOA, 10, LID_GPIO_TYPE_IHZ}),
	pyro1_cont(15, {GPIOA, 8, LID_GPIO_TYPE_IHZ}),
	pyro2_cont(17, {GPIOC, 8, LID_GPIO_TYPE_IHZ}),
	pyro_igniter0(12, {ADC3, LID_ADC_CHANNEL_2}, {GPIOA, 9, LID_GPIO_TYPE_OPP}, pyro0_cont),
	pyro_igniter1(14, {ADC3, LID_ADC_CHANNEL_6}, {GPIOC, 9, LID_GPIO_TYPE_OPP}, pyro1_cont),
	pyro_igniter2(16, {ADC3, LID_ADC_CHANNEL_4}, {GPIOC, 7, LID_GPIO_TYPE_OPP}, pyro2_cont),
	solenoid_0(18, {ADC2, LID_ADC_CHANNEL_16}, {GPIOD, 9, LID_GPIO_TYPE_OPP}),
	solenoid_1(19, {ADC2, LID_ADC_CHANNEL_18}, {GPIOD, 8, LID_GPIO_TYPE_OPP}),
	pressure_control(20, press_1, solenoid_0)
{
	cancom = CANCOM::instance(this);
	registerChannel(&press_0);
	registerChannel(&press_1);
	registerChannel(&press_2);
	registerChannel(&press_3);
	registerChannel(&press_4);
	registerChannel(&press_5);
	registerChannel(&temp_0);
	registerChannel(&temp_1);
	registerChannel(&temp_2);
	registerChannel(&servo_0);
	registerChannel(&servo_1);
	registerChannel(&servo_2);
	registerChannel(&pyro0_cont);
	registerChannel(&pyro1_cont);
	registerChannel(&pyro2_cont);
	registerChannel(&pyro_igniter0);
	registerChannel(&pyro_igniter1);
	registerChannel(&pyro_igniter2);
	registerChannel(&solenoid_0);
	registerChannel(&solenoid_1);
	registerChannel(&pressure_control);
}

	/*
	0	press_fueltank			pressure		ADCChannel				ADC16
	1	press_oxtank			pressure		ADCChannel				ADC16
	2	press_chamber			pressure		ADCChannel				ADC16
	3	press_fuelinj			pressure		ADCChannel				ADC16
	4	press_oxinj				pressure		ADCChannel				ADC16
	5	UNUSED					pressure		ADCChannel				ADC16
	6	temp_oxbottom			temperature		ADCChannel				ADC16
	7	temp_oxmiddle			temperature		ADCChannel				ADC16
	8	temp_oxtop				temperature		ADCChannel				ADC16
	9	servo_fuelmain			fdbk pos		ServoChannel			Servo
	10	servo_oxmain			fdbk pos		ServoChannel			Servo
	11	USUSDED					fdbk pos		ServoChannel			Servo
	12	pyro_igniter0			current			DigitalOutChannel		DigitalOut
	13	pyro0_cont				continuity		DigitalInChannel		ADC16
	14	pyro_igniter1			current			DigitalOutChannel		DigitalOut
	15	pyro1_cont				continuity		DigitalInChannel		ADC16
	16	USUSED					current			DigitalOutChannel		DigitalOut
	17	UNUSED					continuity		DigitalInChannel		ADC16
	18	solenoid_supercharge	current			DigitalOutChannel		DigitalOut
	19	UNUSED					current			DigitalOutChannel		DigitalOut
	20	pressure_control		solenoid state	PressureControlChannel	PneumaticValve
*/

int ECU::init() {
	if(LID_Init(LID_SYSCLK_SRC_EXT, 8000000) != LID_NOICE)
		return -1;

	LID_GPIO_t led1 = {GPIOD, 1, LID_GPIO_TYPE_OPP};
	LID_GPIO_SingleInit(&led1, LID_GPIO_TYPE_OPP);
	LID_GPIO_Write(&led1, LID_GPIO_VALUE_H);

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
	CANState = cancom->exec();
	if(CANState != COMState::RUN)
		return -1;

	LID_UART_Write("RUNNING\n",8);
	while(1) {
		if(GenericChannel::exec() != 0)
			return -1;
	}
	return 0;
}
