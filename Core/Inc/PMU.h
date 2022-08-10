#ifndef PMU_H
#define PMU_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/GenericChannel.h>
#include <Channels/BaroChannel.h>
#include <Can.h>
#include <Channels/IMUChannel.h>
#include <Speaker.h>

#include <STRHAL.h>

class PMU: public GenericChannel
{
	public:
		PMU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		PMU(const PMU &other) = delete;
		PMU& operator=(const PMU &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

	private:
		W25Qxx_Flash &flash;

		STRHAL_GPIO_t ledRed, ledGreen;
		// Modules
		LPS25HB_Baro baro;
		ICM20602_IMU imu;

		// Channels
		ADCChannel sense_5V, sense_5VP, sense_12V;
		//DigitalInChannel umbilical, charging;
		DigitalOutChannel out0, out1, out2, out3, payload;
		BaroChannel baro_channel;
		IMUChannel x_accel, y_accel, z_accel, x_gyro, y_gyro, z_gyro;

		// Coms
		Can &can;

		Speaker speaker;
};

#endif /*PMU_H*/
