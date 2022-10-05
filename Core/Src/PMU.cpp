#include "../Inc/PMU.h"

#include <cstdio>
#include <cstring>

PMU::PMU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider) :
		GenericChannel(node_id, fw_version, refresh_divider),
		ledRed({ GPIOD, 1, STRHAL_GPIO_TYPE_OPP }),
		ledGreen({ GPIOD, 2, STRHAL_GPIO_TYPE_OPP }),
		baro(STRHAL_SPI_SPI1,{ STRHAL_SPI_SPI1_SCK_PA5, STRHAL_SPI_SPI1_MISO_PA6, STRHAL_SPI_SPI1_MOSI_PA7, STRHAL_SPI_SPI1_NSS_PA4, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0 },{ GPIOA, 3, STRHAL_GPIO_TYPE_IHZ }),
		imu(STRHAL_SPI_SPI3,{ STRHAL_SPI_SPI3_SCK_PC10, STRHAL_SPI_SPI3_MISO_PC11, STRHAL_SPI_SPI3_MOSI_PC12, STRHAL_SPI_SPI3_NSS_PA15, STRHAL_SPI_MODE_MASTER, STRHAL_SPI_CPOL_CPHASE_HH, 0x7, 0 },{ GPIOD, 0, STRHAL_GPIO_TYPE_IHZ }, 0xAF),
		sense_5V(0,{ ADC2, STRHAL_ADC_CHANNEL_5 }, 1),
		sense_5VP(1,{ ADC1, STRHAL_ADC_CHANNEL_9 }, 1),
		sense_12V(2,{ ADC2, STRHAL_ADC_CHANNEL_11 }, 1),
		sense_battery(3,{ ADC1, STRHAL_ADC_CHANNEL_8 }, 1),
		sense_pyro(4,{ ADC3, STRHAL_ADC_CHANNEL_7 }, 1),
		sense_suppy_current(5,{ ADC3, STRHAL_ADC_CHANNEL_2 }, 1),
		umbilical(6, {GPIOA, 8, STRHAL_GPIO_TYPE_IHZ}, 1),
		charging(7, {GPIOA, 9, STRHAL_GPIO_TYPE_IHZ}, 1),
		chargingEna(8,{ GPIOA, 10, STRHAL_GPIO_TYPE_OPP }, 1),
		out0(9,{ GPIOC, 6, STRHAL_GPIO_TYPE_OPP }, 1),
		out1(10,{ GPIOC, 7, STRHAL_GPIO_TYPE_OPP }, 1),
		out2(11,{ GPIOC, 8, STRHAL_GPIO_TYPE_OPP }, 1),
		out3(12,{ GPIOC, 9, STRHAL_GPIO_TYPE_OPP }, 1),
		payload(13,{ GPIOC, 1, STRHAL_GPIO_TYPE_OPP }, 1),
		baro_channel(14, baro, 1),
		x_accel(15, &imu, IMUMeasurement::X_ACCEL, 1),
		y_accel(16, &imu, IMUMeasurement::Y_ACCEL, 1),
		z_accel(17, &imu, IMUMeasurement::Z_ACCEL, 1),
		x_gyro(18, &imu, IMUMeasurement::X_GYRO, 1),
		y_gyro(19, &imu, IMUMeasurement::Y_GYRO,1),
		z_gyro(20, &imu, IMUMeasurement::Z_GYRO, 1),
		speaker(STRHAL_TIM_TIM2, STRHAL_TIM_TIM2_CH3_PB10)
{
	registerChannel(&sense_5V); // 2*x in Volts
	registerChannel(&sense_5VP); // 2*x in Volts
	registerChannel(&sense_12V); // 6.6*x in Volts
	registerChannel(&sense_battery); // 6.6*x in Volts
	registerChannel(&sense_pyro); // 6.6*x in Volts
	registerChannel(&sense_suppy_current); // 10000*x in mA
	registerChannel(&umbilical); // if LOW -> valid input voltage present
	registerChannel(&charging); // if LOW -> normal charging in progress
	registerChannel(&chargingEna);
	registerChannel(&out0);
	registerChannel(&out1);
	registerChannel(&out2);
	registerChannel(&out3);
	registerChannel(&payload);
	registerChannel(&baro_channel);
	registerChannel(&x_accel);
	registerChannel(&y_accel);
	registerChannel(&z_accel);
	registerChannel(&x_gyro);
	registerChannel(&y_gyro);
	registerChannel(&z_gyro);

	registerModule(&flash);
	registerModule(&baro);
	registerModule(&imu);

}

int PMU::init()
{
	if (STRHAL_Init(STRHAL_SYSCLK_SRC_EXT, 8000000) != STRHAL_NOICE)
		return -1;

	// init status LEDs
	STRHAL_GPIO_SingleInit(&ledRed, STRHAL_GPIO_TYPE_OPP);
	STRHAL_GPIO_SingleInit(&ledGreen, STRHAL_GPIO_TYPE_OPP);

	// init debug uart
	if (STRHAL_UART_Instance_Init(STRHAL_UART_DEBUG) != 0)
		return -1;

	if (can.init(receptor, heartbeatCan, COMMode::STANDARD_COM_MODE) != 0)
		return -1;

	if (GenericChannel::init() != 0)
		return -1;

	speaker.init();

	STRHAL_GPIO_Write(&ledGreen, STRHAL_GPIO_VALUE_H);
	return 0;
}

int PMU::exec()
{
	//STRHAL_OPAMP_Run();
	STRHAL_ADC_Run();
	STRHAL_QSPI_Run();

	if (can.exec() != 0)
		return -1;

	STRHAL_GPIO_Write(&ledRed, STRHAL_GPIO_VALUE_H);
	STRHAL_UART_Debug_Write_Blocking("RUNNING\n", 8, 50);

	speaker.beep(3, 300, 200);

	while (1)
	{
		//detectReadoutMode();
		if (GenericChannel::exec() != 0)
			return -1;
	}

	speaker.beep(6, 100, 100);

	return 0;
}
