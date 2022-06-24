#include "../Inc/RCU.h"

#include <cstdio>
#include <cstring>
#include <stm32g4xx_ll_usart.h>

RCU::RCU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider) :
	GenericChannel(node_id, fw_version, refresh_divider),
	ledRed({GPIOD, 1, STRHAL_GPIO_TYPE_OPP}),
	ledGreen({GPIOD, 2, STRHAL_GPIO_TYPE_OPP}),
	baro(STRHAL_SPI_SPI1, {STRHAL_SPI_SPI1_SCK_PA5, STRHAL_SPI_SPI1_MISO_PA6, STRHAL_SPI_SPI1_MOSI_PA7, STRHAL_SPI_SPI1_NSS_PA4, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0},{GPIOA, 3, STRHAL_GPIO_TYPE_IHZ}),
	imu(STRHAL_SPI_SPI3, {STRHAL_SPI_SPI3_SCK_PC10, STRHAL_SPI_SPI3_MISO_PC11, STRHAL_SPI_SPI3_MOSI_PC12, STRHAL_SPI_SPI3_NSS_PA15, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0},{GPIOD, 0, STRHAL_GPIO_TYPE_IHZ}),
	lora(STRHAL_SPI_SPI2, {STRHAL_SPI_SPI2_SCK_PB13, STRHAL_SPI_SPI2_MISO_PB14, STRHAL_SPI_SPI2_MOSI_PB15, STRHAL_SPI_SPI2_NSS_PB12, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_LL, 0x7, 0},{GPIOC, 1, STRHAL_GPIO_TYPE_IHZ},{GPIOC, 3, STRHAL_GPIO_TYPE_IHZ},{GPIOB, 11, STRHAL_GPIO_TYPE_IHZ}),
	gnss(STRHAL_UART1, {GPIOC, 7, STRHAL_GPIO_TYPE_OPP}),
	sense_5V(0, {ADC1, STRHAL_ADC_CHANNEL_2}, 1),
	sense_12V(1, {ADC1, STRHAL_ADC_CHANNEL_3}, 1),
	baro_channel(2, baro, 1),
	x_accel(3, &imu, IMUMeasurement::X_ACCEL, 1),
	y_accel(4, &imu, IMUMeasurement::Y_ACCEL, 1),
	z_accel(5, &imu, IMUMeasurement::Z_ACCEL, 1),
	x_gyro(6, &imu, IMUMeasurement::X_GYRO, 1),
	y_gyro(7, &imu, IMUMeasurement::Y_GYRO, 1),
	z_gyro(8, &imu, IMUMeasurement::Z_GYRO, 1),
	speaker(STRHAL_TIM_TIM2, STRHAL_TIM_TIM2_CH3_PB10)
{
	com = Communication::instance(this, &lora);
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

	// TODO find out why IMU is dead
	if(imu.init() != 0)
		return -1;

	if(baro.init() != 0)
		return -1;

	if(gnss.init() != 0)
		return -1;

	if(lora.init() != 0)
		return -1;

	if(com == nullptr)
		return -1;

	//if(com->init(COMMode::LISTENER_COM_MODE) != 0)
	if(com->init() != 0)
		return -1;

	if(flash == nullptr)
		return -1;

	if(flash->init() != 0) //has to be init after Com
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

	if(com->exec() != 0)
		return -1;

	STRHAL_GPIO_Write(&ledRed, STRHAL_GPIO_VALUE_H);
	STRHAL_UART_Debug_Write_Blocking("RUNNING\n", 8, 50);

	speaker.beep(2, 400, 300);

	LL_mDelay(2000);
	int counter = 0;
	//uint8_t loraData[101];
	//memset(loraData,0,101);
	//STRHAL_UART_Listen(STRHAL_UART1);
	char canErr[64];
	while(1) {
		/*counter++;
		if(counter == 100000) {
			counter = 0;
			sprintf((char *)loraData,"geh scheissen markus!");
			//gnss.sendConfiguration(GNSSConstellation::ALL, GNSSSbasConstellation::ALL, GNSSDynamicsMode::AIRBORNE4G);
			bool sent = lora.sendBytes(loraData, 101);
			char buf[32];
			sprintf(buf,"sent %d\n",sent);
			STRHAL_UART_Debug_Write_Blocking(buf, strlen(buf),100);
			//while(lora.isBusy());

			uint8_t who = lora.getStatus();
			char buf2[32];
			sprintf(buf2,"ID: %d\n",who);
			STRHAL_UART_Debug_Write_Blocking(buf2, strlen(buf2),100);
		}*/

		//detectReadoutMode();
		/*counter++;
		if(counter == 10000) {
			counter = 0;
			sprintf(canErr,"%ld %ld\n",FDCAN1->ECR,FDCAN1->PSR);
			STRHAL_UART_Debug_Write_Blocking(canErr, strlen(canErr),100);
		}*/
		/*int nn = 0;
		char readBuf[256] = { 0 };
		nn = STRHAL_UART_Read(STRHAL_UART1, readBuf, 20);
		//nn = STRHAL_UART_Read_Blocking(STRHAL_UART1, readBuf, 256, 10);
		if(nn > 0) {
			STRHAL_UART_Debug_Write_Blocking(readBuf, strlen(readBuf),100);
		}*/
		if(flash->exec() != 0)
			return -1;
		if(GenericChannel::exec() != 0)
			return -1;
		//com->heartbeatLora();
		//LL_mDelay(1000);
	}

	speaker.beep(6, 100, 100);

	return 0;
}
