#include "../Inc/PMU.h"
#include "../Inc/Can.h"

#include <cstdio>
#include <cstring>

PMU::PMU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider) :
	GenericChannel(node_id, fw_version, refresh_divider),
	ledRed({GPIOD, 1, STRHAL_GPIO_TYPE_OPP}),
	ledGreen({GPIOD, 2, STRHAL_GPIO_TYPE_OPP}),

	sense_5V(0, {ADC2, STRHAL_ADC_CHANNEL_5}, 1),
	sense_5VP(1, {ADC1, STRHAL_ADC_CHANNEL_9}, 1),
	sense_12V(2, {ADC2, STRHAL_ADC_CHANNEL_11}, 1),
	umbilical(3, {GPIOA, 8, STRHAL_GPIO_TYPE_IHZ}, 1),
	charging(4, {GPIOA, 9, STRHAL_GPIO_TYPE_IHZ}, 1),
	out0(5, {GPIOC, 6, STRHAL_GPIO_TYPE_OPP}, 1),
	out1(6, {GPIOC, 7, STRHAL_GPIO_TYPE_OPP}, 1),
	out2(7, {GPIOC, 8, STRHAL_GPIO_TYPE_OPP}, 1),
	out3(8, {GPIOC, 9, STRHAL_GPIO_TYPE_OPP}, 1),
	payload(9, {GPIOC, 1, STRHAL_GPIO_TYPE_OPP}, 1),
	barometer(10, STRHAL_SPI_SPI1, {STRHAL_SPI_SPI1_SCK_PA5, STRHAL_SPI_SPI1_MISO_PA6, STRHAL_SPI_SPI1_MOSI_PA7, STRHAL_SPI_SPI1_NSS_PA4, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0}, 1),
	imu_0(11, STRHAL_SPI_SPI3, {STRHAL_SPI_SPI3_SCK_PC10, STRHAL_SPI_SPI3_MISO_PC11, STRHAL_SPI_SPI3_MOSI_PC12, STRHAL_SPI_SPI3_NSS_PA15, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0}, 1),
	x_accel(12, IMUMeasurement::X_ACCEL, imu_0, 1),
	y_accel(13, IMUMeasurement::Y_ACCEL, imu_0, 1),
	z_accel(14, IMUMeasurement::Z_ACCEL, imu_0, 1),
	x_gyro(15, IMUMeasurement::X_GYRO, imu_0, 1),
	y_gyro(16, IMUMeasurement::Y_GYRO, imu_0, 1),
	z_gyro(17, IMUMeasurement::Z_GYRO, imu_0, 1),
	speaker(STRHAL_TIM_TIM2, STRHAL_TIM_TIM2_CH3_PB10)
{
	cancom = Can::instance(this);
	flash = W25Qxx_Flash::instance(0x1F);
	registerChannel(&sense_5V);
	registerChannel(&sense_5VP);
	registerChannel(&sense_12V);
	registerChannel(&umbilical);
	registerChannel(&charging);
	registerChannel(&out0);
	registerChannel(&out1);
	registerChannel(&out2);
	registerChannel(&out3);
	registerChannel(&payload);
	registerChannel(&barometer);
	registerChannel(&imu_0);
	registerChannel(&x_accel);
	registerChannel(&y_accel);
	registerChannel(&z_accel);
	registerChannel(&x_gyro);
	registerChannel(&y_gyro);
	registerChannel(&z_gyro);

}

int PMU::init() {
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

int PMU::exec() {
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

void PMU::testChannels() {
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
					std::sprintf(write,"ChannelId: %d, ChannelType: %d, Measurement: %d\n",channel->getChannelId(),type,adc->getMeasurement());
					STRHAL_UART_Write(write, strlen(write));
					STRHAL_Systick_BusyWait(500);
				}
			} else if(type == CHANNEL_TYPE_DIGITAL_OUT) {
				SetMsg_t set_msg =
				{ 0 };
				set_msg.variable_id = DIGITAL_OUT_STATE;
				set_msg.value = 1;
				uint8_t ret_n = 0;
				std::sprintf(write,"ChannelId: %d, ChannelType: %d\n",state,type);
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
