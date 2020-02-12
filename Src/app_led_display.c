/*
 * app_led_display.c
 *
 *  Created on: Dec 31, 2019
 *      Author: VAIO
 */
#include "main.h"



uint8_t ledPositions[20] = {
		9, 8, 11, 10, 13, 12, 15, 14, 7, 6,
		25, 24, 27, 26, 29, 28, 31, 30, 23, 22
};
uint32_t ledStatusBuffer = 0;



static void Latch_Enable(int8_t count);
static void Latch_Disable(int8_t count);
static void Output_Enable(void);
//static void Output_Disable(void);
static void Clock_On(int8_t count);
static void Clock_Off(int8_t count);


void Led_Display_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = LED_SDI;
	HAL_GPIO_Init(LED_SDI_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED_SCK;
	HAL_GPIO_Init(LED_SCK_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED_LE;
	HAL_GPIO_Init(LED_LE_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED_OE;
	HAL_GPIO_Init(LED_OE_PORT, &GPIO_InitStruct);

	Output_Enable();
	Latch_Disable(1);
	Clock_Off(1);
}


static void Latch_Enable(int8_t count){
	if(count <= 0) return;
	while(count-- != 0){
		HAL_GPIO_WritePin(LED_LE_PORT, LED_LE, GPIO_PIN_SET);
	}
}
static void Latch_Disable(int8_t count){
	if(count <= 0) return;
	while(count-- != 0){
		HAL_GPIO_WritePin(LED_LE_PORT, LED_LE, GPIO_PIN_RESET);
	}
}
static void Output_Enable(void){
	HAL_GPIO_WritePin(LED_OE_PORT, LED_OE, GPIO_PIN_RESET);
}
//static void Output_Disable(void){
//	HAL_GPIO_WritePin(LED_OE_PORT, LED_OE, GPIO_PIN_SET);
//}
static void Clock_On(int8_t count){
	if(count <= 0) return;
	while(count-- != 0){
		HAL_GPIO_WritePin(LED_SCK_PORT, LED_SCK, GPIO_PIN_SET);
	}

}
static void Clock_Off(int8_t count){
	if(count <= 0) return;
	while(count-- != 0){
		HAL_GPIO_WritePin(LED_SCK_PORT, LED_SCK, GPIO_PIN_RESET);
	}

}
static void Data_Out(GPIO_PinState state){
	HAL_GPIO_WritePin(LED_SDI_PORT, LED_SDI, state);
}


void Led_Display_Update_Buffer(uint8_t ledpos, FlagStatus status){
	if(status == SET){
		ledStatusBuffer = ledStatusBuffer & (~ (1 << ledPositions[ledpos]));
	} else {
		ledStatusBuffer = ledStatusBuffer | (1 << ledPositions[ledpos]);
	}
}

uint8_t ledCounter = 0;
void Led_Display(void){
	uint8_t i;

	for (i = 0; i < 32; i ++){
		Data_Out((ledStatusBuffer >> i) & 0x01);
		Clock_Off(4);
		Clock_On(4);
	}
	Latch_Disable(4);
	Latch_Enable(4);
	Latch_Disable(1);
}


