#include "can.h"
#include "ui.h"
#include "serial.h"
#include "systick.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CAN_MESSAGE_RAM_START_ADDRESS (0x4000AC00)
#define BIT_2_TO_15_MASK         0x0000fffc //TODO Add to memory mapping

static const uint8_t Can_DlcToLength[] =
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };

static const uint32_t Can_LengthToDlc[] =
{ 0,    //  0
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

static const Can_Handle handles[] =
{
{
FDCAN1, (Can_Message_RAM*) CAN_MESSAGE_RAM_START_ADDRESS },
{
FDCAN2, (Can_Message_RAM*) (CAN_MESSAGE_RAM_START_ADDRESS + sizeof(Can_Message_RAM)) } };

static Result_t Can_SetRamBlockAddresses(uint32_t can_handle_index)
{
	FDCAN_GlobalTypeDef *can = handles[can_handle_index].can;
	Can_Message_RAM *can_ram = handles[can_handle_index].can_ram;
	MODIFY_REG(can->SIDFC, FDCAN_SIDFC_FLSSA, ((uint32_t)can_ram->std_filters - CAN_MESSAGE_RAM_START_ADDRESS));	// Standard filter list start address
	MODIFY_REG(can->SIDFC, FDCAN_SIDFC_LSS, (CAN_STD_FILTER_NUMBER << FDCAN_SIDFC_LSS_Pos)); // Standard filter elements number
	MODIFY_REG(can->XIDFC, FDCAN_XIDFC_FLESA, ((uint32_t)can_ram->ext_filters - CAN_MESSAGE_RAM_START_ADDRESS));	// Extended filter list start address
	MODIFY_REG(can->XIDFC, FDCAN_XIDFC_LSE, (CAN_EXT_FILTER_NUMBER << FDCAN_XIDFC_LSE_Pos)); // Extended filter elements number
	MODIFY_REG(can->RXF0C, FDCAN_RXF0C_F0SA, ((uint32_t)can_ram->rx_fifo0 - CAN_MESSAGE_RAM_START_ADDRESS)); // Rx FIFO 0 start address
	MODIFY_REG(can->RXF0C, FDCAN_RXF0C_F0S, (CAN_RX_FIFO0_ELMTS_NUMBER << FDCAN_RXF0C_F0S_Pos)); // Rx FIFO 0 elements number
	MODIFY_REG(can->RXF1C, FDCAN_RXF1C_F1SA, ((uint32_t)can_ram->rx_fifo1 - CAN_MESSAGE_RAM_START_ADDRESS)); // Rx FIFO 1 start address
	MODIFY_REG(can->RXF1C, FDCAN_RXF1C_F1S, (CAN_RX_FIFO1_ELMTS_NUMBER << FDCAN_RXF1C_F1S_Pos)); // Rx FIFO 1 elements number

	MODIFY_REG(can->RXBC, FDCAN_RXBC_RBSA, ((uint32_t)can_ram->rx_buffer - CAN_MESSAGE_RAM_START_ADDRESS)); // Rx buffer list start address

	MODIFY_REG(can->TXEFC, FDCAN_TXEFC_EFSA, ((uint32_t)can_ram->tx_fifo - CAN_MESSAGE_RAM_START_ADDRESS)); // Tx event FIFO start address
	MODIFY_REG(can->TXEFC, FDCAN_TXEFC_EFS, (CAN_TX_EVENT_NUMBER << FDCAN_TXEFC_EFS_Pos)); // Tx event FIFO elements number

	MODIFY_REG(can->TXBC, FDCAN_TXBC_TBSA, ((uint32_t)can_ram->tx_buffer - CAN_MESSAGE_RAM_START_ADDRESS)); // Tx buffer list start address
	MODIFY_REG(can->TXBC, FDCAN_TXBC_NDTB, (CAN_TX_BUFFER_NUMBER << FDCAN_TXBC_NDTB_Pos)); // Dedicated Tx buffers number
	MODIFY_REG(can->TXBC, FDCAN_TXBC_TFQS, ( CAN_TX_FIFO_QUEUE_ELMTS_NUMBER << FDCAN_TXBC_TFQS_Pos)); // Tx FIFO/Queue elements number
	CLEAR_BIT(can->TXBC, FDCAN_TXBC_TFQM);

	return NOICE;
}
Result_t Can_InitFdcan(uint32_t can_handle_index)
{
	FDCAN_GlobalTypeDef *can = handles[can_handle_index].can;
	//Can_Message_RAM *can_ram = handles[can_handle_index].can_ram;
	LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PLL2Q);

	const uint32_t CvtEltSize[] =
	{ 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7 };

	CLEAR_BIT(can->CCCR, FDCAN_CCCR_CSR);

	uint32_t tickstart = Systick_GetTick();

	// Check Sleep mode acknowledge
	while ((can->CCCR & FDCAN_CCCR_CSA) == FDCAN_CCCR_CSA)
	{
		if ((Systick_GetTick() - tickstart) > CAN_TIMEOUT_VALUE)
			return OOF_CAN_INIT;
	}

	SET_BIT(can->CCCR, FDCAN_CCCR_INIT);

	tickstart = Systick_GetTick();

	// Wait until the INIT bit into CCCR register is set
	while ((can->CCCR & FDCAN_CCCR_INIT) == 0U)
	{
		if ((Systick_GetTick() - tickstart) > CAN_TIMEOUT_VALUE)
			return OOF_CAN_INIT;
	}

	// Enable configuration change
	SET_BIT(can->CCCR, FDCAN_CCCR_CCE);

	//CLEAR_BIT(can->CCCR, FDCAN_CCCR_DAR);  //AutoRetransmission Enabled
	SET_BIT(can->CCCR, FDCAN_CCCR_DAR);  //AutoRetransmission Disabled

	//SET_BIT(can->CCCR, FDCAN_CCCR_TXP);    //transmit pause Enabled
	CLEAR_BIT(can->CCCR, FDCAN_CCCR_TXP);  //transmit pause Disabled

	//CLEAR_BIT(can->CCCR, FDCAN_CCCR_PXHD); //Protocol Exception Handling Enabled
	SET_BIT(can->CCCR, FDCAN_CCCR_PXHD); //Protocol Exception Handling  Disabled

	SET_BIT(can->CCCR, FDCAN_CCCR_BRSE);

	SET_BIT(can->CCCR, FDCAN_CCCR_FDOE);
	CLEAR_BIT(can->CCCR, (FDCAN_CCCR_TEST | FDCAN_CCCR_MON | FDCAN_CCCR_ASM));
	CLEAR_BIT(can->TEST, FDCAN_TEST_LBCK);

	// Set FDCAN Operating Mode:
	//           | Normal | Restricted |    Bus     | Internal | External
	//           |        | Operation  | Monitoring | LoopBack | LoopBack
	// CCCR.TEST |   0    |     0      |     0      |    1     |    1
	// CCCR.MON  |   0    |     0      |     1      |    1     |    0
	// TEST.LBCK |   0    |     0      |     0      |    1     |    1
	// CCCR.ASM  |   0    |     1      |     0      |    0     |    0

	// Set the nominal bit timing register
	can->NBTP = ((((uint32_t) CAN_NOMINAL_SYNC_JUMP_WIDTH - 1U) << FDCAN_NBTP_NSJW_Pos) | (((uint32_t) CAN_NOMINAL_TIMESEG_1 - 1U) << FDCAN_NBTP_NTSEG1_Pos) | (((uint32_t) CAN_NOMINAL_TIMESEG_2 - 1U) << FDCAN_NBTP_NTSEG2_Pos) | (((uint32_t) CAN_NOMINAL_PRESCALER - 1U) << FDCAN_NBTP_NBRP_Pos));

	// Bit Rate Switching Enable
	can->DBTP = ((((uint32_t) CAN_DATA_SYNC_JUMP_WIDTH - 1U) << FDCAN_DBTP_DSJW_Pos) | (((uint32_t) CAN_DATA_TIMESEG_1 - 1U) << FDCAN_DBTP_DTSEG1_Pos) | (((uint32_t) CAN_DATA_TIMESEG_2 - 1U) << FDCAN_DBTP_DTSEG2_Pos) | (((uint32_t) CAN_DATA_PRESCALER - 1U) << FDCAN_DBTP_DBRP_Pos));

	if (CAN_TX_FIFO_QUEUE_ELMTS_NUMBER > 0U)	// Select between Tx FIFO and Tx Queue operation modes
		SET_BIT(can->TXBC, CAN_TX_FIFO_QUEUE_MODE);

	// Configure Tx element size
	if ((CAN_TX_BUFFER_NUMBER + CAN_TX_FIFO_QUEUE_ELMTS_NUMBER) > 0U)
		MODIFY_REG(can->TXESC, FDCAN_TXESC_TBDS, CvtEltSize[CAN_TX_ELMTS_SIZE]);

	// Configure Rx FIFO 0 element size
	if (CAN_RX_FIFO0_ELMTS_NUMBER > 0U)
		MODIFY_REG(can->RXESC, FDCAN_RXESC_F0DS, (CvtEltSize[CAN_RX_FIFO0_ELMTS_SIZE] << FDCAN_RXESC_F0DS_Pos));

	// Configure Rx FIFO 1 element size
	if (CAN_RX_FIFO1_ELMTS_NUMBER > 0U)
		MODIFY_REG(can->RXESC, FDCAN_RXESC_F1DS, (CvtEltSize[CAN_RX_FIFO1_ELMTS_SIZE] << FDCAN_RXESC_F1DS_Pos));

	/* Configure Rx buffer element size */
	if (CAN_RX_BUFFER_NUMBER > 0U)
		MODIFY_REG(can->RXESC, FDCAN_RXESC_RBDS, (CvtEltSize[CAN_RX_BUFFER_SIZE] << FDCAN_RXESC_RBDS_Pos));

	//TODO: TTCAN maybe?
	// By default operation mode is set to Event-driven communication.
	// If Time-triggered communication is needed, user should call the
	// HAL_FDCAN_TT_ConfigOperation function just after the HAL_FDCAN_Init
	//	TTCAN_TypeDef *ttcan = (TTCAN_TypeDef*) ((uint32_t) can + 0x100U);
	//if (hfdcan->Instance == FDCAN1)
	//		CLEAR_BIT(hfdcan->ttcan->TTOCF, FDCAN_TTOCF_OM);

	// Calculate each RAM block address
	Result_t result = Can_SetRamBlockAddresses(can_handle_index);
	if (result != NOICE)
		return result;

	//Config TxDelayCompensation
	can->TDCR = ((CAN_TDC_FILTER << FDCAN_TDCR_TDCF_Pos) | (CAN_TDC_OFFSET << FDCAN_TDCR_TDCO_Pos));

	// Enable transmitter delay compensation
	SET_BIT(can->DBTP, FDCAN_DBTP_TDC);

	if (can == FDCAN1)	// Configure global filter to reject everything
		can->GFC = ((FDCAN_REJECT << FDCAN_GFC_ANFS_Pos) | (FDCAN_REJECT << FDCAN_GFC_ANFE_Pos) | (FDCAN_FILTER_REMOTE << FDCAN_GFC_RRFS_Pos) | (FDCAN_REJECT_REMOTE << FDCAN_GFC_RRFE_Pos));

	if (can == FDCAN2)	//Accept everything
		can->GFC = ((FDCAN_ACCEPT_IN_RX_FIFO1 << FDCAN_GFC_ANFS_Pos) | (FDCAN_ACCEPT_IN_RX_FIFO1 << FDCAN_GFC_ANFE_Pos) | (FDCAN_FILTER_REMOTE << FDCAN_GFC_RRFS_Pos) | (FDCAN_REJECT_REMOTE << FDCAN_GFC_RRFE_Pos));

	//Can Start
	CLEAR_BIT(can->CCCR, FDCAN_CCCR_INIT);

	Systick_BusyWait(100);
	return NOICE;
}

void Can_ClockCalibration(void)
{

	/* Bypass clock calibration */
	SET_BIT(FDCAN_CCU->CCFG, FDCANCCU_CCFG_BCC);

	/* Configure clock divider */
	MODIFY_REG(FDCAN_CCU->CCFG, FDCANCCU_CCFG_CDIV, FDCAN_CLOCK_DIV1);

}

void Can_InitGPIO(void)
{

	LL_GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PLL2Q);

	LL_APB1_GRP2_EnableClock( LL_APB1_GRP2_PERIPH_FDCAN);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

	//FDCAN1 GPIO Configuration
	//PA11     ------> FDCAN1_RX
	//PA12     ------> FDCAN1_TX

	GPIO_InitStruct.Pin = LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_9;

	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//FDCAN2 GPIO Configuration
	//PB12     ------> FDCAN2_RX
	//PB13     ------> FDCAN2_TX

	GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_9;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
void Can_AddStdFilter(uint32_t can_handle_index, uint32_t filter_index, uint32_t mask, uint32_t filter_id, uint32_t target_location)
{
	Can_Message_RAM *can_ram = handles[can_handle_index].can_ram;

	can_ram->std_filters[filter_index].S0.bit.SFEC = target_location; // e.g. FDCAN_FILTER_TO_RXFIFO0
	can_ram->std_filters[filter_index].S0.bit.SFID1 = filter_id;
	can_ram->std_filters[filter_index].S0.bit.SFID2 = mask;
	can_ram->std_filters[filter_index].S0.bit.SFT = FDCAN_FILTER_MASK;
}
Result_t Can_Init(uint8_t node_id)
{
	Result_t result = OOF;
	Can_ClockCalibration();
	Can_InitGPIO();
	Can_MessageId_t mask =
	{ 0 };
	mask.info.direction = 0x1;
	mask.info.node_id = 0x1F;
	mask.info.special_cmd = 0x3;
	Can_MessageId_t id =
	{ 0 };
	id.info.direction = MASTER2NODE_DIRECTION;
	id.info.special_cmd = STANDARD_SPECIAL_CMD;

	result = Can_InitFdcan(MAIN_CAN_BUS);
	if (result != NOICE)
		return result;
	id.info.node_id = node_id;
	Can_AddStdFilter(MAIN_CAN_BUS, 0, mask.uint32, id.uint32, FDCAN_FILTER_TO_RXFIFO0);
	id.info.node_id = 0;
	Can_AddStdFilter(MAIN_CAN_BUS, 1, mask.uint32, id.uint32, FDCAN_FILTER_TO_RXFIFO0);

	//TODO: remove for Debugging only
	result = Can_InitFdcan(1);
	if (result != NOICE)
		return result;

	return NOICE;
}

void Can_checkFifo(uint32_t can_handle_index)
{

	FDCAN_GlobalTypeDef *can = handles[can_handle_index].can;
	Can_Message_RAM *can_ram = handles[can_handle_index].can_ram;
	Can_MessageId_t id =
	{ 0 };
	uint32_t length = 0;
	Can_MessageData_t data =
	{ 0 };
	//TODO Loop over both fifos
	if (can->RXF0S & FDCAN_RXF0S_F0FL)	//Check FIFO 0
	{
		uint8_t get_index = ((can->RXF0S >> 8) & 0x3F);
		id.uint32 = can_ram->rx_fifo0[get_index].R0.bit.ID >> 18;
		//uint8_t is_extended = can_ram->rx_fifo0[get_index].R0.bit.XTD;
		uint8_t is_remote_frame = can_ram->rx_fifo0[get_index].R0.bit.RTR;
		uint8_t is_error_passiv = can_ram->rx_fifo0[get_index].R0.bit.ESI;
		uint32_t dlc = can_ram->rx_fifo0[get_index].R1.bit.DLC;
		length = Can_DlcToLength[dlc];
		memcpy(data.uint8, &can_ram->rx_fifo0[get_index].data.uint8[0], CAN_ELMTS_ARRAY_SIZE);

		Serial_PutString("FDCAN ");
		Serial_PrintInt(can_handle_index + 1);
		Serial_PrintString("FIFO 0:  ");

		Serial_PutString("id:  ");
		Serial_PrintInt(id.uint32);
		Serial_PutString("id.info.node_id:  ");
		Serial_PrintInt(id.info.node_id);
		Serial_PutString("is_remote_frame:  ");
		Serial_PrintInt(is_remote_frame);
		Serial_PutString("is_error_passiv:  ");
		Serial_PrintInt(is_error_passiv);
		Serial_PutString("dlc:  ");
		Serial_PrintInt(dlc);

		for (uint32_t c = 0; c < length; c++)
		{
			Serial_PutInt(c);
			Serial_PutString(":  ");
			Serial_PutInt(data.uint8[c]);
			Serial_PutString("  ");
			Serial_PrintHex(data.uint8[c]);
		}

		Ui_ProcessCanMessage(id, &data, length);

		can->RXF0A = get_index & 0x3F;
	}
	if (can->RXF1S & FDCAN_RXF1S_F1FL)	//Check FIFO 1
	{
		uint8_t get_index = ((can->RXF1S >> 8) & 0x3F);

		id.uint32 = can_ram->rx_fifo1[get_index].R0.bit.ID >> 18;
		//uint8_t is_extended = can_ram->rx_fifo1[get_index].R0.bit.XTD;
		uint8_t is_remote_frame = can_ram->rx_fifo1[get_index].R0.bit.RTR;
		uint8_t is_error_passiv = can_ram->rx_fifo1[get_index].R0.bit.ESI;
		uint32_t dlc = can_ram->rx_fifo1[get_index].R1.bit.DLC;
		length = Can_DlcToLength[dlc];
		memcpy(data.uint8, &can_ram->rx_fifo1[get_index].data.uint8[0], CAN_ELMTS_ARRAY_SIZE);
		Serial_PutString("FDCAN ");
		Serial_PrintInt(can_handle_index + 1);
		Serial_PrintString("FIFO 1:  ");

		Serial_PutString("id:  ");
		Serial_PrintInt(id.uint32);
		Serial_PutString("id.info.node_id:  ");
		Serial_PrintInt(id.info.node_id);
		Serial_PutString("is_remote_frame:  ");
		Serial_PrintInt(is_remote_frame);
		Serial_PutString("is_error_passiv:  ");
		Serial_PrintInt(is_error_passiv);
		Serial_PutString("dlc:  ");
		Serial_PrintInt(dlc);

		for (uint32_t c = 0; c < length; c++)
		{
			Serial_PutInt(c);
			Serial_PutString(":  ");
			Serial_PutInt(data.uint8[c]);
			Serial_PutString("  ");
			Serial_PrintHex(data.uint8[c]);
		}
		//Ui_ProcessCanMessage(id, &data, length);

		can->RXF1A = get_index & 0x3F;
	}

}

Result_t Can_sendMessage(uint32_t can_handle_index, uint32_t message_id, uint8_t *data, uint32_t length)
{
	FDCAN_GlobalTypeDef *can = handles[can_handle_index].can;
	Can_Message_RAM *can_ram = handles[can_handle_index].can_ram;

	uint8_t index = ((FDCAN_TXFQS_TFQPI_Msk & can->TXFQS) >> FDCAN_TXFQS_TFQPI_Pos);
	if ((can->TXFQS & FDCAN_TXFQS_TFFL) < 1)
		return OOF_CAN_TX_FULL;
	Can_Tx_Element *packet = &can_ram->tx_fifo[index];

	packet->T0.bit.XTD = 0;
	packet->T0.bit.ID = message_id << 18;
	packet->T0.bit.RTR = 0;
	packet->T1.bit.FDF = 1;
	packet->T1.bit.BRS = 1;
	packet->T1.bit.DLC = Can_LengthToDlc[length];
	packet->T1.bit.EFCC = 0;
	packet->T1.bit.MM = 0;

	uint32_t i = 0;
	for (uint32_t c = 0; c < length; c += 4)
		packet->data.uint32[i++] = data[c] | data[c + 1] << 8 | data[c + 2] << 16 | data[c + 3] << 24;
	while (i < Can_DlcToLength[Can_LengthToDlc[length]] / 4)
		packet->data.uint32[i++] = 0;

	Serial_PrintString("CAN SEND MESSAGE");
	Serial_PutString("message_id: ");
	Serial_PrintInt(message_id);

	for (uint32_t c = 0; c < length; c++)
	{
		Serial_PutInt(c);
		Serial_PutString(":  ");
		Serial_PutInt(packet->data.uint8[c]);
		Serial_PutString("  ");
		Serial_PrintHex(packet->data.uint8[c]);
	}

	can->TXBAR = (1 << index);

	return NOICE;
}

