#include <Channels/RocketChannel.h>

RocketChannel::RocketChannel(uint8_t id, const ADCChannel &oxPressureChannel, const ADCChannel &fuelPressureChannel, const ADCChannel &chamberPressureChannel, ServoChannel &oxServoChannel, ServoChannel &fuelServoChannel, PyroChannel &igniter0Channel, PyroChannel &igniter1Channel,
		uint32_t refreshDivider) :
		AbstractChannel(CHANNEL_TYPE_ROCKET, id, refreshDivider),
		oxPressureChannel(oxPressureChannel),
		fuelPressureChannel(fuelPressureChannel),
		chamberPressureChannel(chamberPressureChannel),
		oxServoChannel(oxServoChannel),
		fuelServoChannel(fuelServoChannel),
		igniter0Channel(igniter0Channel),
		igniter1Channel(igniter1Channel),
		state(PAD_IDLE),
		ignitionState(IgnitionSequence::INIT),
		can(Can::instance(0))
{
}

int RocketChannel::init()
{
	return 0;
}

int RocketChannel::exec()
{
	uint64_t time = STRHAL_Systick_GetTick();
	if ((time - timeLastSample) < EXEC_SAMPLE_TICKS)
		return 0;

	timeLastSample = time;

	// Current State Logic - executes state logic, also returns new state if transition conditions are met
	internalNextState = currentStateLogic(time);

	ROCKET_STATE nextState = state;

	if (externalNextState != state)
	{ // Prioritize external event - there has to be some kind of priority, because internal could be different to external -> external means CAN -> either Sequence or Abort
		nextState = externalNextState;
	}
	else if (internalNextState != state)
	{
		externalNextState = internalNextState; // Incase an internal state change happens, the external state, which is from some previous change would block it, so it is updated here
		nextState = internalNextState;
	}

	// Next State Logic
	if (nextState != state)
	{
		nextStateLogic(nextState, time);
	}

	return 0;
}

ROCKET_STATE RocketChannel::currentStateLogic(uint64_t time)
{
	switch (state)
	{
		case PAD_IDLE:
			// wait until auto sequence command from CAN
			break;
		case AUTO_CHECK:
			return autoCheck(time);
		case IGNITION_SEQUENCE:
			return ignitionSequence(time);
		case HOLD_DOWN:
			return holddown(time);
		case POWERED_ASCENT:
			return poweredAscent(time);
		case UNPOWERED_ASCENT:
			// wait until end of flight command from PMU2
			if (time - timeLastTransition > 10000)
			{
				return DEPRESS;
			}
			break;
		case DEPRESS:
			return depress(time);
		case ABORT:
			return abort(time);
		default:
			break;
	}
	return state;
}

void RocketChannel::nextStateLogic(ROCKET_STATE nextState, uint64_t time)
{
	timeLastTransition = time;
	switch (nextState)
	{
		case PAD_IDLE:
			break;
		case AUTO_CHECK:
			break;
		case IGNITION_SEQUENCE:
			break;
		case HOLD_DOWN:
			break;
		case POWERED_ASCENT:
		{
			SetMsg_t setMsg =
			{ 0 };
			setMsg.variable_id = 1; // servo target position
			setMsg.value = 65000; // open servo
			can.sendAsMaster(9, 11, 4, (uint8_t*) &setMsg, 5 + sizeof(uint32_t)); // send REQ_SET_VARIABLE (4) command to holddown servo (channelId 11) on oxcart node (nodeId 9)*/
			/*SetMsg_t setMsg =
			 { 0 };
			 setMsg.variable_id = 0; // pyro state
			 setMsg.value = 65000; // enable pyro
			 cancom->sendAsMaster(7, 12, 4, (uint8_t *) &setMsg, 5+sizeof(uint32_t)); // send REQ_SET_VARIABLE (4) command to pmu pyro (channelId 12) on oxcart node (nodeId 7)
			 */
			break;
		}
		case UNPOWERED_ASCENT:
			//fuelServoChannel.setTargetPos(0);
			oxServoChannel.setTargetPos(0);
			break;
		case DEPRESS:
			if (chamberPressureChannel.getMeasurement() > chamberPressureMin)
				return; // do not set next state to DEPRESS if there is still combustion going on

			fuelServoChannel.setTargetPos(65000);
			oxServoChannel.setTargetPos(65000);
			break;
		case ABORT:
			fuelServoChannel.setTargetPos(0);
			oxServoChannel.setTargetPos(0);
			(void) igniter0Channel.setState(0);
			(void) igniter1Channel.setState(0);
			break;
		default:
			break;
	}
	state = nextState;
	return;
}

ROCKET_STATE RocketChannel::autoCheck(uint64_t time)
{
	if (time - timeLastTransition > 2000)
	{
		return PAD_IDLE;
	}
	// TODO check Holddown
	if (igniter0Channel.getContinuity() == 1 || //no continuity
			igniter1Channel.getContinuity() == 1 || //no continuity
			oxPressureChannel.getMeasurement() < oxPressureMin || fuelPressureChannel.getMeasurement() < fuelPressureMin)
	{

		autoCheckBadCounter++;
	}
	else
	{
		autoCheckBadCounter = 0;
	}

	if (autoCheckBadCounter > AUTO_CHECK_BAD_COUNT_MAX)
		return ABORT;

	return AUTO_CHECK;
}

ROCKET_STATE RocketChannel::ignitionSequence(uint64_t time)
{
	switch (ignitionState)
	{
		case IgnitionSequence::INIT: // Start of Ignition Sequence
			if (time - timeLastTransition >= 0)
			{
				fuelServoChannel.setTargetPos(0);
				oxServoChannel.setTargetPos(0);
				(void) igniter0Channel.setState(0);
				(void) igniter1Channel.setState(0);
				ignitionState = IgnitionSequence::IGNITION0_ON;
			}
			break;
		case IgnitionSequence::IGNITION0_ON: // T-2.5 - Ignition
			if (time - timeLastTransition > 7500)
			{
				(void) igniter0Channel.setState(65000);
				ignitionState = IgnitionSequence::IGNITION1_ON;
			}
			break;
		case IgnitionSequence::IGNITION1_ON: // T-1.5 - Ignition
			if (time - timeLastTransition > 8500)
			{
				(void) igniter1Channel.setState(65000);
				ignitionState = IgnitionSequence::T_0;
			}
			break;
		case IgnitionSequence::T_0: // T - Valves to 0
			if (time - timeLastTransition > 10000)
			{
				fuelServoChannel.setTargetPos(19000);
				oxServoChannel.setTargetPos(30000);
				ignitionState = IgnitionSequence::VALVES_SLOWLY_OPEN;
			}
			break;
		case IgnitionSequence::VALVES_SLOWLY_OPEN: // T+0.5 -> T+1.2 Slowly move valves across the opening point (fuel: 22000, ox: 33000)
			if (time - timeLastTransition > 10500)
			{
				fuelServoChannel.moveToPosInInterval(32000, 700);
				oxServoChannel.moveToPosInInterval(43000, 700);
				ignitionState = IgnitionSequence::VALVES_FULLY_OPEN;
			}
			break;
		case IgnitionSequence::VALVES_FULLY_OPEN: // T+1.2 -> T+1.7 Valves to 100% open
			if (time - timeLastTransition > 11200)
			{
				fuelServoChannel.moveToPosInInterval(65000, 500);
				oxServoChannel.moveToPosInInterval(65000, 500);
				ignitionState = IgnitionSequence::IGNITION_OFF;
			}
			break;
		case IgnitionSequence::IGNITION_OFF: // T+1.5 - Ignition off
			if (time - timeLastTransition > 11500)
			{
				(void) igniter0Channel.setState(0);
				(void) igniter1Channel.setState(0);
				return HOLD_DOWN;
			}
			break;
	}

	return IGNITION_SEQUENCE;

}

ROCKET_STATE RocketChannel::holddown(uint64_t time)
{
	if (holdDownTimeout > 0)
	{
		if (time - timeLastTransition > holdDownTimeout)
		{ // release after x s to lessen apogee
			if (chamberPressureChannel.getMeasurement() < chamberPressureMin)
			{ // if holddown timeout has passed, still check for chamber pressure
				chamberPressureGoodCounter = 0;
				chamberPressureLowCounter++;
			}
			else
			{
				chamberPressureLowCounter = 0;
				chamberPressureGoodCounter++;
			}

			// if either event (low or good pressure) occurs exclusively for a specified amount of times -> abort (low)/release(good)
			if (chamberPressureLowCounter > CHAMBER_PRESSURE_LOW_COUNT_MAX)
			{
				//return ABORT; do not abort in test environment
			}

			if (chamberPressureGoodCounter > CHAMBER_PRESSURE_GOOD_COUNT_MIN)
			{
				//TODO calibrate holddown servo
				return POWERED_ASCENT;
			}
		}
		else
		{
			chamberPressureGoodCounter = 0;
			chamberPressureLowCounter = 0;
		}
	}
	else
	{
		if (chamberPressureChannel.getMeasurement() >= chamberPressureMin)
		{
			return POWERED_ASCENT;
		}
	}
	return HOLD_DOWN;

}

ROCKET_STATE RocketChannel::poweredAscent(uint64_t time)
{
	if (time - timeLastTransition > 3500)
	{ // motor burnout, close valves, IMPORTANT!: total burn time before shutoff is powered + unpowered ascent
		return UNPOWERED_ASCENT;
	}
	return POWERED_ASCENT;
}

ROCKET_STATE RocketChannel::depress(uint64_t time)
{
	if (oxPressureChannel.getMeasurement() < 10 && fuelPressureChannel.getMeasurement() < 10)
	{ // PMU2 sent end of flight, depress rocket and go to idle state once pressures drop below a minimum
		// TODO: add variable for depress maximum pressures
		return PAD_IDLE;
	}
	return DEPRESS;
}

ROCKET_STATE RocketChannel::abort(uint64_t time)
{
	return ABORT;
}

int RocketChannel::reset()
{
	externalNextState = PAD_IDLE;
	ignitionState = IgnitionSequence::INIT;
	return 0;
}

int RocketChannel::processMessage(uint8_t commandId, uint8_t *returnData, uint8_t &n)
{
	switch (commandId)
	{
		case ROCKET_REQ_INTERNAL_CONTROL:
			if (state == PAD_IDLE)
			{
				externalNextState = IGNITION_SEQUENCE;
			}
			return 0;
		case ROCKET_REQ_ABORT:
			externalNextState = ABORT;
			return 0;
		case ROCKET_REQ_END_OF_FLIGHT:
			if (state == UNPOWERED_ASCENT)
			{
				externalNextState = DEPRESS;
			}
			return 0;
		case ROCKET_REQ_SET_ROCKET_STATE:
			setRocketState(returnData, n);
			return 0;
		case ROCKET_REQ_GET_ROCKET_STATE:
			getRocketState(returnData, n);
			return 0;
		case ROCKET_REQ_AUTO_CHECK:
			if (state == PAD_IDLE)
			{
				externalNextState = AUTO_CHECK;
			}
			return 0;
		default:
			return AbstractChannel::processMessage(commandId, returnData, n);
	}
}

int RocketChannel::getSensorData(uint8_t *data, uint8_t &n)
{
	uint16_t *out = (uint16_t*) (data + n);
	*out = state;

	n += ROCKET_DATA_N_BYTES;
	return 0;
}

int RocketChannel::setVariable(uint8_t variableId, int32_t data)
{
	switch (variableId)
	{
		case ROCKET_STATE_REFRESH_DIVIDER:
			refreshDivider = data;
			refreshCounter = 0;
			return 0;
		case ROCKET_MINIMUM_CHAMBER_PRESSURE:
			chamberPressureMin = data * 4095 / UINT16_MAX; // convert from 16 to 12bit scale
			return 0;
		case ROCKET_MINIMUM_FUEL_PRESSURE:
			fuelPressureMin = data * 4095 / UINT16_MAX;
			return 0;
		case ROCKET_MINIMUM_OX_PRESSURE:
			oxPressureMin = data * 4095 / UINT16_MAX;
			return 0;
		case ROCKET_HOLDDOWN_TIMEOUT:
			holdDownTimeout = data;
			return 0;
		default:
			return -1;
	}
}

int RocketChannel::getVariable(uint8_t variableId, int32_t &data) const
{
	switch (variableId)
	{
		case ROCKET_STATE_REFRESH_DIVIDER:
			data = (int32_t) refreshDivider;
			return 0;
		case ROCKET_MINIMUM_CHAMBER_PRESSURE:
			data = (int32_t) chamberPressureMin * UINT16_MAX / 4095;
			return 0;
		case ROCKET_MINIMUM_FUEL_PRESSURE:
			data = (int32_t) fuelPressureMin * UINT16_MAX / 4095;
			return 0;
		case ROCKET_MINIMUM_OX_PRESSURE:
			data = (int32_t) oxPressureMin * UINT16_MAX / 4095;
			return 0;
		case ROCKET_HOLDDOWN_TIMEOUT:
			data = (int32_t) holdDownTimeout;
			return 0;
		default:
			return -1;
	}
}

void RocketChannel::setRocketState(uint8_t *data, uint8_t &n)
{
	RocketStateReqMsg_t *rocketStateRequestMsg;
	RocketStateResMsg_t *rocketStateResponseMsg;
	rocketStateRequestMsg = (RocketStateReqMsg_t*) data;
	externalNextState = static_cast<ROCKET_STATE>(rocketStateRequestMsg->state);

	rocketStateResponseMsg = (RocketStateResMsg_t*) data;
	rocketStateResponseMsg->state = state;
	rocketStateResponseMsg->status = SUCCESS;
	n += sizeof(RocketStateResMsg_t);
}

void RocketChannel::getRocketState(uint8_t *data, uint8_t &n)
{
	RocketStateResMsg_t *rocketStateResponseMsg;

	rocketStateResponseMsg = (RocketStateResMsg_t*) data;
	rocketStateResponseMsg->state = state;
	rocketStateResponseMsg->status = WRITABLE;
	n += sizeof(RocketStateResMsg_t);
}
