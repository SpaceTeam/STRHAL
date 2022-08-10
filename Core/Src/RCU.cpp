#include "../Inc/RCU.h"

#include <cstdio>
#include <cstring>
#include <stm32g4xx_ll_usart.h>

RCU::RCU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider) :
		GenericChannel(node_id, fw_version, refresh_divider),
		flash(W25Qxx_Flash::instance()),
		ledRed({ GPIOD, 1, STRHAL_GPIO_TYPE_OPP }),
		ledGreen({ GPIOD, 2, STRHAL_GPIO_TYPE_OPP }),
		baro(STRHAL_SPI_SPI1,{ STRHAL_SPI_SPI1_SCK_PA5, STRHAL_SPI_SPI1_MISO_PA6, STRHAL_SPI_SPI1_MOSI_PA7, STRHAL_SPI_SPI1_NSS_PA4, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0 },{ GPIOA, 3, STRHAL_GPIO_TYPE_IHZ }),
		imu(STRHAL_SPI_SPI3,{ STRHAL_SPI_SPI3_SCK_PC10, STRHAL_SPI_SPI3_MISO_PC11, STRHAL_SPI_SPI3_MOSI_PC12, STRHAL_SPI_SPI3_NSS_PA15, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0 },{ GPIOD, 0, STRHAL_GPIO_TYPE_IHZ }),
		lora(STRHAL_SPI_SPI2,{ STRHAL_SPI_SPI2_SCK_PB13, STRHAL_SPI_SPI2_MISO_PB14, STRHAL_SPI_SPI2_MOSI_PB15, STRHAL_SPI_SPI2_NSS_PB12, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_LL, 0x7, 0 },{ GPIOC, 1, STRHAL_GPIO_TYPE_IHZ },{ GPIOC, 3, STRHAL_GPIO_TYPE_IHZ },{ GPIOB, 11, STRHAL_GPIO_TYPE_IHZ }),
		gnss(STRHAL_UART1,{ GPIOC, 7, STRHAL_GPIO_TYPE_OPP }),
		sense_5V(0,{ ADC1, STRHAL_ADC_CHANNEL_2 }, 1),
		sense_12V(1,{ ADC1, STRHAL_ADC_CHANNEL_3 }, 1),
		baro_channel(2, baro, 1),
		x_accel(3, &imu, IMUMeasurement::X_ACCEL, 1),
		y_accel(4, &imu, IMUMeasurement::Y_ACCEL, 1),
		z_accel(5, &imu, IMUMeasurement::Z_ACCEL, 1),
		x_gyro(6, &imu, IMUMeasurement::X_GYRO, 1),
		y_gyro(7, &imu, IMUMeasurement::Y_GYRO, 1),
		z_gyro(8, &imu, IMUMeasurement::Z_GYRO, 1),
		gps_longitude(9, &gnss.gnssData.longitude, 1),
		gps_latitude(10, &gnss.gnssData.latitude, 1),
		gps_altitude(11, &gnss.gnssData.altitude, 1),
		radio(Radio::instance(node_id, lora)),
		can(Can::instance(node_id)),
		speaker(STRHAL_TIM_TIM2, STRHAL_TIM_TIM2_CH3_PB10)
{
	registerChannel(&sense_5V);
	registerChannel(&sense_12V);
	registerChannel(&baro_channel);
	registerChannel(&x_accel);
	registerChannel(&y_accel);
	registerChannel(&z_accel);
	registerChannel(&x_gyro);
	registerChannel(&y_gyro);
	registerChannel(&z_gyro);

	registerModule(&flash);
	registerModule(&gnss);
	registerModule(&baro);
	//registerModule(&imu);

}

int RCU::init()
{
	if (STRHAL_Init(STRHAL_SYSCLK_SRC_EXT, 8000000) != STRHAL_NOICE)
		return -1;

	// init status LEDs
	STRHAL_GPIO_SingleInit(&ledRed, STRHAL_GPIO_TYPE_OPP);
	STRHAL_GPIO_SingleInit(&ledGreen, STRHAL_GPIO_TYPE_OPP);

	// init debug uart
	if (STRHAL_UART_Instance_Init(STRHAL_UART_DEBUG) != 0)
		return -1;

	if (lora.init() != 0)
		return -1;

	if (can.init(receptor, heartbeatCan, COMMode::STANDARD_COM_MODE) != 0)
		return -1;

	if (radio.init(nullptr, heartbeatLora) != 0)
		return -1;

	if (GenericChannel::init() != 0)
		return -1;

	speaker.init();

	STRHAL_GPIO_Write(&ledGreen, STRHAL_GPIO_VALUE_H);
	return 0;
}

int RCU::exec()
{
	//STRHAL_OPAMP_Run();
	STRHAL_ADC_Run();
	STRHAL_QSPI_Run();

	if (can.exec() != 0)
		return -1;

	if (radio.exec() != 0)
		return -1;

	STRHAL_GPIO_Write(&ledRed, STRHAL_GPIO_VALUE_H);
	STRHAL_UART_Debug_Write_Blocking("RUNNING\n", 8, 50);

	speaker.beep(2, 400, 300);

	LL_mDelay(2000);

#ifdef UART_DEBUG
	STRHAL_UART_Listen(STRHAL_UART_DEBUG);

	uint8_t msgBuf[128] =
	{ 0 };
	uint8_t bufIndex = 0;
	bool msgStarted = false;
#endif
	while (1)
	{
		//detectReadoutMode();
#ifdef UART_DEBUG

		uint8_t tempBuf[64] =
		{ 0 };
		int32_t ret = STRHAL_UART_Read(STRHAL_UART_DEBUG, (char *) tempBuf, 64);
		if(ret > 0)
		{
			if(msgStarted)
			{
				memcpy(&msgBuf[bufIndex-1], tempBuf, ret);
				bufIndex += ret;
				if(tempBuf[ret-1] == 0x0A) // msg ended
				{
					msgStarted = false;
					bufIndex = 0;
					receptor((uint32_t) (msgBuf[0] << 11), &msgBuf[1], bufIndex - 1);
					memset(msgBuf, 0, 128);
				}
			}
			else
			{
				if(tempBuf[0] == 0x3A) // start byte
				{
					if(tempBuf[ret-1] == 0x0A) // msg ended
					{
						memcpy(msgBuf, tempBuf, ret - 1);
						receptor((uint32_t) (msgBuf[0] << 11), &msgBuf[1], ret - 1);
						memset(msgBuf, 0, 128);
					}
					bufIndex += ret;
					msgStarted = true;
					if(ret > 1)
						memcpy(msgBuf, &tempBuf[1], ret-1);
				}
				// else ignore msg
			}

		}
#endif
		if (GenericChannel::exec() != 0)
			return -1;
	}

	speaker.beep(6, 100, 100);

	return 0;
}
