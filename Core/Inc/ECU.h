#ifndef ECU_H
#define ECU_H

#include <Channels/ADCChannel.h>
#include <Channels/DigitalOutChannel.h>
#include <Channels/DigitalInChannel.h>
#include <Channels/PyroChannel.h>
#include <Channels/PressureControlChannel.h>
#include <Channels/ServoChannel.h>
#include <Channels/GenericChannel.h>
#include <Channels/IMUChannel.h>
#include <CANCOM.h>
#include <W25Qxx_Flash.h>
#include <Speaker.h>

#include <LID.h>

class ECU : public GenericChannel {
	public:
		ECU(uint32_t node_id, uint32_t fw_version);
		ECU(const ECU &other) = delete;
		ECU& operator=(const ECU &other) = delete;

		int init() override;
		//int reset() override;
		int exec() override;

		void testChannels();
		void testServo(ServoChannel &servo);

		//static constexpr uint8_t ECU_CHANNEL_N = 22;

	private:
		CANCOM *cancom;
		COMState CANState;

		LID_GPIO_t ledRed, ledGreen;

		ADCChannel press_0, press_1, press_2, press_3, press_4, press_5;
		ADCChannel temp_0, temp_1, temp_2;
		ServoChannel servo_0, servo_1, servo_2;
		DigitalInChannel pyro0_cont, pyro1_cont, pyro2_cont;
		PyroChannel pyro_igniter0, pyro_igniter1, pyro_igniter2;
		DigitalOutChannel solenoid_0, solenoid_1;
		PressureControlChannel pressure_control;
		IMUChannel imu_0;
		ADCChannel pyro_sense;

		Speaker speaker;

		bool checkPyroVoltageHigh();
		void readoutMode();
		void loggingMode();
		uint32_t pyro_on_counter;

		//W25Qxx_Flash flash;
};

/*
0	press_fueltank			pressure		ADCChannel				ADC16
1	press_oxtank			pressure		ADCChannel				ADC16
2	press_chamber			pressure		ADCChannel				ADC16
3	press_fuelinj			pressure		ADCChannel				ADC16
4	press_oxinj				pressure		ADCChannel				ADC16
5	UNUSED					pressure		ADCChannel				ADC16
6	temp_oxbottom			temperature		ADCChannel				ADC16
7	temp_oxmiddle			temperature		ADCChannel				ADC16
8	temp_oxtop				temperature		ADCChannel				ADC16
9	servo_fuelmain			fdbk pos		ServoChannel			Servo
10	servo_oxmain			fdbk pos		ServoChannel			Servo
11	USUSDED					fdbk pos		ServoChannel			Servo
12	pyro_igniter0			current			DigitalOutChannel		DigitalOut
13	pyro0_cont				continuity		DigitalInChannel		ADC16
14	pyro_igniter1			current			DigitalOutChannel		DigitalOut
15	pyro1_cont				continuity		DigitalInChannel		ADC16
16	USUSED					current			DigitalOutChannel		DigitalOut
17	UNUSED					continuity		DigitalInChannel		ADC16
18	solenoid_supercharge	current			DigitalOutChannel		DigitalOut
19	UNUSED					current			DigitalOutChannel		DigitalOut
20	pressure_control		solenoid state	PressureControlChannel	PneumaticValve
 */

#endif /*ECU_H*/
