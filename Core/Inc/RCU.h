#ifndef RCU_H
#define RCU_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/GenericChannel.h>
#include <Channels/IMUChannel.h>
#include <Channels/IMUSingleChannel.h>
#include <Channels/BaroChannel.h>
#include <Can.h>
#include <W25Qxx_Flash.h>
#include <Speaker.h>

#include <STRHAL.h>

class RCU : public GenericChannel {
	public:
		RCU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		RCU(const RCU &other) = delete;
		RCU& operator=(const RCU &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

		void testChannels();

	private:
		Can *cancom;
		COMState CANState;

		W25Qxx_Flash *flash;

		STRHAL_GPIO_t ledRed, ledGreen;

		ADCChannel sense_5V, sense_12V;
		BaroChannel barometer;
		IMUChannel imu_0;
		IMUSingleChannel x_accel, y_accel, z_accel, x_gyro, y_gyro, z_gyro;
		Speaker speaker;
};

#endif /*RCU_H*/
