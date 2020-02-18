/*
 * app_relay.h
 *
 *  Created on: Dec 16, 2019
 *      Author: VAIO
 */

#ifndef APP_RELAY_H_
#define APP_RELAY_H_


void Relay_Init(void);

void Set_Relay(uint8_t relayIndex);
void Reset_Relay(uint8_t relayIndex);
void Update_Relay_Physical_Status(void);
FlagStatus Get_Relay_Status(uint8_t relayIndex);
void Check_Physical_Working_Statuses_Of_Relays_Fuses(uint8_t relayIndex);
void Check_All_Physical_Working_Statuses_Of_Relays_Fuses(void);


#endif /* APP_RELAY_H_ */