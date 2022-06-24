#include <string.h>
#include <STRHAL_CAN.h>
#include <stdio.h>

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
	STRHAL_CAN_STATE_0,
	STRHAL_CAN_STATE_INITIALISING,
	STRHAL_CAN_STATE_RUNNING
} STRHAL_CAN_State_t;

typedef struct {
	FDCAN_GlobalTypeDef *can;
	Can_Message_RAM *can_ram;
	//STRHAL_CAN_Filters_t filters; TODO: Add filter, noob
	volatile STRHAL_CAN_State_t state;
	STRHAL_CAN_Receptor_t rxReceptors[STRHAL_FDCAN_N_RX];
	uint8_t filter_n;
	uint8_t fifo_sub_state;
} STRHAL_CAN_Handle_t;

static STRHAL_CAN_Handle_t _fdcans[2] = {
	[STRHAL_FDCAN1] = { .can = FDCAN1, .can_ram = FDCAN1_MESSAGE_RAM, .state = STRHAL_CAN_STATE_0, .filter_n = 0, .fifo_sub_state = 0},
	[STRHAL_FDCAN2] = { .can = FDCAN2, .can_ram = FDCAN2_MESSAGE_RAM, .state = STRHAL_CAN_STATE_0, .filter_n = 0, .fifo_sub_state = 0}
};

static void STRHAL_CAN_Init_GPIO(void)
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


int STRHAL_CAN_Instance_Init(STRHAL_FDCAN_Id_t fdcan_id) {
	if(fdcan_id < 0 || fdcan_id >= STRHAL_N_FDCAN)
		return -1;

	_fdcans[fdcan_id].state = STRHAL_CAN_STATE_INITIALISING;

	FDCAN_GlobalTypeDef *can = _fdcans[fdcan_id].can;
	//Can_Message_RAM *can_ram = handles[can_handle_index].can_ram;
	LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PCLK1);

	CLEAR_BIT(can->CCCR, FDCAN_CCCR_CSR);

	uint32_t tot = 0;

	// Check Sleep mode acknowledge
	while ((can->CCCR & FDCAN_CCCR_CSA) == FDCAN_CCCR_CSA) {
		tot++;
		if (tot > STRHAL_CAN_START_TOT)
			return -1;
	}

	SET_BIT(can->CCCR, FDCAN_CCCR_INIT);

	// Wait until the INIT bit into CCCR register is set
	while ((can->CCCR & FDCAN_CCCR_INIT) == 0U) {
		tot++;
		if (tot > STRHAL_CAN_START_TOT)
			return -1;
	}

	// Enable configuration change
	SET_BIT(can->CCCR, FDCAN_CCCR_CCE);
	// Wait until the CCE bit into CCCR register is set
	while ((can->CCCR & FDCAN_CCCR_CCE) == 0U) {
		tot++;
		if (tot > STRHAL_CAN_START_TOT)
			return -1;
	}

	//SET_BIT(can->CCCR, FDCAN_CCCR_DAR);  //AutoRetransmission Disabled
	CLEAR_BIT(can->CCCR, FDCAN_CCCR_DAR);  //AutoRetransmission Enabled

	CLEAR_BIT(can->CCCR, FDCAN_CCCR_TXP);  //transmit pause Disabled

	SET_BIT(can->CCCR, FDCAN_CCCR_PXHD); //Protocol Exception Handling  Disabled

	SET_BIT(can->CCCR, FDCAN_FRAME_FD_BRS); //FD mode with BitRate Switching

	CLEAR_BIT(can->CCCR, (FDCAN_CCCR_TEST | FDCAN_CCCR_MON | FDCAN_CCCR_ASM));
	CLEAR_BIT(can->TEST, FDCAN_TEST_LBCK);

	//if(fdcan_id == STRHAL_FDCAN1) { // TODO find out why this is here - it is here because the FDCAN_CONFIG reg needs only to be set once
		MODIFY_REG(FDCAN_CONFIG->CKDIV, FDCAN_CKDIV_PDIV, FDCAN_CLOCK_DIV2);
	//}

	// Set the nominal bit timing register
	can->NBTP = ((((uint32_t) FDCAN_NOMINAL_SYNC_JUMP_WIDTH - 1U) << FDCAN_NBTP_NSJW_Pos) | (((uint32_t) FDCAN_NOMINAL_TIMESEG_1 - 1U) << FDCAN_NBTP_NTSEG1_Pos) | (((uint32_t) FDCAN_NOMINAL_TIMESEG_2 - 1U) << FDCAN_NBTP_NTSEG2_Pos) | (((uint32_t) FDCAN_NOMINAL_PRESCALER - 1U) << FDCAN_NBTP_NBRP_Pos));

	// Bit Rate Switching Enable
	can->DBTP = ((((uint32_t) FDCAN_DATA_SYNC_JUMP_WIDTH - 1U) << FDCAN_DBTP_DSJW_Pos) | (((uint32_t) FDCAN_DATA_TIMESEG_1 - 1U) << FDCAN_DBTP_DTSEG1_Pos) | (((uint32_t) FDCAN_DATA_TIMESEG_2 - 1U) << FDCAN_DBTP_DTSEG2_Pos) | (((uint32_t) FDCAN_DATA_PRESCALER - 1U) << FDCAN_DBTP_DBRP_Pos));

	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_LSS, (STRHAL_CAN_STD_FILTER_NUMBER << FDCAN_RXGFC_LSS_Pos)); // Standard filter elements number
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_LSE, (STRHAL_CAN_EXT_FILTER_NUMBER << FDCAN_RXGFC_LSE_Pos)); // Extended filter elements number
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_F0OM, (FDCAN_RX_FIFO_BLOCKING << FDCAN_RXGFC_F0OM_Pos)); // FIFO 0 operation mode
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_F1OM, (FDCAN_RX_FIFO_BLOCKING << FDCAN_RXGFC_F1OM_Pos)); // FIFO 1 operation mode
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_ANFS, (FDCAN_REJECT << FDCAN_RXGFC_ANFS_Pos)); // Accept Non-matching Frames Standard
	MODIFY_REG(can->RXGFC, FDCAN_RXGFC_ANFE, (FDCAN_REJECT << FDCAN_RXGFC_ANFE_Pos)); // Accept Non-matching Frames Extended
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

int STRHAL_CAN_Subscribe(STRHAL_FDCAN_Id_t fdcan_id, STRHAL_FDCAN_Rx_Id_t rx_id, STRHAL_FDCAN_Filter_t *filter, uint8_t n, STRHAL_CAN_Receptor_t receptor){
	/* Error handling for user inputs */
	if(fdcan_id < 0 || fdcan_id >= STRHAL_N_FDCAN) // invalid fdcan instance
		return -1;

	STRHAL_CAN_Handle_t *fdcan = &_fdcans[fdcan_id];
	if(fdcan->state != STRHAL_CAN_STATE_INITIALISING)	// fdcan not in init mode (Subscribe called in wrong order)
		return -1;

	if(filter == NULL || receptor == NULL) // Nullptr check
		return 0;

	if(n > STRHAL_CAN_RAM_N_FILTER - fdcan->filter_n) // Correct Filter Nbr
		n = fdcan->filter_n - n;

	if(fdcan->fifo_sub_state & (1U << rx_id)) // Already subscribed to this fifo
		return -1;

	Can_Message_RAM *can_ram = fdcan->can_ram;

	uint32_t sfec;

	switch(rx_id) {
		case STRHAL_FDCAN_RX0:
			fdcan->rxReceptors[0] = receptor;
			fdcan->fifo_sub_state |= 1U << STRHAL_FDCAN_RX0;
			sfec = FDCAN_FILTER_TO_RXFIFO0;
			break;

		case STRHAL_FDCAN_RX1:
			fdcan->rxReceptors[1] = receptor;
			fdcan->fifo_sub_state |= 1U << STRHAL_FDCAN_RX1;
			sfec = FDCAN_FILTER_TO_RXFIFO1;
		default:
			return -1;
	}

	uint8_t i;
	for(i = 0; i < n; i++, fdcan->filter_n++) {
		can_ram->std_filters[i].S0.bit.SFEC = sfec;
		can_ram->std_filters[i].S0.bit.SFID1 = filter[i].value_id1;
		can_ram->std_filters[i].S0.bit.SFID2 = filter[i].mask_id2;
		can_ram->std_filters[i].S0.bit.SFT = filter[i].type;
	}
	MODIFY_REG(fdcan->can->RXGFC, FDCAN_RXGFC_LSS, (fdcan->filter_n << FDCAN_RXGFC_LSS_Pos)); // Standard filter elements number
	return n;
}

STRHAL_Oof_t STRHAL_CAN_Init()
{
	STRHAL_Oof_t status = STRHAL_NOICE;
	STRHAL_CAN_Init_GPIO();

	return status;
}

int32_t STRHAL_CAN_Receive(STRHAL_FDCAN_Id_t fdcan_id, uint32_t *id, uint8_t *data, uint32_t n) {
	if(fdcan_id < 0 || fdcan_id >= STRHAL_N_FDCAN)
		return -1;

	if(n == 0)
		return 0;

	FDCAN_GlobalTypeDef *can =  _fdcans[fdcan_id].can;
	Can_Message_RAM *can_ram = _fdcans[fdcan_id].can_ram;

	uint8_t i;
	uint32_t l;

	Can_Rx_Element *rx_fifo;
	__IO uint32_t *rxfXA;

	if (can->RXF0S & FDCAN_RXF0S_F0FL) {	//Check FIFO 0 Fill Level, set Acknowledge
		i = ((FDCAN_RXF0S_F0GI_Msk & can->RXF0S) >> FDCAN_RXF0S_F0GI_Pos);
		rx_fifo = &can_ram->rx_fifo0[i];
		rxfXA = &can->RXF0A;
	}
	else if (can->RXF1S & FDCAN_RXF1S_F1FL) {
		i = ((FDCAN_RXF1S_F1GI_Msk & can->RXF1S) >> FDCAN_RXF1S_F1GI_Pos);
		rx_fifo = &can_ram->rx_fifo1[i];
		rxfXA = &can->RXF1A;
	}
	else {
		return 0;
	}

	*id = rx_fifo->R0.bit.ID >> 18;
	l = Can_DlcToLength[can_ram->rx_fifo1[i].R1.bit.DLC];
	n = n > l-2 ? l-2 : n;

	memcpy(data, can_ram->rx_fifo1[i].data.byte, n < FDCAN_ELMTS_ARRAY_SIZE ? n : FDCAN_ELMTS_ARRAY_SIZE);
	*rxfXA = i & 0x7;
	return n;
}

int32_t STRHAL_CAN_Send(STRHAL_FDCAN_Id_t fdcan_id, uint32_t id, const uint8_t *data, uint32_t n) {
	if(fdcan_id < 0 || fdcan_id >= STRHAL_N_FDCAN)
		return -1;

	if(n == 0)
		return 0;

	FDCAN_GlobalTypeDef *can = _fdcans[fdcan_id].can;
	Can_Message_RAM *can_ram = _fdcans[fdcan_id].can_ram;

	if (!(can->TXFQS & FDCAN_TXFQS_TFFL))
		return -1;

	if(n > FDCAN_ELMTS_ARRAY_SIZE)
		n = FDCAN_ELMTS_ARRAY_SIZE;

	uint8_t i = ((can->TXFQS & FDCAN_TXFQS_TFQPI_Msk) >> FDCAN_TXFQS_TFQPI_Pos);

	Can_Tx_Element *frame = &can_ram->tx_buffer[i];
	frame->T0.bit.XTD = 0;
	frame->T0.bit.ID = id << 18;
	frame->T0.bit.RTR = 0;
	frame->T1.bit.FDF = 1;
	frame->T1.bit.BRS = 1;
	frame->T1.bit.DLC = Can_LengthToDlc[n];
	frame->T1.bit.EFCC = 0;
	frame->T1.bit.MM = 0;

	/*if(frame->T0.bit.ID == 0 || frame->T1.bit.DLC == 0) {
		uint8_t temp = 1;
		(void) temp;
	}*/

	uint32_t j = 0;
	for (uint32_t c = 0; c < n; c += 4)
		frame->data.word[j++] = data[c] | data[c + 1] << 8 | data[c + 2] << 16 | data[c + 3] << 24;
	while (j < Can_DlcToLength[Can_LengthToDlc[n]] / 4)
		frame->data.word[j++] = 0;

	can->TXBAR = (1 << i);

	return n;
}


void STRHAL_CAN_Run() {
	// End initialisation - start FDCANs
	STRHAL_CAN_Handle_t *fdcan1 = &_fdcans[STRHAL_FDCAN1];
	STRHAL_CAN_Handle_t *fdcan2 = &_fdcans[STRHAL_FDCAN2];
	if(fdcan1->state == STRHAL_CAN_STATE_INITIALISING) {
		if(fdcan1->fifo_sub_state & (1U << STRHAL_FDCAN_RX0)) {
			CLEAR_BIT(FDCAN1->ILS, FDCAN_ILS_RXFIFO0);
			SET_BIT(FDCAN1->ILE, FDCAN_ILE_EINT0);
			SET_BIT(FDCAN1->IE, FDCAN_IE_RF0NE);
		}
		if(fdcan1->fifo_sub_state & (1U << STRHAL_FDCAN_RX1)) {
			CLEAR_BIT(FDCAN1->ILS, FDCAN_ILS_RXFIFO1);
			SET_BIT(FDCAN1->ILE, FDCAN_ILE_EINT0);
			SET_BIT(FDCAN1->IE, FDCAN_IE_RF1NE);
		}

		NVIC_SetPriority(FDCAN1_IT0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 1));
		NVIC_EnableIRQ(FDCAN1_IT0_IRQn);

		CLEAR_BIT(FDCAN1->CCCR, FDCAN_CCCR_INIT);

		_fdcans[STRHAL_FDCAN1].state = STRHAL_CAN_STATE_RUNNING;
		LL_mDelay(100);
	}
	if(fdcan2->state == STRHAL_CAN_STATE_INITIALISING) {
		if(fdcan2->fifo_sub_state & (1U << STRHAL_FDCAN_RX0)) {
			CLEAR_BIT(FDCAN2->ILS, FDCAN_ILS_RXFIFO0);
			SET_BIT(FDCAN2->ILE, FDCAN_ILE_EINT0);
			SET_BIT(FDCAN2->IE, FDCAN_IE_RF0NE);
		}
		if(fdcan2->fifo_sub_state & (1U << STRHAL_FDCAN_RX1)) {
			CLEAR_BIT(FDCAN2->ILS, FDCAN_ILS_RXFIFO1);
			SET_BIT(FDCAN2->ILE, FDCAN_ILE_EINT0);
			SET_BIT(FDCAN2->IE, FDCAN_IE_RF1NE);
		}

		NVIC_SetPriority(FDCAN2_IT0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 2));
		NVIC_EnableIRQ(FDCAN2_IT0_IRQn);

		CLEAR_BIT(FDCAN2->CCCR, FDCAN_CCCR_INIT);
		_fdcans[STRHAL_FDCAN2].state = STRHAL_CAN_STATE_RUNNING;
		LL_mDelay(100);
	}

}

void FDCAN1_IT0_IRQHandler(void) {
	if(FDCAN1->IR & FDCAN_IR_RF0N) {
		SET_BIT(FDCAN1->IR, FDCAN_IR_RF0N);

		STRHAL_CAN_Receptor_t rec = _fdcans[STRHAL_FDCAN1].rxReceptors[0];
		Can_Message_RAM *can_ram = _fdcans[STRHAL_FDCAN1].can_ram;

		uint8_t i;
		uint32_t l;
		uint32_t id;

		i = ((FDCAN_RXF0S_F0GI_Msk & FDCAN1->RXF0S) >> FDCAN_RXF0S_F0GI_Pos);

		Can_Rx_Element *rx_fifo = &can_ram->rx_fifo0[i];

		id = rx_fifo->R0.bit.ID >> 18;
		l = Can_DlcToLength[rx_fifo->R1.bit.DLC];

		if(rec != NULL)
			rec(id, rx_fifo->data.byte, l-2);

		FDCAN1->RXF0A = i & 0x7;
	}
	if(FDCAN1->IR & FDCAN_IR_RF1N) {
		SET_BIT(FDCAN1->IR, FDCAN_IR_RF1N);

		STRHAL_CAN_Receptor_t rec = _fdcans[STRHAL_FDCAN1].rxReceptors[1];
		Can_Message_RAM *can_ram = _fdcans[STRHAL_FDCAN1].can_ram;

		uint8_t i;
		uint32_t l;
		uint32_t id;

		i = ((FDCAN_RXF1S_F1GI_Msk & FDCAN1->RXF1S) >> FDCAN_RXF1S_F1GI_Pos);

		Can_Rx_Element *rx_fifo = &can_ram->rx_fifo1[i];

		id = rx_fifo->R0.bit.ID >> 18;
		l = Can_DlcToLength[rx_fifo->R1.bit.DLC];

		//rec(id, rx_fifo->data.byte, l);
		if(rec != NULL)
			rec(id, rx_fifo->data.byte, l-2);
		FDCAN1->RXF1A = i & 0x7;
	}
}

void FDCAN2_IT0_IRQHandler(void) {
	if(FDCAN2->IR & FDCAN_IR_RF0N) {
		CLEAR_BIT(FDCAN2->IR, FDCAN_IR_RF0N);

		STRHAL_CAN_Receptor_t rec = _fdcans[STRHAL_FDCAN2].rxReceptors[0];
		Can_Message_RAM *can_ram = _fdcans[STRHAL_FDCAN2].can_ram;

		uint8_t i;
		uint32_t l;
		uint32_t id;

		i = ((FDCAN_RXF0S_F0GI_Msk & FDCAN2->RXF0S) >> FDCAN_RXF0S_F0GI_Pos);

		Can_Rx_Element *rx_fifo = &can_ram->rx_fifo0[i];

		id = rx_fifo->R0.bit.ID >> 18;
		l = Can_DlcToLength[rx_fifo->R1.bit.DLC];

		//rec(id, rx_fifo->data.byte, l);
		if(rec != NULL)
			rec(id, rx_fifo->data.byte, l-2);

		FDCAN2->RXF0A = i & 0x7;

	}
	if(FDCAN2->IR & FDCAN_IR_RF1N) {
		CLEAR_BIT(FDCAN2->IR, FDCAN_IR_RF1N);

		STRHAL_CAN_Receptor_t rec = _fdcans[STRHAL_FDCAN2].rxReceptors[1];
		Can_Message_RAM *can_ram = _fdcans[STRHAL_FDCAN2].can_ram;

		uint8_t i;
		uint32_t l;
		uint32_t id;

		i = ((FDCAN_RXF1S_F1GI_Msk & FDCAN2->RXF1S) >> FDCAN_RXF1S_F1GI_Pos);

		Can_Rx_Element *rx_fifo = &can_ram->rx_fifo1[i];

		id = rx_fifo->R0.bit.ID >> 18;
		l = Can_DlcToLength[rx_fifo->R1.bit.DLC];

		//rec(id, rx_fifo->data.byte, l);
		if(rec != NULL)
			rec(id, rx_fifo->data.byte, l-2);
		FDCAN2->RXF1A = i & 0x7;
	}
}
