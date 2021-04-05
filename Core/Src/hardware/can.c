#include "can.h"
#include "main.h"
#include "cmds.h"
#include "sysclock.h"





static Can_Message_RAM can1_ram;
static Can_Message_RAM can2_ram;


static Result_t Can_SetRamBlockAddresses(FDCAN_GlobalTypeDef *can, Can_Message_RAM *can_ram)
{
	uint32_t StartAddress;


	MODIFY_REG(can->SIDFC, FDCAN_SIDFC_FLSSA, (((uint32_t)can_ram->std_filters) << FDCAN_SIDFC_FLSSA_Pos));	// Standard filter list start address
	MODIFY_REG(can->SIDFC, FDCAN_SIDFC_LSS, (CAN_STD_FILTER_NUMBER << FDCAN_SIDFC_LSS_Pos));	// Standard filter elements number
	MODIFY_REG(can->XIDFC, FDCAN_XIDFC_FLESA, (((uint32_t)can_ram->ext_filters) << FDCAN_XIDFC_FLESA_Pos));	// Extended filter list start address
	MODIFY_REG(can->XIDFC, FDCAN_XIDFC_LSE, (CAN_EXT_FILTER_NUMBER << FDCAN_XIDFC_LSE_Pos));	// Extended filter elements number
	MODIFY_REG(can->RXF0C, FDCAN_RXF0C_F0SA, (((uint32_t)can_ram->rx_fifo0) << FDCAN_RXF0C_F0SA_Pos));	// Rx FIFO 0 start address
	MODIFY_REG(can->RXF0C, FDCAN_RXF0C_F0S, (CAN_RX_FIFO0_ELMTS_NUMBER << FDCAN_RXF0C_F0S_Pos));	// Rx FIFO 0 elements number
	MODIFY_REG(can->RXF1C, FDCAN_RXF1C_F1SA, (((uint32_t)can_ram->rx_fifo1) << FDCAN_RXF1C_F1SA_Pos));	// Rx FIFO 1 start address
	MODIFY_REG(can->RXF1C, FDCAN_RXF1C_F1S, (CAN_RX_FIFO1_ELMTS_NUMBER << FDCAN_RXF1C_F1S_Pos));	// Rx FIFO 1 elements number

	MODIFY_REG(can->RXBC, FDCAN_RXBC_RBSA, (((uint32_t)can_ram->rx_buffer) << FDCAN_RXBC_RBSA_Pos));	// Rx buffer list start address

	MODIFY_REG(can->TXEFC, FDCAN_TXEFC_EFSA, (((uint32_t)can_ram->tx_fifo) << FDCAN_TXEFC_EFSA_Pos));// Tx event FIFO start address
	MODIFY_REG(can->TXEFC, FDCAN_TXEFC_EFS, (CAN_TX_EVENT_NUMBER << FDCAN_TXEFC_EFS_Pos));// Tx event FIFO elements number

	MODIFY_REG(can->TXBC, FDCAN_TXBC_TBSA, (((uint32_t)can_ram->tx_buffer) << FDCAN_TXBC_TBSA_Pos));// Tx buffer list start address
	MODIFY_REG(can->TXBC, FDCAN_TXBC_NDTB, (CAN_TX_BUFFER_NUMBER << FDCAN_TXBC_NDTB_Pos));// Dedicated Tx buffers number
	MODIFY_REG(can->TXBC, FDCAN_TXBC_TFQS, ( CAN_TX_FIFO_QUEUE_ELMTS_NUMBER << FDCAN_TXBC_TFQS_Pos));// Tx FIFO/Queue elements number


	return NOICE;
}

Result_t Fdcan_Init(FDCAN_GlobalTypeDef *can)
{
	LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PLL2Q);

	const uint32_t CvtEltSize[] =
	{ 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 5, 0, 0, 0, 6, 0, 0, 0, 7 };

	CLEAR_BIT(can->CCCR, FDCAN_CCCR_CSR);

	uint32_t tickstart = Systick_GetTick();

	// Check Sleep mode acknowledge
	while ((can->CCCR & FDCAN_CCCR_CSA) == FDCAN_CCCR_CSA)
	{
		if ((Systick_GetTick() - tickstart) > CAN_TIMEOUT_VALUE)
			return OOF_INIT_CAN;
	}

	SET_BIT(can->CCCR, FDCAN_CCCR_INIT);

	tickstart = Systick_GetTick();

	// Wait until the INIT bit into CCCR register is set
	while ((can->CCCR & FDCAN_CCCR_INIT) == 0U)
	{
		if ((Systick_GetTick() - tickstart) > CAN_TIMEOUT_VALUE)
			return OOF_INIT_CAN;
	}

	// Enable configuration change
	SET_BIT(can->CCCR, FDCAN_CCCR_CCE);

	CLEAR_BIT(can->CCCR, FDCAN_CCCR_DAR);  //AutoRetransmission Enabled
	//SET_BIT(can->CCCR, FDCAN_CCCR_DAR);  //AutoRetransmission Disabled

	SET_BIT(can->CCCR, FDCAN_CCCR_TXP);    //transmit pause Enabled
	//CLEAR_BIT(can->CCCR, FDCAN_CCCR_TXP);//transmit pause Disabled

	CLEAR_BIT(can->CCCR, FDCAN_CCCR_PXHD); //Protocol Exception Handling Enabled
	//SET_BIT(can->CCCR, FDCAN_CCCR_PXHD); //Protocol Exception Handling  Disabled

	// Set FDCAN Frame Format
	MODIFY_REG(can->CCCR, FDCAN_FRAME_FD_BRS, FDCAN_FRAME_FD_BRS);

	// Reset FDCAN Operation Mode
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
	can->NBTP = ((((uint32_t) CAN_NOMINAL_SYNC_JUMP_WIDTH - 1U) << FDCAN_NBTP_NSJW_Pos) | (((uint32_t) CAN_NOMINAL_TIMESEG_1 - 1U) << FDCAN_NBTP_NTSEG1_Pos)
			| (((uint32_t) CAN_NOMINAL_TIMESEG_2 - 1U) << FDCAN_NBTP_NTSEG2_Pos) | (((uint32_t) CAN_NOMINAL_PRESCALER - 1U) << FDCAN_NBTP_NBRP_Pos));

	// Bit Rate Switching Enable
	can->DBTP = ((((uint32_t) CAN_DATA_SYNC_JUMP_WIDTH - 1U) << FDCAN_DBTP_DSJW_Pos) | (((uint32_t) CAN_DATA_TIMESEG_1 - 1U) << FDCAN_DBTP_DTSEG1_Pos)
			| (((uint32_t) CAN_DATA_TIMESEG_2 - 1U) << FDCAN_DBTP_DTSEG2_Pos) | (((uint32_t) CAN_DATA_PRESCALER - 1U) << FDCAN_DBTP_DBRP_Pos));

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
	Result_t result = FDCAN_CalcultateRamBlockAddresses(can);
	if (result != NOICE)
		return result;

	//Config TxDelayCompensation
	can->TDCR = ((CAN_TDC_FILTER << FDCAN_TDCR_TDCF_Pos) | (CAN_TDC_OFFSET << FDCAN_TDCR_TDCO_Pos));

	// Enable transmitter delay compensation
	SET_BIT(can->DBTP, FDCAN_DBTP_TDC);

	// Configure global filter to reject everything
	can->GFC = ((FDCAN_REJECT << FDCAN_GFC_ANFS_Pos) | (FDCAN_REJECT << FDCAN_GFC_ANFE_Pos) | (FDCAN_FILTER_REMOTE << FDCAN_GFC_RRFS_Pos) | (FDCAN_REJECT_REMOTE << FDCAN_GFC_RRFE_Pos));

	//Can Start
    CLEAR_BIT(can->CCCR, FDCAN_CCCR_INIT);

	Systick_BusyWait(100);

}

void Can_InitGPIO(void)
{

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

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

	/**FDCAN2 GPIO Configuration
	 PB12     ------> FDCAN2_RX
	 PB13     ------> FDCAN2_TX
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_9;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

Result_t Can_Init(uint8_t node_id)
{
	Result_t result = OOF;
	Can_InitGPIO();

	result = Fdcan_Init(FDCAN1);
	if (result != NOICE)
		return result;
	result = Fdcan_Init(FDCAN2);
	if (result != NOICE)
		return result;




	return NOICE;
}

HAL_StatusTypeDef Can_AddStdFilter(FDCAN_GlobalTypeDef *can, uint32_t filter_index, uint32_t mask, uint32_t filter_id, uint32_t target_location)
{




	FDCAN_FilterTypeDef filter;
	filter.IdType = FDCAN_STANDARD_ID;
	filter.FilterIndex = filter_index;
	filter.FilterType = FDCAN_FILTER_MASK;
	filter.FilterConfig = target_location;
	filter.FilterID1 = filter_id;
	filter.FilterID2 = mask;

	uint32_t FilterElementW1;
	  uint32_t FilterElementW2;
	  uint32_t *FilterAddress;
	  HAL_FDCAN_StateTypeDef state = hfdcan->State;


	    if (sFilterConfig->IdType == FDCAN_STANDARD_ID)
	    {

	      // Build filter element
	      if (sFilterConfig->FilterConfig == FDCAN_FILTER_TO_RXBUFFER)
	      {
	        FilterElementW1 = ((FDCAN_FILTER_TO_RXBUFFER << 27U)       |
	                           (sFilterConfig->FilterID1 << 16U)       |
	                           (sFilterConfig->IsCalibrationMsg << 8U) |
	                           sFilterConfig->RxBufferIndex);
	      }
	      else
	      {
	        FilterElementW1 = ((sFilterConfig->FilterType << 30U)   |
	                           (sFilterConfig->FilterConfig << 27U) |
	                           (sFilterConfig->FilterID1 << 16U)    |
	                           sFilterConfig->FilterID2);
	      }

	      /* Calculate filter address */
	      FilterAddress = (uint32_t *)(hfdcan->msgRam.StandardFilterSA + (sFilterConfig->FilterIndex * 4U));

	      /* Write filter element to the message RAM */
	      *FilterAddress = FilterElementW1;
	    }


	return HAL_FDCAN_ConfigFilter(handle, &filter);

}


inline uint32_t Can_GetRxFifoFillLevel(FDCAN_GlobalTypeDef *can, uint32_t RxFifo)
{
	return can->Instance->RXF0S & ((RxFifo == FDCAN_RX_FIFO0)? FDCAN_RXF0S_F0FL : FDCAN_RXF1S_F1FL);
}

void Can_checkFifo(FDCAN_GlobalTypeDef *can)
{
	FDCAN_HandleTypeDef *handle = (can == FDCAN1) ? &hfdcan1 : &hfdcan2;

	while (Can_GetRxFifoFillLevel(can, FDCAN_RX_FIFO0) > 0)
	{
		Serial_print((can == FDCAN1) ? "FDCAN1\n" : "FDCAN2\n");

		FDCAN_RxHeaderTypeDef canHeader;
		uint8_t canData[64];

		if (HAL_FDCAN_GetRxMessage(handle, FDCAN_RX_FIFO0, &canHeader, canData) == HAL_OK)
		{
			if (canHeader.RxFrameType == FDCAN_REMOTE_FRAME)
			{
				Serial_print("REMOTE\n");
			}
			else
			{
				Can_processStandardMessage(canHeader.Identifier, canData, DLCtoBytes[canHeader.DataLength >> 16]);
			}
		}
	}

}

Result_t Can_processStandardMessage(uint32_t message_id, uint8_t *data, uint32_t length)
{

	uint8_t buffer = ((data[0] >> 6) & 0x3);
	uint8_t channel = (data[0] & 0x3F);
	Serial_println("CAN NOICE\n");

	char send_buffer[1024] =
	{ 0 };
	sprintf(send_buffer + strlen(send_buffer), "message id :  %ld, 0x%03lx\n", message_id, message_id);

	for (uint32_t c = 0; c < length; c++)
		sprintf(send_buffer + strlen(send_buffer), "%ld :  0x%02x\n", c, data[c]);

	Serial_println(send_buffer);

	Result_t result = OOF;

	if (buffer == 0)
	{
		uint8_t cmd_id = data[1];

		if (channel < CHANNEL_TYPE_LAST && channel_type_array[channel].cmds != 0)
		{
			const can_function *channel_cmds_array = channel_type_array[channel].cmds;
			const uint8_t last_index = channel_type_array[channel].last_index;
			if (cmd_id < last_index && channel_cmds_array[cmd_id] != NULL)
				result = channel_cmds_array[cmd_id]((uint32_t*) &data[2]);

		}
		//TODO else ERROR
	}
	else
	{
		uint32_t timestamp = (uint32_t) data[1];
		uint8_t cmd_id = data[5];

		timestamp = cmd_id;					//Just here to get rid of warnings
		cmd_id = timestamp;					//Just here to get rid of warnings
	}
	return result;
}

void Can_sendPacket(uint8_t channel_id, uint8_t cmd_id, uint8_t *data, uint32_t length)
{

}

Result_t Can_sendMessage(FDCAN_GlobalTypeDef *caninstance, uint32_t message_id, uint8_t *data, uint32_t length)
{
	FDCAN_HandleTypeDef *handle = (caninstance == FDCAN1) ? &hfdcan1 : &hfdcan2;

	if (length > 64)
		return OOF;
	FDCAN_TxHeaderTypeDef pHeader;
	pHeader.Identifier = (message_id & 0x07FF);
	pHeader.IdType = FDCAN_STANDARD_ID;
	pHeader.TxFrameType = FDCAN_DATA_FRAME;
	pHeader.DataLength = DLC_table[length];
	pHeader.ErrorStateIndicator = FDCAN_ESI_PASSIVE;
	pHeader.BitRateSwitch = FDCAN_BRS_ON;
	pHeader.FDFormat = FDCAN_FD_CAN;
	pHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; //FDCAN_STORE_TX_EVENTS
	pHeader.MessageMarker = 0;
	Serial_println("Send CAN MESSAGE");
	return (HAL_FDCAN_AddMessageToTxFifoQ(handle, &pHeader, data) == HAL_OK) ? NOICE : OOF;
}

