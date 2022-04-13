#include "../Inc/ECU.h"
#include "../Inc/CANCOM.h"

#include <cstdio>
#include <cstring>

ECU::ECU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider) :
	GenericChannel(node_id, fw_version, refresh_divider),
	ledRed({GPIOD, 1, STRHAL_GPIO_TYPE_OPP}),
	ledGreen({GPIOD, 2, STRHAL_GPIO_TYPE_OPP}),
	press_0(0, {ADC2, STRHAL_ADC_CHANNEL_15}, 1),
	press_1(1, {ADC3, STRHAL_ADC_CHANNEL_5}, 1),
	press_2(2, {ADC1, STRHAL_ADC_CHANNEL_14}, 1),
	press_3(3, {ADC3, STRHAL_ADC_CHANNEL_7}, 1),
	press_4(4, {ADC1, STRHAL_ADC_CHANNEL_5}, 1),
	press_5(5, {ADC1, STRHAL_ADC_CHANNEL_11}, 1),
	temp_0(6, {ADC1, STRHAL_ADC_CHANNEL_6}, 1),
	temp_1(7, {ADC1, STRHAL_ADC_CHANNEL_7}, 1),
	temp_2(8, {ADC1, STRHAL_ADC_CHANNEL_8}, 1),
	servo_0(9, 0, STRHAL_TIM_TIM4, STRHAL_TIM_TIM4_CH2_PB7, {ADC1, STRHAL_ADC_CHANNEL_9}, {ADC1, STRHAL_ADC_CHANNEL_1}, {GPIOC, 13, STRHAL_GPIO_TYPE_OPP}, 1),
	servo_1(10, 1, STRHAL_TIM_TIM4, STRHAL_TIM_TIM4_CH3_PB8, {ADC1, STRHAL_ADC_CHANNEL_2}, {ADC1, STRHAL_ADC_CHANNEL_3}, {GPIOC, 14, STRHAL_GPIO_TYPE_OPP}, 1),
	servo_2(11, 2, STRHAL_TIM_TIM4, STRHAL_TIM_TIM4_CH4_PB9, {ADC1, STRHAL_ADC_CHANNEL_4}, {ADC2, STRHAL_ADC_CHANNEL_17}, {GPIOC, 15, STRHAL_GPIO_TYPE_OPP}, 1),
	pyro0_cont(13, {GPIOA, 10, STRHAL_GPIO_TYPE_IHZ}, 1),
	pyro1_cont(15, {GPIOA, 8, STRHAL_GPIO_TYPE_IHZ}, 1),
	pyro2_cont(17, {GPIOC, 8, STRHAL_GPIO_TYPE_IHZ}, 1),
	pyro_igniter0(12, {ADC3, STRHAL_ADC_CHANNEL_2}, {GPIOA, 9, STRHAL_GPIO_TYPE_OPP}, pyro0_cont, 1),
	pyro_igniter1(14, {ADC3, STRHAL_ADC_CHANNEL_6}, {GPIOC, 9, STRHAL_GPIO_TYPE_OPP}, pyro1_cont, 1),
	pyro_igniter2(16, {ADC3, STRHAL_ADC_CHANNEL_4}, {GPIOC, 7, STRHAL_GPIO_TYPE_OPP}, pyro2_cont, 1),
	solenoid_0(18, {ADC2, STRHAL_ADC_CHANNEL_16}, {GPIOD, 9, STRHAL_GPIO_TYPE_OPP}, 1),
	solenoid_1(19, {ADC2, STRHAL_ADC_CHANNEL_18}, {GPIOD, 8, STRHAL_GPIO_TYPE_OPP}, 1),
	pressure_control(20, press_1, solenoid_1, 1),
	imu_0(21, STRHAL_SPI_SPI3, {STRHAL_SPI_SPI3_SCK_PC10, STRHAL_SPI_SPI3_MISO_PC11, STRHAL_SPI_SPI3_MOSI_PC12, STRHAL_SPI_SPI3_NSS_PA15, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0}, 1),
	speaker(STRHAL_TIM_TIM2, STRHAL_TIM_TIM2_CH3_PB10)
{
	cancom = CANCOM::instance(this);
	flash = W25Qxx_Flash::instance(0x1F);
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
	//registerChannel(&imu_0);
}

int ECU::init() {
	if(STRHAL_Init(STRHAL_SYSCLK_SRC_EXT, 8000000) != STRHAL_NOICE)
		return -1;

	// init status LEDs
	STRHAL_GPIO_SingleInit(&ledRed, STRHAL_GPIO_TYPE_OPP);
	STRHAL_GPIO_SingleInit(&ledGreen, STRHAL_GPIO_TYPE_OPP);

	if(flash == nullptr)
		return -1;

	if(flash->init() != 0)
		return -1;

	if(cancom == nullptr)
		return -1;

	CANState = cancom->init();
	if(CANState != COMState::SBY)
		return -1;


	if(GenericChannel::init() != 0)
		return -1;

	speaker.init();

	STRHAL_GPIO_Write(&ledGreen, STRHAL_GPIO_VALUE_H);
	return 0;
}

int ECU::exec() {
	STRHAL_OPAMP_Run();
	STRHAL_ADC_Run();
	STRHAL_QSPI_Run();

	CANState = cancom->exec();
	if(CANState != COMState::RUN)
		return -1;

	STRHAL_GPIO_Write(&ledRed, STRHAL_GPIO_VALUE_H);
	STRHAL_UART_Write("RUNNING\n",8);

	speaker.beep(2, 400, 300);

	while(1) {
		if(GenericChannel::exec() != 0)
			return -1;
	}

	speaker.beep(6, 100, 100);

	return 0;
}

void ECU::testServo(ServoChannel &servo) {
	servo.setTargetPos(0);
	servo.getPos();

	char buf[64];
	uint64_t t_last_sample = 0;
	uint8_t state = 0;
	while(1) {
		uint64_t t = STRHAL_Systick_GetTick();
		if((t - t_last_sample) > 3000) {
			t_last_sample = t;
			if(state == 0) {
				servo.setTargetPos(63000);
				state = 1;
			} else {
				servo.setTargetPos(0);
				state = 0;
			}

		}
		sprintf(buf,"%d, %d, %d\n",servo.getCurrentMeasurement(),servo.getFeedbackMeasurement(), servo.getPos());
		STRHAL_UART_Write(buf, strlen(buf));
		if(GenericChannel::exec() != 0)
			return;
	}
}

void ECU::testChannels() {
	char read[256], write[256];
	uint8_t state = 0;
	STRHAL_UART_Listen();
	while(1) {
		int32_t n = STRHAL_UART_Read(read, 2);
		if(n > 0) {
			AbstractChannel *channel = GenericChannel::channels[state];
			CHANNEL_TYPE type = channel->getChannelType();
			if(type == CHANNEL_TYPE_ADC16) {
				ADCChannel * adc = (ADCChannel *) channel;
				int nn = 0;
				while(nn == 0) {
					nn = STRHAL_UART_Read(read, 2);
					std::sprintf(write,"ChanneSTRHAL: %d, ChannelType: %d, Measurement: %d\n",channel->getChanneSTRHAL(),type,adc->getMeasurement());
					STRHAL_UART_Write(write, strlen(write));
					STRHAL_Systick_BusyWait(500);
				}
			} else if(type == CHANNEL_TYPE_DIGITAL_OUT) {
				SetMsg_t set_msg =
				{ 0 };
				set_msg.variable_id = DIGITAL_OUT_STATE;
				set_msg.value = 1;
				uint8_t ret_n = 0;
				std::sprintf(write,"ChanneSTRHAL: %d, ChannelType: %d\n",state,type);
				STRHAL_UART_Write(write, strlen(write));
				STRHAL_Systick_BusyWait(1000);
				STRHAL_UART_Write("..Setting Output for 10s in\n", 28);
				STRHAL_Systick_BusyWait(500);
				STRHAL_UART_Write("..3s\n", 5);
				STRHAL_Systick_BusyWait(1000);
				STRHAL_UART_Write("..2s\n", 5);
				STRHAL_Systick_BusyWait(1000);
				STRHAL_UART_Write("..1s\n", 5);
				STRHAL_Systick_BusyWait(1000);
				channel->processMessage(COMMON_REQ_SET_VARIABLE, (uint8_t *) &set_msg, ret_n);
				for(int i = 0; i < 5; i++) {
					uint8_t n = 0;
					uint8_t meas[2];
					channel->getSensorData(meas, n);
					std::sprintf(write,"...Output ON, Measurement: %d\n",meas[0] << 8 | meas[1]);
					STRHAL_UART_Write(write, strlen(write));
					STRHAL_Systick_BusyWait(2000);
				}
				set_msg.variable_id = DIGITAL_OUT_STATE;
				set_msg.value = 0;
				channel->processMessage(COMMON_REQ_SET_VARIABLE, (uint8_t *) &set_msg, ret_n);
				STRHAL_UART_Write("..Output OFF\n", 13);
			} else if(type == CHANNEL_TYPE_PNEUMATIC_VALVE) {
				std::sprintf(write,"Channel %d/type: %d not implemented\n",state,type);
				STRHAL_UART_Write(write, strlen(write));
				STRHAL_UART_Write("Channel not implemented\n", 24);
			} else if(type == CHANNEL_TYPE_SERVO) {
				std::sprintf(write,"Channel %d/type: %d not implemented\n",state,type);
				STRHAL_UART_Write(write, strlen(write));
			} else {
				std::sprintf(write,"Channel %d/type: %d not implemented\n",state,type);
				STRHAL_UART_Write(write, strlen(write));
			}
			state = (state == 20) ? 0 : (state+1);
		}
		STRHAL_Systick_BusyWait(500);
	}
}
