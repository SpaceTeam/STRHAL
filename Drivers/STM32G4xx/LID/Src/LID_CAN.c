#include "../Inc/LID_CAN.h"
#include <string.h>

static const uint8_t Can_DlcToLength[] =
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };

static const uint32_t Can_LengthToDlc[] = {
	0,    //  0
	1,    //  1
	2,    //  2
	3,    //  3
	4,    //  4
	5,    //  5
	6,    //  6
	7,    //  7
	8,    //  8
	9,    //  9
	9,    // 10
	9,    // 11
	9,    // 12
	10,   // 13
	10,   // 14
	10,   // 15
	10,   // 16
	11,   // 17
	11,   // 18
	11,   // 19
	11,   // 20
	12,   // 21
	12,   // 22
	12,   // 23
	12,   // 24
	13,   // 25
	13,   // 26
	13,   // 27
	13,   // 28
	13,   // 29
	13,   // 30
	13,   // 31
	13,   // 32
	14,   // 33
	14,   // 34
	14,   // 35
	14,   // 36
	14,   // 37
	14,   // 38
	14,   // 39
	14,   // 40
	14,   // 41
	14,   // 42
	14,   // 43
	14,   // 44
	14,   // 45
	14,   // 46
	14,   // 47
	14,   // 48
	15,   // 49
	15,   // 50
	15,   // 51
	15,   // 52
	15,   // 53
	15,   // 54
	15,   // 55
	15,   // 56
	15,   // 57
	15,   // 58
	15,   // 59
	15,   // 60
	15,   // 61
	15,   // 62
	15,   // 63
	15,   // 64
};

typedef enum {
	LID_CAN_STATE_0,
	LID_CAN_STATE_INITIALISING,
	LID_CAN_STATE_RUNNING
} LID_CAN_State_t;

typedef struct {
	FDCAN_GlobalTypeDef *can;
	Can_Message_RAM *can_ram;
	//LID_CAN_Filters_t filters; TODO: Add filter, noob
	volatile LID_CAN_State_t state;
} LID_CAN_Handle_t;

static LID_CAN_Handle_t _fdcans[2] = {
	[LID_FDCAN1] = { FDCAN1, FDCAN1_MESSAGE_RAM, LID_CAN_STATE_0},
	[LID_FDCAN2] = { FDCAN2, FDCAN2_MESSAGE_RAM, LID_CAN_STATE_0}
};

static void LID_CAN_ClockCalibration(void)
{
	/* Bypass clock calibration */
	//SET_BIT(FDCAN_CCU->CCFG, FDCANCCU_CCFG_BCC);

	/* Configure clock divider */
	//MODIFY_REG(FDCAN_CCU->CCFG, FDCANCCU_CCFG_CDIV, FDCAN_CLOCK_DIV1);
	MODIFY_REG(FDCAN_CONFIG->CKDIV, FDCAN_CKDIV_PDIV, FDCAN_CLOCK_DIV2);
}

static void LID_CAN_Init_GPIO(void)
{

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PCLK1);

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_FDCAN);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

	//FDCAN1 GPIO Configuration
	//PA11     ------> FDCAN1_RX
	//PA12     ------> FDCAN1_TX

	GPIO_InitStruct.Pin = LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_9;

	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//FDCAN2 GPIO Configuration
	//PB5     ------> FDCAN2_RX
	//PB6     ------> FDCAN2_TX

	GPIO_InitStruct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_6;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_9;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/*
static void LID_CAN_AddStdFilter(uint32_t can_handle_index, uint32_t filter_index, uint32_t mask, uint32_t filter_id, uint32_t target_location)
{
	Can_Message_RAM *can_ram = _fdcans[can_handle_index].can_ram;

	can_ram->std_filters[filter_index].S0.bit.SFEC = target_location; // e.g. FDCAN_FILTER_TO_RXFIFO0
	can_ram->std_filters[filter_index].S0.bit.SFID1 = filter_id;
	can_ram->std_filters[filter_index].S0.bit.SFID2 = mask;
	can_ram->std_filters[filter_index].S0.bit.SFT = FDCAN_FILTER_MASK;
}
*/

int LID_CAN_Instance_Init(LID_FDCAN_Id_t fdcan_id) {
	if(fdcan_id < 0 || fdcan_id >= LID_N_FDCAN)
		return -1;

	_fdcans[fdcan_id].state = LID_CAN_STATE_INITIALISING;

	FDCAN_GlobalTypeDef *can = _fdcans[fdcan_id].can;
	//Can_Message_RAM *can_ram = handles[can_handle_index].can_ram;
	LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PCLK1);

	CLEAR_BIT(can->CCCR, FDCAN_CCCR_CSR);

	uint32_t tot = 0;

	// Check Sleep mode acknowledge
	while ((can->CCCR & FDCAN_CCCR_CSA) == FDCAN_CCCR_CSA) {
		tot++;
		if (tot > LID_CAN_START_TOT)
			return -1;
	}

	SET_BIT(can->CCCR, FDCAN_CCCR_INIT);

	// Wait until the INIT bit into CCCR register is set
	while ((can->CCCR & FDCAN_CCCR_INIT) == 0U) {
		tot++;
		if (tot > LID_CAN_START_TOT)
			return -1;
	}

	// Enable configuration change
	SET_BIT(can->CCCR, FDCAN_CCCR_CCE);
	// Wait until the CCE bit into CCCR register is set
	while ((can->CCCR & FDCAN_CCCR_CCE) == 0U) {
		tot++;
		if (tot > LID_CAN_START_TOT)
			return -1;
	}

	//CLEAR_BIT(can->CCCR, FDCAN_CCCR_DAR);  //AutoRetransmission Enabled
	SET_BIT(can->CCCR, FDCAN_CCCR_DAR);  //AutoRetransmission Disabled

	//SET_BIT(can->CCCR, FDCAN_CCCR_TXP);    //transmit pause Enabled
	CLEAR_BIT(can->CCCR, FDCAN_CCCR_TXP);  //transmit pause Disabled

	//CLEAR_BIT(can->CCCR, FDCAN_CCCR_PXHD); //Protocol Exception Handling Enabled
	SET_BIT(can->CCCR, FDCAN_CCCR_PXHD); //Protocol Exception Handling  Disabled


	//SET_BIT(can->CCCR, FDCAN_CCCR_BRSE);
	//SET_BIT(can->CCCR, FDCAN_CCCR_FDOE);
	SET_BIT(can->CCCR, FDCAN_FRAME_FD_BRS); //FD mode with BitRate Switching

	//CLEAR_BIT(can->CCCR, (FDCAN_CCCR_TEST | FDCAN_CCCR_MON | FDCAN_CCCR_ASM));
	//CLEAR_BIT(can->TEST, FDCAN_TEST_LBCK);

	SET_BIT(can->CCCR, FDCAN_CCCR_TEST);
	SET_BIT(can->TEST, FDCAN_TEST_LBCK);
	CLEAR_BIT(can->CCCR, (FDCAN_CCCR_MON | FDCAN_CCCR_ASM));

	// Set FDCAN Operating Mode:
	//           | Normal | Restricted |    Bus     | Internal | External
	//           |        | Operation  | Monitoring | LoopBack | LoopBack
	// CCCR.TEST |   0    |     0      |     0      |    1     |    1
	// CCCR.MON  |   0    |     0      |     1      |    1     |    0
	// TEST.LBCK |   0    |     0      |     0      |    1     |    1
	// CCCR.ASM  |   0    |     1      |     0      |    0     |    0

	// Set the nominal bit timing register
	can->NBTP = ((((uint32_t) FDCAN_NOMINAL_SYNC_JUMP_WIDTH - 1U) << FDCAN_NBTP_NSJW_Pos) | (((uint32_t) FDCAN_NOMINAL_TIMESEG_1 - 1U) << FDCAN_NBTP_NTSEG1_Pos) | (((uint32_t) FDCAN_NOMINAL_TIMESEG_2 - 1U) << FDCAN_NBTP_NTSEG2_Pos) | (((uint32_t) FDCAN_NOMINAL_PRESCALER - 1U) << FDCAN_NBTP_NBRP_Pos));

	// Bit Rate Switching Enable
	can->DBTP = ((((uint32_t) FDCAN_DATA_SYNC_JUMP_WIDTH - 1U) << FDCAN_DBTP_DSJW_Pos) | (((uint32_t) FDCAN_DATA_TIMESEG_1 - 1U) << FDCAN_DBTP_DTSEG1_Pos) | (((uint32_t) FDCAN_DATA_TIMESEG_2 - 1U) << FDCAN_DBTP_DTSEG2_Pos) | (((uint32_t) FDCAN_DATA_PRESCALER - 1U) << FDCAN_DBTP_DBRP_Pos));

	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_LSS, (LID_CAN_STD_FILTER_NUMBER << FDCAN_RXGFC_LSS_Pos)); // Standard filter elements number
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_LSE, (LID_CAN_EXT_FILTER_NUMBER << FDCAN_RXGFC_LSE_Pos)); // Extended filter elements number
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_F0OM, (FDCAN_RX_FIFO_BLOCKING << FDCAN_RXGFC_F0OM_Pos)); // FIFO 0 operation mode
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_F1OM, (FDCAN_RX_FIFO_BLOCKING << FDCAN_RXGFC_F1OM_Pos)); // FIFO 1 operation mode
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_ANFS, (FDCAN_ACCEPT_IN_RX_FIFO0 << FDCAN_RXGFC_ANFS_Pos)); // Accept Non-matching Frames Standard
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_ANFE, (FDCAN_ACCEPT_IN_RX_FIFO0 << FDCAN_RXGFC_ANFE_Pos)); // Accept Non-matching Frames Extended
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_RRFS, (FDCAN_FILTER_REMOTE << FDCAN_RXGFC_RRFS_Pos)); // Reject Remote Frames Standard
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_RRFE, (FDCAN_REJECT_REMOTE << FDCAN_RXGFC_RRFE_Pos)); // Reject Remote Frames Extended

	CLEAR_BIT(can->TXBC, FDCAN_TXBC_TFQM); // Tx FIFO/Queue Mode

	//Config TxDelayCompensation
	can->TDCR = ((FDCAN_TDC_FILTER << FDCAN_TDCR_TDCF_Pos) | (FDCAN_TDC_OFFSET << FDCAN_TDCR_TDCO_Pos));

	// Enable transmitter delay compensation
	SET_BIT(can->DBTP, FDCAN_DBTP_TDC);

	LL_mDelay(100);

	return 0;
}

LID_Oof_t LID_CAN_Init()
{
	LID_Oof_t status = LID_NOICE;
	LID_CAN_ClockCalibration();
	LID_CAN_Init_GPIO();

	//TODO: Add filter implementation
	/*Can_MessageId_t mask =
	{ 0 };
	mask.info.direction = 0x1;
	mask.info.node_id = 0x1F;
	mask.info.special_cmd = 0x3;
	Can_MessageId_t id =
	{ 0 };
	id.info.direction = MASTER2NODE_DIRECTION;
	id.info.special_cmd = STANDARD_SPECIAL_CMD;
	id.info.node_id = node_id;
	LID_CAN_AddStdFilter(LID_FDCAN1, 0, mask.uint32, id.uint32, FDCAN_FILTER_TO_RXFIFO0);
	id.info.node_id = 0;
	LID_CAN_AddStdFilter(LID_FDCAN1, 1, mask.uint32, id.uint32, FDCAN_FILTER_TO_RXFIFO0);
	*/

	return status;
}

uint8_t LID_CAN_Receive(LID_FDCAN_Id_t fdcan_id, LID_CAN_MessageId_t *id, LID_CAN_MessageData_t *can_data, uint32_t *length) {
	FDCAN_GlobalTypeDef *can =  _fdcans[fdcan_id].can;
	Can_Message_RAM *can_ram = _fdcans[fdcan_id].can_ram;

	if (can->RXF0S & FDCAN_RXF0S_F0FL) {	//Check FIFO 0 Fill Level, set Acknowledge
		uint8_t get_index = ((FDCAN_RXF0S_F0GI_Msk & can->RXF0S) >> FDCAN_RXF0S_F0GI_Pos);
		id->word = can_ram->rx_fifo0[get_index].R0.bit.ID >> 18;

		uint32_t dlc = can_ram->rx_fifo0[get_index].R1.bit.DLC;
		*length = Can_DlcToLength[dlc];
		memcpy(can_data->byte, &can_ram->rx_fifo0[get_index].data.byte[0], FDCAN_ELMTS_ARRAY_SIZE);

		//Ui_ProcessCanMessage(id, &data, length);

		can->RXF0A = get_index & 0x7;
		return 1;
	}
	if (can->RXF1S & FDCAN_RXF1S_F1FL) {	//Check FIFO 1 Fill Level, set Acknowledge
		uint8_t get_index = ((FDCAN_RXF1S_F1GI_Msk & can->RXF1S) >> FDCAN_RXF1S_F1GI_Pos);

		id->word = can_ram->rx_fifo1[get_index].R0.bit.ID >> 18;
		//uint8_t is_extended = can_ram->rx_fifo1[get_index].R0.bit.XTD;
		//uint8_t is_remote_frame = can_ram->rx_fifo1[get_index].R0.bit.RTR;
		//uint8_t is_error_passiv = can_ram->rx_fifo1[get_index].R0.bit.ESI;
		uint32_t dlc = can_ram->rx_fifo1[get_index].R1.bit.DLC;
		*length = Can_DlcToLength[dlc];
		memcpy(can_data->byte, &can_ram->rx_fifo1[get_index].data.byte[0], FDCAN_ELMTS_ARRAY_SIZE);

		can->RXF1A = get_index & 0x7;
		return 1;
	}
	return 0;
}

int LID_CAN_Send(LID_FDCAN_Id_t fdcan_id, LID_CAN_MessageId_t message_id, LID_CAN_MessageData_t *data, uint32_t length) {
	FDCAN_GlobalTypeDef *can = _fdcans[fdcan_id].can;
	Can_Message_RAM *can_ram = _fdcans[fdcan_id].can_ram;
	uint32_t can_length = CAN_MSG_LENGTH(length);
	uint8_t *can_data = data->byte;

	uint8_t index = ((FDCAN_TXFQS_TFQPI_Msk & can->TXFQS) >> FDCAN_TXFQS_TFQPI_Pos);
	if ((can->TXFQS & FDCAN_TXFQS_TFFL) < 1)
		return -1;
	Can_Tx_Element *packet = &can_ram->tx_buffer[index];

	packet->T0.bit.XTD = 0;
	packet->T0.bit.ID = message_id.word << 18;
	packet->T0.bit.RTR = 0;
	packet->T1.bit.FDF = 1;
	packet->T1.bit.BRS = 1;
	packet->T1.bit.DLC = Can_LengthToDlc[can_length];
	packet->T1.bit.EFCC = 0;
	packet->T1.bit.MM = 0;

	uint32_t i = 0;
	for (uint32_t c = 0; c < can_length; c += 4)
		packet->data.word[i++] = can_data[c] | can_data[c + 1] << 8 | can_data[c + 2] << 16 | can_data[c + 3] << 24;
	while (i < Can_DlcToLength[Can_LengthToDlc[can_length]] / 4)
		packet->data.word[i++] = 0;

	can->TXBAR = (1 << index);

	return 0;
}

void LID_CAN_Run() {
	// End initialisation - start FDCAN
	if(_fdcans[LID_FDCAN1].state == LID_CAN_STATE_INITIALISING) {
		CLEAR_BIT(_fdcans[LID_FDCAN1].can->CCCR, FDCAN_CCCR_INIT);
		_fdcans[LID_FDCAN1].state = LID_CAN_STATE_RUNNING;
		LL_mDelay(100);
	}
	if(_fdcans[LID_FDCAN2].state == LID_CAN_STATE_INITIALISING) {
		CLEAR_BIT(_fdcans[LID_FDCAN2].can->CCCR, FDCAN_CCCR_INIT);
		_fdcans[LID_FDCAN2].state = LID_CAN_STATE_RUNNING;
		LL_mDelay(100);
	}
}
