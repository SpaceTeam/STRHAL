#include "../../Inc/Modules/SAM_M8Q_GNSS.h"
#include <stm32g4xx_ll_exti.h>
#include <stm32g4xx_ll_system.h>

#include <cstring>
#include <cstdio>

SAM_M8Q_GNSS::SAM_M8Q_GNSS(const STRHAL_UART_Id_t uartId, const STRHAL_GPIO_t &resetPin) :
		uartId(uartId), resetPin(resetPin)
{
}

int SAM_M8Q_GNSS::init()
{
	// init reset GPIO
	STRHAL_GPIO_SingleInit(&resetPin, STRHAL_GPIO_TYPE_OPP);
	STRHAL_GPIO_Write(&resetPin, STRHAL_GPIO_VALUE_H);

	// init GNSS uart
	if (STRHAL_UART_Instance_Init(uartId) != 0)
		return -1;

	LL_mDelay(10);

	STRHAL_UART_Listen(uartId);

	reset();

	LL_mDelay(1000);

	//if (!sendConfiguration(GNSSConstellation::ALL, GNSSSbasConstellation::ALL, GNSSDynamicsMode::AIRBORNE4G))
		//return -1;

	return 0;
}

int SAM_M8Q_GNSS::exec()
{
	uint8_t gnssBuf[64] =
	{ 0 };
	int32_t gnssRet = STRHAL_UART_Read(STRHAL_UART1, (char*) gnssBuf, 64);
	if (gnssRet > 0)
	{
		processData(gnssBuf, gnssRet);
	}
	return 0;
}

int SAM_M8Q_GNSS::reset()
{
	STRHAL_GPIO_Write(&resetPin, STRHAL_GPIO_VALUE_L);
	LL_mDelay(2000);
	STRHAL_GPIO_Write(&resetPin, STRHAL_GPIO_VALUE_H);
	return 0;
}

int SAM_M8Q_GNSS::processData(uint8_t *buffer, uint32_t length)
{
	uint32_t id;
	uint8_t ret = 0;
	for (uint32_t i = 0; i < length; i++)
	{
		if (Ubx::parseStream(buffer[i], gps_rx_buff, &position, &rx_stats, &id) == PARSER_COMPLETE)
		{
			if (position.Satellites < 0)
			{
				position.Satellites = 0;
			}
			// Check if we received a valid position solution and the solution is valid
			if (id == UBX_ID_POSLLH)
			{
				if (position.Status > GPSPOSITION_STATUS_NOFIX)
				{
					// Update the timestamp for the last valid solution
					//status.not_stored.body.gnss_last_fix_timestamp = status.main.body.timestamp;
					gnssData.longitude = (uint16_t) (position.Longitude >> 16);
					gnssData.latitude = (uint16_t) (position.Latitude >> 16);
					gnssData.altitude = (uint16_t) position.Altitude;
					ret = 1;
				}
				else
				{
					// NOFIX
				}
			}
		}
	}
	return ret;
}

int SAM_M8Q_GNSS::sendConfiguration(GNSSConstellation constellation, GNSSSbasConstellation sbas, GNSSDynamicsMode mode)
{
	int ret = 1;

	ret &= clearConfig();
	//ret &= setTimepulse();
	ret &= enableMessage(UBLOX_NAV_CLASS, UBLOX_NAV_VELNED, 1);    // NAV-VELNED
	ret &= enableMessage(UBLOX_NAV_CLASS, UBLOX_NAV_POSLLH, 1);    // NAV-POSLLH
	ret &= enableMessage(UBLOX_NAV_CLASS, UBLOX_NAV_SOL, 1);       // NAV-SOL
	ret &= enableMessage(UBLOX_NAV_CLASS, UBLOX_NAV_TIMEUTC, 5);   // NAV-TIMEUTC
	ret &= enableMessage(UBLOX_NAV_CLASS, UBLOX_NAV_DOP, 1);       // NAV-DOP
	ret &= enableMessage(UBLOX_NAV_CLASS, UBLOX_NAV_SVINFO, 5);    // NAV-SVINFO

	ret &= setMode(mode);                // Set the requested gps mode
	ret &= setSbas(sbas);                // Set SBAS configuration
	if (constellation == GNSSConstellation::ALL)
	{ // Some Modules dont allow 10Hz when using ALL GPS
		ret &= setMessageRate((uint16_t) 200);
	}
	else
	{
		ret &= setMessageRate((uint16_t) 100);
	}
	ret &= setConstellation(constellation, sbas);
	return ret;
}

void SAM_M8Q_GNSS::resetChecksum()
{
	checksumTxA = 0;
	checksumTxB = 0;
}

void SAM_M8Q_GNSS::updateChecksum(uint8_t c)
{
	checksumTxA += c;
	checksumTxB += checksumTxA;
}

int SAM_M8Q_GNSS::sendConfigDataChecksummed(const uint8_t *data, uint16_t length, uint32_t retries)
{
	// Calculate checksum
	resetChecksum();
	for (uint16_t i = 0; i < length; i++)
	{
		updateChecksum(data[i]);
	}

	// Send buffer followed by checksum
	const uint8_t syncword[] =
	{ UBLOX_SYNC1, UBLOX_SYNC2 };
	const uint8_t checksum[] =
	{ checksumTxA, checksumTxB };

	STRHAL_UART_Write_Blocking(uartId, (const char*) syncword, sizeof(syncword), 50);
	STRHAL_UART_Write_Blocking(uartId, (const char*) data, length, 50);
	STRHAL_UART_Write_Blocking(uartId, (const char*) checksum, sizeof(checksum), 50);

	for (uint32_t i = 0; i < retries; i++)
	{
		if (waitForACK(1000) == 1)
		{
			return 1;
		}
	}
	return 0;
}

int SAM_M8Q_GNSS::waitForACK(uint32_t delay)
{
	int ret = PARSER_INCOMPLETE;
	struct GPS_RX_STATS gpsRxStats;
	GPSPositionData gpsPosition;

	uint8_t c;
	uint64_t enterTime = STRHAL_Systick_GetTick();
	while ((STRHAL_Systick_GetTick() - enterTime) < delay)
	{
		uint16_t received = STRHAL_UART_Read(uartId, (char*) &c, 1);
		if (received > 0)
		{
			ret = Ubx::parseStream(c, gps_rx_buff, &gpsPosition, &gpsRxStats, 0);
			switch (ret)
			{
				case PARSER_COMPLETE_ACK:
					return 1;
				case PARSER_COMPLETE_NAK:
					return 0;
				default:
					break;
			}
		}
	}

	return -1;     // Timeout
}

int SAM_M8Q_GNSS::enableMessage(uint8_t msgClass, uint8_t msgId, uint8_t rate)
{
	const uint8_t msg[] =
	{
	UBLOX_CFG_CLASS,       // CFG
			UBLOX_CFG_MSG,         // MSG
			0x03,                  // length lsb
			0x00,                  // length msb
			msgClass,              // class
			msgId,                 // id
			rate,                  // rate
			};
	return sendConfigDataChecksummed(msg, sizeof(msg), 5);
}

int SAM_M8Q_GNSS::setMessageRate(uint16_t msPeriod)
{
	const uint8_t msg[] =
	{
	UBLOX_CFG_CLASS,       // CFG
			UBLOX_CFG_RATE,        // RATE
			0x06,                  // length lsb
			0x00,                  // length msb
			(uint8_t) msPeriod,              // rate lsb
			(uint8_t) (msPeriod >> 8),         // rate msb
			0x01,                  // cycles
			0x00, 0x01,                  // timeref 1 = GPS time
			0x00, };
	return sendConfigDataChecksummed(msg, sizeof(msg), 5);
}

int SAM_M8Q_GNSS::setMode(GNSSDynamicsMode mode)
{
	uint8_t dynamicModel;

	switch (mode)
	{
		case GNSSDynamicsMode::PORTABLE:
			dynamicModel = UBLOX_DYN_PORTABLE;
			break;
		case GNSSDynamicsMode::PEDESTRIAN:
			dynamicModel = UBLOX_DYN_PED;
			break;
		case GNSSDynamicsMode::AUTOMOTIVE:
			dynamicModel = UBLOX_DYN_AUTOMOTIVE;
			break;
		case GNSSDynamicsMode::AIRBORNE1G:
			dynamicModel = UBLOX_DYN_AIR1G;
			break;
		case GNSSDynamicsMode::AIRBORNE2G:
		default:
			dynamicModel = UBLOX_DYN_AIR2G;
			break;
		case GNSSDynamicsMode::AIRBORNE4G:
			dynamicModel = UBLOX_DYN_AIR4G;
			break;
	}

	const uint8_t msg[] =
	{
	UBLOX_CFG_CLASS,       // CFG
			UBLOX_CFG_NAV5,        // NAV5 mode
			0x24,                  // length lsb - 36 bytes
			0x00,                  // length msb
			0b0000101,             // mask LSB (fixMode, dyn)
			0x00,                  // mask MSB (reserved)
			dynamicModel,             // dynamic model (7 - airborne < 2g)
			0x02,                  // fixmode (2 - 3D only)
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,       // padded with 32 zeros
			};
	return sendConfigDataChecksummed(msg, sizeof(msg), 5);
}

int SAM_M8Q_GNSS::setTimepulse()
{
	const uint8_t TP_POLARITY = 1;
	const uint32_t int_us = 1000000;
	const uint32_t len_us = 100000;

	const uint8_t msg[] =
	{
	UBLOX_CFG_CLASS,       // CFG
			UBLOX_CFG_TP,          // TP
			0x14,                  // length lsb
			0x00,                  // length msb
			int_us & 0xff, int_us >> 8 & 0xff,   // interval (us)
			int_us >> 16 & 0xff, int_us >> 24 & 0xff, len_us & 0xff, len_us >> 8 & 0xff,     // length (us)
			len_us >> 16 & 0xff, len_us >> 24 & 0xff, TP_POLARITY,           // polarity - zero off
			0x01,                  // 1 - GPS time
			0x00,                  // bitmask
			0x00,                  // reserved
			0x00, 0x00,            // antenna delay
			0x00, 0x00,            // rf group delay
			0, 0, 0, 0             // user delay
			};
	return sendConfigDataChecksummed(msg, sizeof(msg), 5);
}

int SAM_M8Q_GNSS::setSbas(GNSSSbasConstellation sbas)
{
	bool enable = sbas != GNSSSbasConstellation::NONE;

	uint32_t sv_mask;

	switch (sbas)
	{
		case GNSSSbasConstellation::WAAS:
			sv_mask = UBLOX_SBAS_WAAS;
			break;
		case GNSSSbasConstellation::EGNOS:
			sv_mask = UBLOX_SBAS_EGNOS;
			break;
		case GNSSSbasConstellation::MSAS:
			sv_mask = UBLOX_SBAS_MSAS;
			break;
		case GNSSSbasConstellation::GAGAN:
			sv_mask = UBLOX_SBAS_GAGAN;
			break;
		case GNSSSbasConstellation::ALL:
		case GNSSSbasConstellation::NONE:
		default:
			sv_mask = UBLOX_SBAS_AUTO;
			break;
	}

	const uint8_t msg[] =
	{
	UBLOX_CFG_CLASS, // CFG
			UBLOX_CFG_SBAS,  // SBAS
			0x08,            // length lsb
			0x00,            // length msb
			enable,          // enable flag
			0b011,           // mode
			3,               // # SBAS tracking channels
			0, (uint8_t) sv_mask, (uint8_t) (sv_mask >> 8), (uint8_t) (sv_mask >> 16), (uint8_t) (sv_mask >> 24), };
	return sendConfigDataChecksummed(msg, sizeof(msg), 5);
}

int SAM_M8Q_GNSS::pollVersion()
{
	const uint8_t msg[] =
	{ UBLOX_MON_CLASS, UBLOX_MON_VER, 0x00, 0x00 };
	return sendConfigDataChecksummed(msg, sizeof(msg), 5);
}

int SAM_M8Q_GNSS::setConstellation(GNSSConstellation constellation, GNSSSbasConstellation sbas)
{
	// Do all this stuff upfront so we don't need to use ternary everywhere.
	uint8_t sbas_chan = 3;
	bool enable_gps = true;
	bool enable_glonass = false;

	uint8_t max_glonass_ch = 0;
	uint8_t max_gps_ch = 16;
	uint8_t min_gps_ch = 10;    // We have 32 channels, so always reserving
								// at least 10 for GPS is no big deal (if
								// enabled)

	bool enable_sbas = (sbas != GNSSSbasConstellation::NONE);

	// Don't save channels for SBAS when we're not using it.
	if (!enable_sbas)
	{
		sbas_chan = 0;
	}

	switch (constellation)
	{
		case GNSSConstellation::GLONASS:
			enable_gps = false; // fall through
		case GNSSConstellation::ALL:
			enable_glonass = true;
			break;
		case GNSSConstellation::GPS:
		default:
			// Nothing to see here.  Defaults are good.
			break;
	}

	if (enable_glonass)
	{
		max_glonass_ch = 16;
	}

	if (!enable_gps)
	{
		max_gps_ch = 0;
		min_gps_ch = 0;
	}

	const uint8_t msg[] =
	{
	UBLOX_CFG_CLASS, // CFG
			UBLOX_CFG_GNSS,  // GNSS
			28,              // length lsb   4 + 8 x 3 = 28
			0,               // length msb
			0,               // msgver = 0
			0,               // numTrkChHw (ro)
			32,              // numTrkChUse
			3,               // 3 config blocks here

			UBLOX_GNSSID_GPS,               // Configuration for GPS
			min_gps_ch,      // Minimum number of channels to reserve.
			max_gps_ch,      // maximum number of channels used
			0,               // reserved1
			enable_gps,      // flags, 1 here means enable
			0, enable_gps,      // flags, sigcfgmask, 1 sane for all sat systems
			enable_gps,      // UNKNOWN-- but ucenter sets it?

			UBLOX_GNSSID_SBAS, // This chunk is always for SBAS/DGPS
			sbas_chan,       // How many to save for SBAS?
			sbas_chan,       // Maximum sbas channels
			0,               // reserved1
			enable_sbas,     // flags, 1 here means enable
			0, enable_sbas,     // flags, sigcfgmask, SBAS L1CA
			enable_sbas,

			// If this next one is used, it's always GLONASS.
			UBLOX_GNSSID_GLONASS,// GLONASS.
			0,               // Minimum num channels reserved
			max_glonass_ch,  // Maximum sbas channels
			0,               // reserved1
			enable_glonass,  // flags, 1 here means enable
			0, enable_glonass,  // flags, sigcfgmask, GLONASS L1OF
			enable_glonass };
	return sendConfigDataChecksummed(msg, sizeof(msg), 5);
}

int SAM_M8Q_GNSS::clearConfig()
{
	// Reset the messges and navigation settings
	const uint32_t MASK = 0b00001110;
	const uint8_t msg[] =
	{
	UBLOX_CFG_CLASS, // CFG
			UBLOX_CFG_CFG,   // CFG-CFG
			0x0C,            // length lsb
			0x00,            // length msb
			MASK,            // clear mask (U4)
			MASK >> 8, MASK >> 16, MASK >> 24, 0, 0, 0, 0,         // load mask
			0, 0, 0, 0          // save mask
			};

	return sendConfigDataChecksummed(msg, sizeof(msg), 5);
}
