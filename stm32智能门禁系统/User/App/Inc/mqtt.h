/**
 * @file mqtt.h
 * @author Naming8104
 * @brief 
 * @version 0.1
 * @date 2022-04-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef INC_MQTT_H_
#define INC_MQTT_H_

#include "main.h"

#include "secret.h"

typedef struct
{
  char hostname[128];
  uint16_t port;
  //char clientid[256];
  char clientid[64];
  char username[64];
  char password[64];
} sign_mqtt_t;

void MQTT_Init();
void MQTT_Task();
void MQTT_SetNotification_DoorOpen(uint8_t *key_id, KeyType lock_type);
void MQTT_SetNotification_KeyInfo(uint8_t *key_id, KeyType lock_type);

#endif // !INC_MQTT_H_