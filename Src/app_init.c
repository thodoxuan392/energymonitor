/*
 * app_init.c
 *
 *  Created on: Nov 23, 2019
 *      Author: VAIO
 */
#include "main.h"
#include "app_adc.h"
#include "app_accel.h"
#include "app_flash_database.h"
#include "app_gpio.h"
#include "app_led_display.h"
#include "app_sim3g.h"
#include "app_temperature.h"
#include "app_timer.h"
#include "app_uart.h"
#include "app_system_clock_config.h"
#include "app_sim3g.h"
#include "app_spi.h"
#include "app_i2c.h"
#include "app_25LC512.h"
#include "app_init.h"




enum InitState initState = HAL_INIT;
void Sys_Init(void){

	HAL_Init();
	SystemClock_Config();

	UART3_Init();
	UART1_Init();
	DEBUG_INIT(printf("UART_INIT - Done \r\n"));
	MX_GPIO_Init();
	DEBUG_INIT(printf("GPIO_INIT - ADC_DMA_Init - Done \r\n"));

	Timer_Init();
   	DEBUG_INIT(printf("TIMER_INIT - Done \r\n"));

}
void System_Initialization(void)
{
    while(initState != MAX_INIT_STATE){
        switch (initState) {
        case HAL_INIT:
        	HAL_Init();
            break;
        case SYSTEM_CLOCK_INIT:
        	SystemClock_Config();
            break;
        case UART_INIT:
        	UART3_Init();
        	UART1_Init();
        	DEBUG_INIT(printf("UART_INIT - Done \r\n"));
        	break;
        case GPIO_INIT:
        	MX_GPIO_Init();
        	DEBUG_INIT(printf("GPIO_INIT - ADC_DMA_Init - Done \r\n"));
            break;
        case LED_DISPLAY_INIT:
        	Led_Display_Init();
        	DEBUG_INIT(printf("LED_DISPLAY_INIT - Done \r\n"));
        	break;
        case RELAY_INIT:
        	Relay_Init();
        	DEBUG_INIT(printf("RELAY_INIT - Done \r\n"));
        	break;
        case FLASH_INIT:
        	DEBUG_INIT(printf("FLASH_INIT - Done \r\n"));
        	break;
        case TIMER_INIT:
        	Timer_Init();
        	DEBUG_INIT(printf("TIMER_INIT - Done \r\n"));
            break;
        case SPI_INIT:
        	SPI1_Init();
        	SPI2_Init();
        	DEBUG_INIT(printf("SPI_INIT - Done \r\n"));
        	break;
        case SPI_25LCXXX_INIT:
        	MC25LC512_Initilize();
        	DEBUG_INIT(printf("SPI_25LCXXX_INIT - Done \r\n"));
        	break;
        case I2C_INIT:
        	I2C_Init();
        	DEBUG_INIT(printf("I2C_Init - Done \r\n"));
        	break;
        case ADC_INIT:
        	ADC_DMA_Init();
        	ADC1_Init();
        	DEBUG_INIT(printf("ADC_INIT - Done \r\n"));
        	break;
        case ACCELERATOR_INIT:
        	break;
        case WATCH_DOG_INIT:

        	DEBUG_INIT(printf("WATCH_DOG_INIT - Done \r\n"));
            break;
        case START_DMA_ADC:
        	StartGettingADCValues();
        	DEBUG_INIT(printf("START_DMA_ADC - Done \r\n"));
        	break;
        case SIM_3G_INIT:
//        	Sim3g_Init();
        	DEBUG_INIT(printf("SIM_3G_INIT - Done \r\n"));
        	break;
        default:
            initState = HAL_INIT;
            break;
        }
        initState = initState + 1;
    }
}


