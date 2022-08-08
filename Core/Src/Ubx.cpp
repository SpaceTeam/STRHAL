#include "../Inc/Ubx.h"

UbxProtocollState Ubx::proto_state = UbxProtocollState::START;
uint16_t Ubx::rx_count = 0;
uint32_t Ubx::parse_errors = 0;
MsgTracker Ubx::msgtracker =
{ 0 };

int Ubx::parseStream(uint8_t c, char *gps_rx_buffer, GPSPositionData *GpsData, struct GPS_RX_STATS *gpsRxStats, uint32_t *receivedID)
{
	struct UBXPacket *ubx = (struct UBXPacket*) gps_rx_buffer;

	int ackReceived = 0;

	switch (proto_state)
	{
		case UbxProtocollState::START: // detect protocol
			if (c == UBX_SYNC1)   // first UBX sync char found
			{
				proto_state = UbxProtocollState::UBX_SY2;
			}
			break;
		case UbxProtocollState::UBX_SY2:
			if (c == UBX_SYNC2)  // second UBX sync char found
			{
				proto_state = UbxProtocollState::UBX_CLASS;
			}
			else
			{
				proto_state = UbxProtocollState::START; // reset state
			}
			break;
		case UbxProtocollState::UBX_CLASS:
			ubx->header.classId = c;
			proto_state = UbxProtocollState::UBX_ID;
			break;
		case UbxProtocollState::UBX_ID:
			ubx->header.id = c;
			proto_state = UbxProtocollState::UBX_LEN1;
			break;
		case UbxProtocollState::UBX_LEN1:
			ubx->header.len = c;
			proto_state = UbxProtocollState::UBX_LEN2;
			break;
		case UbxProtocollState::UBX_LEN2:
			ubx->header.len += (c << 8);
			if (ubx->header.len > sizeof(UBXPayload))
			{
				gpsRxStats->gpsRxOverflow++;
				proto_state = UbxProtocollState::START;
			}
			else
			{
				rx_count = 0;
				proto_state = UbxProtocollState::UBX_PAYLOAD;
			}
			break;
		case UbxProtocollState::UBX_PAYLOAD:
			if (rx_count < ubx->header.len)
			{
				ubx->payload.payload[rx_count] = c;
				if (++rx_count == ubx->header.len)
				{
					proto_state = UbxProtocollState::UBX_CHK1;
				}
			}
			else
			{
				gpsRxStats->gpsRxOverflow++;
				proto_state = UbxProtocollState::START;
			}
			break;
		case UbxProtocollState::UBX_CHK1:
			ubx->header.ck_a = c;
			proto_state = UbxProtocollState::UBX_CHK2;
			break;
		case UbxProtocollState::UBX_CHK2:
			ubx->header.ck_b = c;
			if (checksumMessage(ubx))    // message complete and valid
			{
				uint32_t id = parseMessage(ubx, GpsData, &ackReceived);
				if (receivedID != 0)
				{
					*receivedID = id;
				}
				proto_state = UbxProtocollState::FINISHED;
			}
			else
			{
				gpsRxStats->gpsRxChkSumError++;
				proto_state = UbxProtocollState::START;
			}
			break;
		default:
			break;
	}

	if (proto_state == UbxProtocollState::START)
	{
		return PARSER_ERROR;    // parser couldn't use this byte
	}
	else if (proto_state == UbxProtocollState::FINISHED)
	{
		gpsRxStats->gpsRxReceived++;
		proto_state = UbxProtocollState::START;

		switch (ackReceived)
		{
			case 0x01:
				// ACK Received
				return PARSER_COMPLETE_ACK;
			case 0x00:
				// NAK Received
				return PARSER_COMPLETE_NAK;
			default:
				return PARSER_COMPLETE;    // message complete & processed
		}
	}

	return PARSER_INCOMPLETE; // message not (yet) complete
}

uint32_t Ubx::parseMessage(const struct UBXPacket *ubx, GPSPositionData *GpsPosition, int *receivedack)
{
	uint32_t id = ubx->header.id;

	if (receivedack)
	{
		*receivedack = -1;
	}

	switch (ubx->header.classId)
	{
		case UBX_CLASS_NAV:
			switch (ubx->header.id)
			{
				case UBX_ID_POSLLH:
					parseNavPosllh(&ubx->payload.nav_posllh, GpsPosition);
					break;
				case UBX_ID_DOP:
					parseNavDop(&ubx->payload.nav_dop, GpsPosition);
					break;
				case UBX_ID_SOL:
					parseNavSol(&ubx->payload.nav_sol, GpsPosition);
					break;
				case UBX_ID_VELNED:
					parseNavVelned(&ubx->payload.nav_velned, GpsPosition);
					break;
				case UBX_ID_TIMEUTC:
					parseNavTimeutc(&ubx->payload.nav_timeutc);
					break;
				case UBX_ID_SVINFO:
					parseNavSvinfo(&ubx->payload.nav_svinfo);
					break;
			}
			break;
		case UBX_CLASS_MON:
			switch (ubx->header.id)
			{
				case UBX_ID_MONVER:
					parseMonVer(&ubx->payload.mon_ver);
					break;
			}
			break;
		case UBX_CLASS_ACK_NAK:
			if (receivedack)
			{
				*receivedack = ubx->header.id;
			}
			break;
	}
	return id;
}

void Ubx::parseNavPosllh(const struct UBX_NAV_POSLLH *posllh, GPSPositionData *GpsPosition)
{
	if (checkMsgtracker(posllh->iTOW, POSLLH_RECEIVED))
	{
		if (GpsPosition->Status != GPSPOSITION_STATUS_NOFIX)
		{
			GpsPosition->Altitude = (float) posllh->hMSL * 0.001f;
			GpsPosition->GeoidSeparation = (float) (posllh->height - posllh->hMSL) * 0.001f;
			GpsPosition->Latitude = posllh->lat;
			GpsPosition->Longitude = posllh->lon;
			GpsPosition->TimeOfWeek = posllh->iTOW;
		}
	}
}

void Ubx::parseNavSol(const struct UBX_NAV_SOL *sol, GPSPositionData *GpsPosition)
{
	if (checkMsgtracker(sol->iTOW, SOL_RECEIVED))
	{
		GpsPosition->Satellites = sol->numSV;
		GpsPosition->Accuracy = sol->pAcc / 100.0f;

		if (sol->flags & STATUS_FLAGS_GPSFIX_OK)
		{
			switch (sol->gpsFix)
			{
				case STATUS_GPSFIX_2DFIX:
					GpsPosition->Status = GPSPOSITION_STATUS_FIX2D;
					break;
				case STATUS_GPSFIX_3DFIX:
					GpsPosition->Status = (sol->flags & STATUS_FLAGS_DIFFSOLN) ? GPSPOSITION_STATUS_DIFF3D : GPSPOSITION_STATUS_FIX3D;
					break;
				default:
					GpsPosition->Status = GPSPOSITION_STATUS_NOFIX;
			}
		}
		else
		{ // fix is not valid so we make sure to treat is as NOFIX
			GpsPosition->Status = GPSPOSITION_STATUS_NOFIX;
		}
	}
}

void Ubx::parseNavDop(const struct UBX_NAV_DOP *dop, GPSPositionData *GpsPosition)
{
	if (checkMsgtracker(dop->iTOW, DOP_RECEIVED))
	{
		GpsPosition->HDOP = (float) dop->hDOP * 0.01f;
		GpsPosition->VDOP = (float) dop->vDOP * 0.01f;
		GpsPosition->PDOP = (float) dop->pDOP * 0.01f;
	}
}

void Ubx::parseNavVelned(const struct UBX_NAV_VELNED *velned, GPSPositionData *GpsPosition)
{
	if (checkMsgtracker(velned->iTOW, VELNED_RECEIVED))
	{
		if (GpsPosition->Status != GPSPOSITION_STATUS_NOFIX)
		{
			// gps velocity unused
			/*GpsVelocity.North = (float) velned->velN / 100.0f;
			 GpsVelocity.East = (float) velned->velE / 100.0f;
			 GpsVelocity.Down = (float) velned->velD / 100.0f;
			 GpsVelocity.Accuracy = (float) velned->sAcc / 100.0f;*/
			GpsPosition->Groundspeed = (float) velned->gSpeed * 0.01f;
			GpsPosition->Heading = (float) velned->heading * 1.0e-5f;
		}
	}
}

void Ubx::parseNavTimeutc(const struct UBX_NAV_TIMEUTC *timeutc)
{
	if (!(timeutc->valid & TIMEUTC_VALIDWKN))
	{
		return;
	}

	GPSTimeData GpsTime;

	GpsTime.Year = timeutc->year;
	GpsTime.Month = timeutc->month;
	GpsTime.Day = timeutc->day;
	GpsTime.Hour = timeutc->hour;
	GpsTime.Minute = timeutc->min;
	GpsTime.Second = timeutc->sec;
	//GPSTimeCallback(&GpsTime);
	(void) GpsTime;
}

void Ubx::parseNavSvinfo(const struct UBX_NAV_SVINFO *svinfo)
{
	// Sv info unused
	/*uint8_t chan = 0;
	 GPSSatellitesData svdata;

	 bool skipped = false;

	 svdata.SatsInView = svinfo->numCh;

	 // Invalid.. too many channels to fit in message.
	 if(svinfo->numCh > MAX_SVS) {
	 return;
	 }

	 for(int i = 0;(i < svinfo->numCh) && (chan < GPSSATELLITES_PRN_NUMELEM);i++) {
	 // Prioritize putting info on satellites we're actually
	 // receiving first
	 if(!svinfo->sv[i].cno) {
	 skipped = true;
	 continue;
	 }

	 svdata.Azimuth[chan] = svinfo->sv[i].azim;
	 svdata.Elevation[chan] = svinfo->sv[i].elev;
	 svdata.PRN[chan] = svinfo->sv[i].svid;

	 // Scale to the range used by NMEA GPSes.. a cno >50 is
	 // extremely improbable.  This makes the display in GCS more
	 //consistent between NMEA and UBX GPSes.
	 if(svinfo->sv[chan].cno < 66) {
	 svdata.SNR[chan] = svinfo->sv[i].cno * 3 / 2;
	 } else {
	 svdata.SNR[chan] = 99;
	 }

	 chan++;
	 }

	 // Now fill any available slots with satellites we're not receiving.
	 if(skipped) {
	 for(int i = 0; (i < svinfo->numCh) && (chan < GPSSATELLITES_PRN_NUMELEM); i++) {
	 if(!svinfo->sv[i].cno) {
	 svdata.Azimuth[chan] = svinfo->sv[i].azim;
	 svdata.Elevation[chan] = svinfo->sv[i].elev;
	 svdata.PRN[chan] = svinfo->sv[i].svid;
	 svdata.SNR[chan] = svinfo->sv[i].cno;
	 chan++;
	 }
	 }
	 }

	 // fill remaining slots (if any)
	 for(; chan < GPSSATELLITES_PRN_NUMELEM; chan++) {
	 svdata.Azimuth[chan] = 0;
	 svdata.Elevation[chan] = 0;
	 svdata.PRN[chan] = 0;
	 svdata.SNR[chan] = 0;
	 }

	 // GPSSatellitesSet(&svdata);
	 */
}

void Ubx::parseMonVer(const struct UBX_MON_VER *version_info)
{
	(void) version_info;
}

// Check if a message belongs to the current data set and register it as 'received'
bool Ubx::checkMsgtracker(uint32_t tow, uint8_t msg_flag)
{
	if (tow > msgtracker.currentTOW ? true                   // start of a new message set
			: (msgtracker.currentTOW - tow > 6 * 24 * 3600 * 1000)) // 6 days, TOW wrap around occured
	{
		msgtracker.currentTOW = tow;
		msgtracker.msg_received = NONE_RECEIVED;
	}
	else if (tow < msgtracker.currentTOW)       // message outdated (don't process)
	{
		return false;
	}

	msgtracker.msg_received |= msg_flag; // register reception of this msg type
	return true;
}

bool Ubx::checksumMessage(const struct UBXPacket *ubx)
{
	int i;
	uint8_t ck_a, ck_b;

	ck_a = ubx->header.classId;
	ck_b = ck_a;

	ck_a += ubx->header.id;
	ck_b += ck_a;

	ck_a += ubx->header.len & 0xff;
	ck_b += ck_a;

	ck_a += ubx->header.len >> 8;
	ck_b += ck_a;

	for (i = 0; i < ubx->header.len; i++)
	{
		ck_a += ubx->payload.payload[i];
		ck_b += ck_a;
	}

	if (ubx->header.ck_a == ck_a && ubx->header.ck_b == ck_b)
	{
		return true;
	}
	else
	{
		parse_errors++;
		// UBloxInfoParseErrorsSet(&parse_errors);

		return false;
	}
}
