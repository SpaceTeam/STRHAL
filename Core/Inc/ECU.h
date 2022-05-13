#ifndef ECU_H
#define ECU_H

#include <Can.h>
#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/PyroChannel.h>
#include <Channels/PressureControlChannel.h>
#include <Channels/ServoChannel.h>
#include <Channels/GenericChannel.h>
#include <Channels/IMUChannel.h>
#include <Channels/IMUSingleChannel.h>
#include <Channels/RocketChannel.h>
#include <W25Qxx_Flash.h>
#include <Speaker.h>

#include <STRHAL.h>

class ECU : public GenericChannel {
	public:
		ECU(uint32_t node_id, uint32_t fw_version, uint32_t refresh_divider);
		ECU(const ECU &other) = delete;
		ECU& operator=(const ECU &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

		void testChannels();
		void testServo(ServoChannel &servo);

	private:
		Can *cancom;
		COMState CANState;

		W25Qxx_Flash *flash;

		STRHAL_GPIO_t ledRed, ledGreen;

		ADCChannel press_0, press_1, press_2, press_3, press_4, press_5;
		ADCChannel temp_0, temp_1, temp_2;
		ServoChannel servo_0, servo_1, servo_2;
		DigitalInChannel pyro0_cont, pyro1_cont, pyro2_cont;
		PyroChannel pyro_igniter0, pyro_igniter1, pyro_igniter2;
		DigitalOutChannel solenoid_0, solenoid_1;
		PressureControlChannel pressure_control;
		//IMUChannel imu_0;
		//IMUSingleChannel x_accel, y_accel, z_accel, x_gyro, y_gyro, z_gyro;
		RocketChannel rocket;

		Speaker speaker;
};

#endif /*ECU_H*/
