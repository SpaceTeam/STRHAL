#include "../Inc/RCU.h"
#include "../Inc/Can.h"

#include <cstdio>
#include <cstring>

RCU::RCU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider) :
	GenericChannel(node_id, fw_version, refresh_divider),
	ledRed({GPIOD, 1, STRHAL_GPIO_TYPE_OPP}),
	ledGreen({GPIOD, 2, STRHAL_GPIO_TYPE_OPP}),
	baro(STRHAL_SPI_SPI1, {STRHAL_SPI_SPI1_SCK_PA5, STRHAL_SPI_SPI1_MISO_PA6, STRHAL_SPI_SPI1_MOSI_PA7, STRHAL_SPI_SPI1_NSS_PA4, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0},{GPIOA, 3, STRHAL_GPIO_TYPE_IHZ}),
	imu(STRHAL_SPI_SPI3, {STRHAL_SPI_SPI3_SCK_PC10, STRHAL_SPI_SPI3_MISO_PC11, STRHAL_SPI_SPI3_MOSI_PC12, STRHAL_SPI_SPI3_NSS_PA15, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0},{GPIOD, 0, STRHAL_GPIO_TYPE_IHZ}),
	lora(STRHAL_SPI_SPI2, {STRHAL_SPI_SPI2_SCK_PB13, STRHAL_SPI_SPI2_MISO_PB14, STRHAL_SPI_SPI2_MOSI_PB15, STRHAL_SPI_SPI2_NSS_PB12, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_LL, 0x5, 0},{GPIOC, 1, STRHAL_GPIO_TYPE_IHZ},{GPIOC, 3, STRHAL_GPIO_TYPE_IHZ},{GPIOB, 11, STRHAL_GPIO_TYPE_IHZ}),
	gnss(STRHAL_UART1),
	sense_5V(0, {ADC2, STRHAL_ADC_CHANNEL_5}, 1),
	sense_12V(1, {ADC2, STRHAL_ADC_CHANNEL_11}, 1),
	baro_channel(2, baro, 1),
	x_accel(3, imu, IMUMeasurement::X_ACCEL, 1),
	y_accel(4, imu, IMUMeasurement::Y_ACCEL, 1),
	z_accel(5, imu, IMUMeasurement::Z_ACCEL, 1),
	x_gyro(6, imu, IMUMeasurement::X_GYRO, 1),
	y_gyro(7, imu, IMUMeasurement::Y_GYRO, 1),
	z_gyro(8, imu, IMUMeasurement::Z_GYRO, 1),
	speaker(STRHAL_TIM_TIM2, STRHAL_TIM_TIM2_CH3_PB10)
{
	cancom = Can::instance(this);
	flash = W25Qxx_Flash::instance(0x1F);
	registerChannel(&sense_5V);
	registerChannel(&sense_12V);
	registerChannel(&baro_channel);
	registerChannel(&x_accel);
	registerChannel(&y_accel);
	registerChannel(&z_accel);
	registerChannel(&x_gyro);
	registerChannel(&y_gyro);
	registerChannel(&z_gyro);

}

int RCU::init() {
	if(STRHAL_Init(STRHAL_SYSCLK_SRC_EXT, 8000000) != STRHAL_NOICE)
		return -1;

	// init status LEDs
	STRHAL_GPIO_SingleInit(&ledRed, STRHAL_GPIO_TYPE_OPP);
	STRHAL_GPIO_SingleInit(&ledGreen, STRHAL_GPIO_TYPE_OPP);

	// init debug uart
	if(STRHAL_UART_Instance_Init(STRHAL_UART_DEBUG) != 0)
		return -1;

	if(flash == nullptr)
		return -1;

	if(flash->init() != 0)
		return -1;

	if(cancom == nullptr)
		return -1;

	// TODO find out why IMU is dead
	if(imu.init() != 0)
		return -1;

	if(baro.init() != 0)
		return -1;

	//if(gnss.init() != 0)
		//return -1;

	if(lora.init() != 0)
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

int RCU::exec() {
	//STRHAL_OPAMP_Run();
	STRHAL_ADC_Run();
	STRHAL_QSPI_Run();

	CANState = cancom->exec();
	if(CANState != COMState::RUN)
		return -1;

	STRHAL_GPIO_Write(&ledRed, STRHAL_GPIO_VALUE_H);
	STRHAL_UART_Debug_Write_Blocking("RUNNING\n", 8, 50);

	speaker.beep(2, 400, 300);

	int counter = 0;
	uint8_t loraData[103];
	while(1) {
		/*
		int nn = 0;
		char readBuf[256] = { 0 };
		char writeBuf[256] = { 0 };
		nn = STRHAL_UART_Read(STRHAL_UART1, readBuf, 256);
		if(nn > 0) {
			for(int i = 0; i < nn-1; i++) {
				sprintf(writeBuf + strlen(writeBuf),"%c,",readBuf[i]);
			}
			sprintf(writeBuf + strlen(writeBuf),"%c\n",readBuf[nn-1]);
			STRHAL_UART_Debug_Write_Blocking(writeBuf, strlen(writeBuf),100);
		};
		*/
		/*
		int32_t n = STRHAL_UART_Read(STRHAL_UART1, readBuf, STRHAL_UART_BUF_SIZE);
		if(n > 0) {
			if(gnss.processData((uint8_t *) readBuf, n)) {
				sprintf(writeBuf,"%d, %ld, %ld\n",gnss.position.Status,gnss.position.Longitude,gnss.position.Latitude);
				STRHAL_UART_Write_Blocking(STRHAL_UART_DEBUG, writeBuf, strlen(writeBuf), 200);
			} else {
				STRHAL_UART_Write_Blocking(STRHAL_UART_DEBUG, "NOFIX\n", 6, 200);
			}
		}*/
		counter++;
		if(counter == 100000) {
			counter = 0;
			/*uint64_t time = STRHAL_Systick_GetTick();
			loraData[0] = (uint8_t) time;
			loraData[1] = (uint8_t) (time >> 8);
			loraData[2] = (uint8_t) (time >> 16);
			loraData[3] = (uint8_t) (time >> 24);
			loraData[4] = (uint8_t) (time >> 32);
			loraData[5] = (uint8_t) (time >> 40);
			loraData[6] = (uint8_t) (time >> 48);
			loraData[7] = (uint8_t) (time >> 56);
			//gnss.sendConfiguration(GNSSConstellation::ALL, GNSSSbasConstellation::ALL, GNSSDynamicsMode::AIRBORNE4G);
			int sent = lora.sendBytes(loraData, 103);
			char buf[32];
			sprintf(buf,"sent %d\n",sent);
			STRHAL_UART_Debug_Write_Blocking(buf, strlen(buf),100);*/
			while(lora.isBusy());

			uint8_t who = lora.whoAmI();
			char buf[32];
			sprintf(buf,"ID: %d\n",who);
			STRHAL_UART_Debug_Write_Blocking(buf, strlen(buf),100);
		}
		if(GenericChannel::exec() != 0)
			return -1;
	}

	speaker.beep(6, 100, 100);

	return 0;
}
