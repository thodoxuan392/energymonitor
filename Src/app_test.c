/*
 * app_test.c
 *
 *  Created on: Feb 12, 2020
 *      Author: VAIO
 */
#include <app_i2c_lcd.h>
#include "main.h"

#include "app_relay.h"
#include "app_led_display.h"
#include "app_sim3g.h"
#include "app_uart.h"
#include "app_pcf8574.h"
#include "app_scheduler.h"
#include "app_gpio.h"
#include "app_eeprom.h"


extern uint8_t SUBSCRIBE_TOPIC_1[MAX_TOPIC_LENGTH];
extern uint8_t SUBSCRIBE_TOPIC_2[MAX_TOPIC_LENGTH];
extern uint8_t PUBLISH_TOPIC_STATUS[MAX_TOPIC_LENGTH];
extern uint8_t PUBLISH_TOPIC_FOTA_STATUS[MAX_TOPIC_LENGTH];
extern uint8_t PUBLISH_TOPIC_POWER[MAX_TOPIC_LENGTH];
uint8_t strTest[] = "                                                                                        ";
void test1(void){


	static uint8_t bool2 = 0;
	static uint8_t index2 = 0;

	sprintf((char*) strTest, "test1 %d\r\n", (int) index2);
	UART3_SendToHost((uint8_t *)strTest);
	if(bool2 == 0){
		Set_Relay(index2);
	} else {
		Reset_Relay(index2);
	}
	if(index2 == 9) {
		bool2 = (bool2 + 1)%2;
		index2 = 0;
	} else {
		index2 ++;
	}
}
void test3(void){
	static int test3Counter = 0;
	sprintf((char*) strTest, "seconds = %d\r\n", (int) test3Counter++);
	UART3_SendToHost((uint8_t *)strTest);
	Test_Led_Display();
}

void test2(void){
	HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
}

void test4(void){
	static uint8_t poweron = 0;
	if(poweron == 0){
		poweron = 1;
		Power_Signal_High();
	} else {
		poweron = 0;
		Power_Signal_Low();
	}
}

void test5(void){
	uint16_t boxID;
	boxID = Get_Box_ID();
	sprintf((char*) strTest, "boxID = %d\r\n", (int) boxID);
	UART3_SendToHost((uint8_t *)strTest);
}
void test8(void){
	UART3_SendToHost((uint8_t *)"test8\r\n");
}
void test6(void){

	SCH_Add_Task(test1, 0, 1000);
	HAL_Delay(10);
	SCH_Add_Task(test3, 2, 100);
	HAL_Delay(10);
	SCH_Add_Task(test2, 3, 20);
	HAL_Delay(10);
	SCH_Add_Task(test5, 3, 200);

	HAL_Delay(10);
	SCH_Add_Task(test8, 7, 21);
}

void test7(void){
	TestSendATcommand();
}

void test9(void){
	static uint8_t rw = 0;
	static uint8_t counterWrite = 0;
	uint8_t i = 0,s;
	uint32_t e, l,w;
	if(rw == 0){
		counterWrite = Read_First_Byte();
		sprintf((char*) strTest, "readfirst = %d\r\n", (int) counterWrite);
		UART3_SendToHost((uint8_t *)strTest);
		rw = 1;
	} else if (rw == 1){
		Write_First_Byte(counterWrite++);
		sprintf((char*) strTest, "readsecond = %d\r\n", (int) Read_First_Byte());
		UART3_SendToHost((uint8_t *)strTest);
		rw = 3;
	} else {
		Eeprom_Read_Outlet(0, &s, &e, &l, &w);
		sprintf((char*) strTest, "i:%d\t s:%d\t e:%d\t l:%d\t \r\n", (int) i, (int)s, (int)e, (int)l);
		UART3_SendToHost((uint8_t *)strTest);
		rw = 0;
	}
}

void Test10(void){
	uint8_t i = 0,s;
	uint32_t e, l, w;
	for(i = 0; i < 10; i ++){
		Eeprom_Read_Outlet(i, &s, &e, &l, &w);
		sprintf((char*) strTest, "i:%d\t s:%d\t e:%d\t l:%d\t \r\n", (int) i, (int)s, (int)e, (int)l);
		UART3_SendToHost((uint8_t *)strTest);
		s ++;
		e++;
		l++;
	//	HAL_Delay(1000);
		Eeprom_Write_Outlet (i, s, e, l, w);
		HAL_Delay(100);
		s = 0;
		e = 0;
		l = 0;
		Eeprom_Read_Outlet(i, &s, &e, &l, &w);
		sprintf((char*) strTest, "i:%d\t s:%d\t e:%d\t l:%d\t \r\n", (int) i, (int)s, (int)e, (int)l);
		UART3_SendToHost((uint8_t *)strTest);
	}
}

void Test11(void){

	for(uint8_t i = 0; i < 10; i ++){
		Eeprom_Write_Outlet (i, 0, 0, 0, 0);
		HAL_Delay(100);
		Eeprom_Update_LimitEnergy(i, 0);
//		HAL_Delay(1000);
	}
}


void Test12(void){
	Lcd_Goto_XY(1,0);
	Lcd_Send_String("Nguyen");
}
