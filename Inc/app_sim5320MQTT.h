/*
 * app_sim5320MQTT.h
 *
 *  Created on: Dec 17, 2019
 *      Author: VAIO
 */

#ifndef APP_SIM5320MQTT_H_
#define APP_SIM5320MQTT_H_

#include "main.h"

#include "app_uart.h"
#include "app_sim5320MQTT.h"

#define MQTT_MSG_CONNECT        0x10
#define MQTT_MSG_CONNACK        0x20
#define MQTT_MSG_PUBLISH        0x30
#define MQTT_SUBSCRIBE          0x82
#define MQTT_SUBACK             0x90
#define MQTT_PINGREQ            0xc0
#define MQTT_PINGRESP           0xd0

#define CR                      '\r'        // 0x0D
#define LF                      '\n'        // 0x0A

typedef enum {
	MQTT_OPEN_STATE = 0,
	MQTT_WAIT_FOR_RESPONSE_FROM_OPEN_STATE,
	MQTT_CONNECT_STATE,
	MQTT_WAIT_FOR_RESPONSE_FROM_CONNECT_STATE,
	MQTT_SUBSCRIBE_STATE,
	MQTT_WAIT_FOR_RESPONSE_FROM_SUBSCRIBE_STATE,
	MQTT_PUBLISH_STATE,
	MQTT_WAIT_FOR_RESPONSE_FROM_PUBLISH_STATE,
	MQTT_PING_REQUEST_STATE,
	MQTT_WAIT_FOR_RESPONSE_FROM_REQUEST_STATE,
	MQTT_DISCONNECT,
	MQTT_WAIT_FOR_DISCONNECT_RESPONSE,
	MAX_MQTT_NUMBER_STATES
} MQTT_STATE;

typedef struct {
	MQTT_STATE state;
	void (*func)(void);
}MQTT_Machine_Type;


void MQTT_Run(void);

#endif /* APP_SIM5320MQTT_H_ */
