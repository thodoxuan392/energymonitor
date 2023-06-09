/*
 * app_uart.c
 *
 *  Created on: Nov 23, 2019
 *      Author: VAIO
 */
#include "main.h"
#include "app_uart.h"
#include "app_string.h"
#include "app_relay.h"







/* UART handler declaration */
UART_HandleTypeDef Uart1Handle;
UART_HandleTypeDef Uart4Handle;
UART_HandleTypeDef Uart3Handle;

/* Buffer used for UART1 */
uint8_t UART1_buffer[RXBUFFERSIZE];
uint8_t UART1_receiveBufferIndexHead = 0;
uint8_t UART1_receiveBufferIndexTail = 0;
__IO ITStatus UART1_TransmitReady = SET;
__IO ITStatus UART1_ReceiveReady = RESET;

/* Buffer used for UART4 */
uint8_t UART4_buffer[RXBUFFERSIZE];
uint8_t UART4_receiveBufferIndexHead = 0;
uint8_t UART4_receiveBufferIndexTail = 0;
__IO ITStatus UART4_TransmitReady = SET;
__IO ITStatus UART4_ReceiveReady = RESET;



HAL_StatusTypeDef Sim3g_Receive_Setup(void);

void UART1_Init(void){
	Uart1Handle.Instance          = USART1;

	Uart1Handle.Init.BaudRate     = 115200;
	Uart1Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	Uart1Handle.Init.StopBits     = UART_STOPBITS_1;
	Uart1Handle.Init.Parity       = UART_PARITY_NONE;
	Uart1Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	Uart1Handle.Init.Mode         = UART_MODE_TX_RX;

	if(HAL_UART_DeInit(&Uart1Handle) != HAL_OK){
		Error_Handler();
	}
	if(HAL_UART_Init(&Uart1Handle) != HAL_OK){
		Error_Handler();
	}
	Sim3g_Receive_Setup();
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
void UART3_Init(void)
{
	Uart3Handle.Instance = USART3;
	Uart3Handle.Init.BaudRate = 115200;
	Uart3Handle.Init.WordLength = UART_WORDLENGTH_8B;
	Uart3Handle.Init.StopBits = UART_STOPBITS_1;
	Uart3Handle.Init.Parity = UART_PARITY_NONE;
	Uart3Handle.Init.Mode = UART_MODE_TX_RX;
	Uart3Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	Uart3Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&Uart3Handle) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * @brief USART4 Initialization Function
  * @param None
  * @retval None
  */
void UART4_Init(void)
{
	Uart4Handle.Instance = UART4;
	Uart4Handle.Init.BaudRate = 115200;
	Uart4Handle.Init.WordLength = UART_WORDLENGTH_8B;
	Uart4Handle.Init.StopBits = UART_STOPBITS_1;
	Uart4Handle.Init.Parity = UART_PARITY_NONE;
	Uart4Handle.Init.Mode = UART_MODE_TX_RX;
	Uart4Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	Uart4Handle.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_DeInit(&Uart4Handle) != HAL_OK){
		Error_Handler();
	}
	if (HAL_UART_Init(&Uart4Handle) != HAL_OK) {
		Error_Handler();
	}
	HAL_UART_Receive_IT(&Uart4Handle, (uint8_t *)UART4_buffer, RXBUFFERSIZE);
}

HAL_StatusTypeDef Sim3g_Receive_Setup(void){
	if(HAL_UART_Receive_IT(&Uart1Handle, (uint8_t *)UART1_buffer, RXBUFFERSIZE) != HAL_OK){
		return HAL_ERROR;
	}
	return HAL_OK;

}


void UART3_SendToHost(uint8_t * buffer){
	uint8_t buffer_len = GetStringLength((uint8_t *) buffer);
	if(HAL_UART_Transmit(&Uart3Handle, (uint8_t *)buffer, buffer_len, 0xFFFFFFFF) == HAL_TIMEOUT){
		uint8_t a = 1;
	}
}


void UART3_Transmit(uint8_t * buffer, uint8_t buffer_len){
	HAL_UART_Transmit(&Uart3Handle, (uint8_t *)buffer, buffer_len, 0xFFFFFFFF);
	return;
}

void UART1_Transmit(uint8_t * buffer){
	uint8_t buffer_len = GetStringLength((uint8_t*)buffer);
	if(buffer_len == 0) {
		return;
	} else {
		if(HAL_UART_Transmit_IT(&Uart1Handle, (uint8_t*)buffer, buffer_len)!= HAL_OK){
			Error_Handler();
		}
		UART1_TransmitReady = RESET;
	}
	return;
}

void Sim3g_Transmit(uint8_t * buffer, uint8_t buffer_len){
	if(buffer_len == 0) {
		return;
	} else {
		if(HAL_UART_Transmit_IT(&Uart1Handle, (uint8_t*)buffer, buffer_len)!= HAL_OK){
			Error_Handler();
		}
		UART1_TransmitReady = RESET;
	}
	return;
}


/****************************************Callback Function*******************************************/

HAL_StatusTypeDef Custom_UART_Receive_IT(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		  /* Check that a Rx process is ongoing */
		  if (huart->RxState == HAL_UART_STATE_BUSY_RX)
		  {
			  huart->ErrorCode = HAL_UART_ERROR_NONE;
			  huart->RxState = HAL_UART_STATE_BUSY_RX;
			  UART1_buffer[UART1_receiveBufferIndexHead] = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
			  UART1_receiveBufferIndexHead = (UART1_receiveBufferIndexHead + 1) % RXBUFFERSIZE;
			  return HAL_OK;
		  } else {
		    return HAL_BUSY;
		  }
	}else if(huart->Instance == UART4){
		  /* Check that a Rx process is ongoing */
		  if (huart->RxState == HAL_UART_STATE_BUSY_RX)
		  {
			  huart->ErrorCode = HAL_UART_ERROR_NONE;
			  huart->RxState = HAL_UART_STATE_BUSY_RX;
			  UART4_buffer[UART4_receiveBufferIndexHead] = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
			  UART4_receiveBufferIndexHead = (UART4_receiveBufferIndexHead + 1) % RXBUFFERSIZE;
			  return HAL_OK;
		  } else {
		    return HAL_BUSY;
		  }
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	/* Set transmission flag: transfer complete */
	if(huart->Instance == USART1){
		UART1_TransmitReady = SET;
	}else if(huart->Instance == UART4){
		UART4_TransmitReady = SET;
	}

}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle){
    Error_Handler();
}

uint8_t Uart1_Received_Buffer_Available(void){
	if(UART1_receiveBufferIndexTail != UART1_receiveBufferIndexHead){
		return 1;
	} else {
		return 0;
	}
}
uint8_t Uart1_Read_Received_Buffer(void){
	uint8_t buffer[2];
	if(UART1_receiveBufferIndexTail == UART1_receiveBufferIndexHead) return 0xff;
	uint8_t ch = UART1_buffer[UART1_receiveBufferIndexTail];
	UART1_receiveBufferIndexTail = (UART1_receiveBufferIndexTail + 1) % RXBUFFERSIZE;
	buffer[0] = ch;
	buffer[1] = 0;
	UART3_SendToHost(buffer);
	return ch;
}


void ATcommandSending(uint8_t * buffer){
	if(isSim3gTransmissionReady() == SET){
		UART1_Transmit(buffer);
	}
}

void MQTTCommandSending(uint8_t * buffer, uint8_t buffer_len){
	if(isSim3gTransmissionReady() == SET){
		Sim3g_Transmit(buffer, buffer_len);
	}
}


ITStatus isSim3gReceiveReady(){

	if(UART1_ReceiveReady == SET){
		UART1_ReceiveReady = RESET;
		return SET;
	} else {
		return RESET;
	}
}
ITStatus isSim3gTransmissionReady(void){

	if(UART1_TransmitReady == SET){
		UART1_TransmitReady = RESET;

		return SET;
	} else {
		return RESET;
	}

}

void UART4_transmit(uint8_t * data, uint32_t data_len){
	UART3_Transmit(data, data_len);
	if(UART4_TransmitReady){
		if(HAL_UART_Transmit_IT(&Uart4Handle, (uint8_t*)data, data_len)!= HAL_OK){
			Error_Handler();
		}
		UART4_TransmitReady = RESET;
	}
}

uint8_t UART4_Read_Available(){
	if(UART4_receiveBufferIndexTail != UART4_receiveBufferIndexHead){
		return 1;
	} else {
		return 0;
	}
}

uint8_t UART4_Read_Received_Buffer(){
	uint8_t buffer[2];
	if(UART4_receiveBufferIndexTail == UART4_receiveBufferIndexHead) return 0xff;
	uint8_t ch = UART4_buffer[UART4_receiveBufferIndexTail];
	UART4_receiveBufferIndexTail = (UART4_receiveBufferIndexTail + 1) % RXBUFFERSIZE;
	buffer[0] = ch;
	buffer[1] = 0;
	UART3_SendToHost(buffer);
	return ch;
}





