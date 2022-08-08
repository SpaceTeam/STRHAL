#ifndef SAM_M8Q_GNSS_H
#define SAM_M8Q_GNSS_H

#include <STRHAL.h>
#include "../Ubx.h"
#include "./Modules/AbstractModule.h"

#define UBLOX_SYNC1     0xB5
#define UBLOX_SYNC2     0x62

#define UBLOX_NAV_CLASS     0x01
#define UBLOX_RXM_CLASS     0x02
#define UBLOX_CFG_CLASS     0x06
#define UBLOX_MON_CLASS     0x0a
#define UBLOX_AID_CLASS     0x0b
#define UBLOX_TIM_CLASS     0x0d

#define UBLOX_NAV_POSLLH    0x02
#define UBLOX_NAV_STATUS    0x03
#define UBLOX_NAV_DOP       0x04
#define UBLOX_NAV_SOL       0x06
#define UBLOX_NAV_VELNED    0x12
#define UBLOX_NAV_TIMEUTC   0x21
#define UBLOX_NAV_SBAS      0x32
#define UBLOX_NAV_SVINFO    0x30

#define UBLOX_AID_REQ       0x00

#define UBLOX_RXM_RAW       0x10
#define UBLOX_RXM_SFRB      0x11

#define UBLOX_MON_VER       0x04
#define UBLOX_MON_HW        0x09

#define UBLOX_TIM_TP        0x01

#define UBLOX_CFG_MSG       0x01
#define UBLOX_CFG_TP        0x07
#define UBLOX_CFG_RATE      0x08
#define UBLOX_CFG_CFG       0x09
#define UBLOX_CFG_SBAS      0x16
#define UBLOX_CFG_NAV5      0x24
#define UBLOX_CFG_GNSS      0x3E

#define UBLOX_SBAS_AUTO     0x00000000
// TODO: Reverify these constants-- seems they have more bits set than they
// should.
#define UBLOX_SBAS_WAAS     0x0004E004
#define UBLOX_SBAS_EGNOS    0x00000851
#define UBLOX_SBAS_MSAS     0x00020200
#define UBLOX_SBAS_GAGAN    0x00000108

#define UBLOX_DYN_PORTABLE   0
#define UBLOX_DYN_STATIONARY 2
#define UBLOX_DYN_PED        3
#define UBLOX_DYN_AUTOMOTIVE 4
#define UBLOX_DYN_SEA        5
#define UBLOX_DYN_AIR1G      6
#define UBLOX_DYN_AIR2G      7
#define UBLOX_DYN_AIR4G      8

#define UBLOX_GNSSID_GPS     0
#define UBLOX_GNSSID_SBAS    1
#define UBLOX_GNSSID_BEIDOU  3
#define UBLOX_GNSSID_QZSS    5
#define UBLOX_GNSSID_GLONASS 6

#define UBLOX_MAX_PAYLOAD   384
#define UBLOX_WAIT_MS       100

enum class GNSSDynamicsMode : uint8_t
{
	PORTABLE,
	PEDESTRIAN,
	AUTOMOTIVE,
	AIRBORNE1G,
	AIRBORNE2G,
	AIRBORNE4G
};

enum class GNSSSbasConstellation : uint8_t
{
	WAAS,
	EGNOS,
	MSAS,
	GAGAN,
	ALL,
	NONE
};

enum class GNSSConstellation : uint8_t
{
	GLONASS,
	ALL,
	GPS
};

enum class GNSSBaudRate : uint8_t
{
	_2400,
	_4800,
	_9600,
	_19200,
	_38400,
	_57600,
	_115200,
	_230400
};

typedef struct
{
		uint16_t longitude;
		uint16_t latitude;
		uint16_t altitude;
} GNSSData_t;

class SAM_M8Q_GNSS: public AbstractModule
{
	public:
		SAM_M8Q_GNSS(const STRHAL_UART_Id_t uartId, const STRHAL_GPIO_t &resetPin);

		int init() override;
		int exec() override;
		int reset() override;

		int processData(uint8_t *buffer, uint32_t length);

		int sendConfiguration(GNSSConstellation constellation, GNSSSbasConstellation sbas, GNSSDynamicsMode mode);

		GPSPositionData position =
		{ .Status = GPSPOSITION_STATUS_NOFIX };
		GNSSData_t gnssData =
		{ 0 };
	private:
		void resetChecksum();
		void updateChecksum(uint8_t c);

		int sendConfigDataChecksummed(const uint8_t *data, uint16_t length, uint32_t retries);
		int waitForACK(uint32_t delay);
		int enableMessage(uint8_t msgClass, uint8_t msgId, uint8_t rate);
		int setMessageRate(uint16_t msPeriod);
		int setMode(GNSSDynamicsMode mode);
		int setTimepulse();
		int setSbas(GNSSSbasConstellation sbas);
		int pollVersion();
		int setConstellation(GNSSConstellation constellation, GNSSSbasConstellation sbas);
		int clearConfig();
		//void setBaudrate(uintptr_t gps_port, GNSSBaudRate baud)

		const STRHAL_UART_Id_t uartId;
		STRHAL_GPIO_t resetPin;
		uint8_t checksumTxA = 0;
		uint8_t checksumTxB = 0;

		char gps_rx_buff[sizeof(struct UBXPacket)];
		struct GPS_RX_STATS rx_stats =
		{ 0 };

};

#endif /*SAM_M8Q_GNSS_H*/
