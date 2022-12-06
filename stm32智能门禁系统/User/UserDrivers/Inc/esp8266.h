/*
 * esp8266.h
 *
 *  Created on: 2021年10月12日
 *      Author: Naming8104
 */

#ifndef INC_ESP8266_H_
#define INC_ESP8266_H_

#include "main.h"

extern uint8_t esp_ready_flag;

//函数声明
void ESP_Init(void);
void ESP_Reset(void);
void ESP_registerCallback(void (* subRecvCallback)(const char* reply));
void ESP_CWMODE(uint8_t mode);
void ESP_CWQAP();
void ESP_CWJAP(const char* wifi_ssid, const char* wifi_pwd);
void ESP_MQTTUSERCFG(const char* clientid, const char* username, const char* password);
void ESP_MQTTCONNCFG(uint16_t keepalive, const char* lwt_topic, const char* lwt_msg);
void ESP_MQTTCONN(const char* hostname, uint16_t port);
void ESP_MQTTSUB(const char* topic);
void ESP_MQTTPUB(const char* topic, const char* data);
void ESP_MQTTPUBRAW(const char* topic, uint16_t length, uint8_t qos, uint8_t retain);
void ESP_UART_Rx_Callback(void);

#endif /* INC_ESP8266_H_ */
