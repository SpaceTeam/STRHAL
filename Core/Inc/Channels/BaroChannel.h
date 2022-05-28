#ifndef BAROCHANNEL_H
#define BAROCHANNEL_H

#include "AbstractChannel.h"
#include <can_houbolt/channels/adc24_channel_def.h>
#include <STRHAL.h>

enum class BaroAddr : uint8_t {
	REF_P_XL = 0x08,
	REF_P_L = 0x09,
	REF_P_H = 0x0A,
	WHO_AM_I = 0x0F,
	RES_CONF = 0x10,
	CTRL_REG1 = 0x20,
	CTRL_REG2 = 0x21,
	CTRL_REG3 = 0x22,
	CTRL_REG4 = 0x23,
	INTERRUPT_CGF = 0x24,
	STATUS_REG = 0x27,
	PRESS_OUT_XL = 0x28,
	PRESS_OUT_L = 0x29,
	PRESS_OUT_H = 0x2A,
	TEMP_OUT_L = 0x2B,
	TEMP_OUT_H = 0x2C,
	FIFO_CTRL = 0x2E,
};

class BaroChannel : public AbstractChannel {
	public:
		BaroChannel(uint8_t id, const STRHAL_SPI_Id_t &spiId, const STRHAL_SPI_Config_t &spiConf, uint32_t refreshDivider);

		BaroChannel(const BaroChannel &other) = delete;
		BaroChannel& operator=(const BaroChannel &other) = delete;
		BaroChannel(const BaroChannel &&other) = delete;
		BaroChannel& operator=(const BaroChannel &&other) = delete;

		~BaroChannel();

		int init() override;
		int reset() override;
		int exec() override;

		int getSensorData(uint8_t *data, uint8_t &n) override;

		bool IsMeasurementAvailable() const;
		bool getMeasurement(int32_t &x);

		uint8_t whoAmI() const;

		int processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n) override;

		static constexpr uint32_t BUF_DATA_SIZE = 64;
		static constexpr uint8_t READ_BIT = 0x80;

		static constexpr uint64_t EXEC_SAMPLE_TICKS = 50;

	protected:
		int setVariable(uint8_t variableId, int32_t data) override;
		int getVariable(uint8_t variableId, int32_t &data) const override;

	private:
		bool readReg(const BaroAddr &address, uint8_t *reg, uint8_t n = 0);
		bool writeReg(const BaroAddr &address, uint8_t reg, uint16_t delay = 0);
		STRHAL_SPI_Id_t spiId;
		STRHAL_SPI_Config_t spiConf;

		int32_t measData[BUF_DATA_SIZE] = {0};
		uint32_t measDataTail, measDataNum;
		uint64_t timeLastSample = 0;
};

#endif /*BAROCHANNEL_H*/
