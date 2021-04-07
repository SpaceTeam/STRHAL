#include "serial.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define QUEUE_SIZE (1UL<<10)
typedef volatile struct
{
	char Array[QUEUE_SIZE];
	uint16_t QueueOut, QueueIn;
} Queue;

Queue rxQueue = {
		0 };

void QueueInit(Queue *queue)
{
	queue->QueueIn = queue->QueueOut = 0;
}
void QueuePut(Queue *queue, char new)
{
	queue->Array[queue->QueueIn] = new;
	queue->QueueIn = (queue->QueueIn + 1) & (QUEUE_SIZE - 1);
}
char QueueGet(Queue *queue)
{
	char return_value = queue->Array[queue->QueueOut];
	queue->QueueOut = (queue->QueueOut + 1) & (QUEUE_SIZE - 1);
	return return_value;
}

char rxString[128] = "";
char rxData[128] = "";
char tx_buffer[2048] = "";

void Serial_Init(void)
{
	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	/**UART4 GPIO Configuration
	 PC10   ------> UART4_TX
	 PC11   ------> UART4_RX
	 */
	LL_GPIO_InitTypeDef GPIO_InitStruct = {
			0 };
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* UART4 interrupt Init */
	NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_EnableIRQ(UART4_IRQn);

	LL_USART_InitTypeDef UART_InitStruct = {
			0 };
	UART_InitStruct.BaudRate = 115200;
	UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	UART_InitStruct.Parity = LL_USART_PARITY_NONE;
	UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(UART4, &UART_InitStruct);
	LL_USART_ConfigAsyncMode(UART4);
	//LL_USART_EnableFIFO(UART4);			//FIFO
	LL_USART_EnableDirectionTx(UART4);
	LL_USART_EnableDirectionRx(UART4);

	LL_USART_EnableDMAReq_TX(UART4);	//TX DMA

	LL_USART_Enable(UART4);

	//TX DMA Init
	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2);
	LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_2);
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_2, LL_DMAMUX1_REQ_UART4_TX);
	LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_2, LL_DMA_PRIORITY_HIGH);
	LL_DMA_SetMode(DMA1, LL_DMA_STREAM_2, LL_DMA_MODE_NORMAL);
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_2, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_2, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_2, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_2, LL_DMA_MDATAALIGN_BYTE);
	LL_DMA_SetPeriphBurstxfer(DMA1, LL_DMA_STREAM_2, LL_DMA_PBURST_SINGLE);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_2, LL_USART_DMA_GetRegAddr(UART4, LL_USART_DMA_REG_DATA_TRANSMIT));

	LL_USART_EnableIT_RXNE_RXFNE(UART4);

	LL_USART_ClearFlag_TC(UART4);

	QueueInit(&rxQueue);

}

void Serial_Send(char *string)
{
	LL_USART_ClearFlag_TC(UART4);
	LL_DMA_ClearFlag_TC2(DMA1);
	LL_DMA_ClearFlag_TE2(DMA1);

	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_2, LL_USART_DMA_GetRegAddr(UART4, LL_USART_DMA_REG_DATA_TRANSMIT));
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_2, (uint32_t) string);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, strlen(string));
	LL_USART_ClearFlag_TC(UART4);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);

}

void Serial_Print(char _out[])
{
	while (LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_2))
	{
	};
	strcpy(tx_buffer, _out);
	Serial_Send(tx_buffer);
}

void Serial_Println(char _out[])
{
	while (LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_2))
	{
	};
	strcpy(tx_buffer, _out);
	strcat(tx_buffer, "\r\n");
	Serial_Send(tx_buffer);
}

void Serial_PrintInt(uint32_t val)
{
	char buffer[100];
	sprintf(buffer, "%d\r\n", val);
	strcpy(tx_buffer, buffer);
	Serial_Send(tx_buffer);
}
uint8_t Serial_CheckInput(char *message)
{
	static uint32_t receive_index = 0;
	char new_char;

	while (rxQueue.QueueOut != rxQueue.QueueIn)
	{
		new_char = QueueGet(&rxQueue);

		if (new_char == SERIAL_DELIMITER)				// Carriage return as Delimiter
		{
			message[receive_index] = 0;	//ADD String Delimiter
			receive_index = 0;
			return 1;
		}
		else if (new_char != LINE_FEED)
		{
			if (receive_index < (RECEIVE_STRING_SIZE - 1))
			{
				message[receive_index] = new_char;
				receive_index++;
			}
		}

	}
	return 0;
}

void UART4_IRQHandler(void)
{
	if (LL_USART_IsActiveFlag_RXNE_RXFNE(UART4))
	{
		QueuePut(&rxQueue, LL_USART_ReceiveData8(UART4));
	}

}
