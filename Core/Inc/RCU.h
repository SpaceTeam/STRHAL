#ifndef RCU_H
#define RCU_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/GenericChannel.h>
#include <Channels/BaroChannel.h>
#include <Can.h>
#include <Radio.h>
#include <Channels/IMUChannel.h>
#include "../Modules/W25Qxx_Flash.h"
#include "../Modules/LoRa1276F30_Radio.h"
#include "../Modules/SAM_M8Q_GNSS.h"
#include <Speaker.h>

#include <STRHAL.h>

class RCU: public GenericChannel
{
	public:
		RCU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		RCU(const RCU &other) = delete;
		RCU& operator=(const RCU &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

	private:
		W25Qxx_Flash &flash;

		STRHAL_GPIO_t ledRed, ledGreen;

		// Modules
		LPS25HB_Baro baro;
		ICM20602_IMU imu;
		LoRa1276F30_Radio lora;
		SAM_M8Q_GNSS gnss;

		// Channels
		ADCChannel sense_5V, sense_12V;
		BaroChannel baro_channel;
		IMUChannel x_accel, y_accel, z_accel, x_gyro, y_gyro, z_gyro;
		ADCChannel gps_longitude, gps_latitude, gps_altitude;

		// Coms
		Radio &radio;
		Can &can;

		Speaker speaker;
};

#endif /*RCU_H*/
