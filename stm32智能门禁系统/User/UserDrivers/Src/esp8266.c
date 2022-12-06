/*
 * esp8266.c
 *
 *  Created on: 2021年10月12日
 *      Author: Naming8104
 */
#include "esp8266.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

#define AT_CMD_MAXLEN 256

static uint8_t UART1_RxBuffer[RXBUFFERSIZE]; //接收数据
static uint8_t UART1_aRxBuffer;              //接收中断缓冲
static uint8_t UART1_Rx_Cnt;                 //接收缓冲计数

uint8_t esp_ready_flag = 0;         // ESP模块是否准备好接收下一条命令

static void (* s_espRxCpltCallback)(const char* RxBuffer);

/**
 * @brief 发送AT命令
 * 
 * @param cmd 命令，最大长度256字节
 * @param timeout 串口超时时间
 */
void ESP_Send_AT_Cmd(char *cmd, uint32_t timeout)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)cmd, strlen(cmd), timeout);
}

/**
 * @brief ESP8266初始化
 */
void ESP_Init(void)
{
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&UART1_aRxBuffer, 1); //打开串口1第一次中断

  //使能
  HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, GPIO_PIN_SET);
  //重置
  HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_SET);
}

/**
 * @brief 注册回调函数
 */
void ESP_registerCallback(void (* espRxCpltCallback)(const char* RxBuffer))
{
  s_espRxCpltCallback = espRxCpltCallback;
}

/**
 * @brief 软件重置
 */
void ESP_Reset(void)
{
  ESP_Send_AT_Cmd("AT+RESTORE\r\n", 2000);
}
/**
 * @brief 设置 Wi-Fi 模式
 * 
 * @param mode  0: Null模式, WiFi RF将被禁用
                1: Station模式
                2: SoftAP模式
                3: SoftAP+Station模式
 */
void ESP_CWMODE(uint8_t mode)
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+CWMODE=%d\r\n", mode);
  ESP_Send_AT_Cmd(cmd, 200);
}
/**
 * @brief 与 AP 断开连接
 * 
 */
void ESP_CWQAP()
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+CWQAP\r\n");
  ESP_Send_AT_Cmd(cmd, 200);
}
/**
 * @brief 连接到 AP
 * 
 * @param wifi_ssid 目标 AP 的 SSID
 * @param wifi_pwd 密码, 最大: 64字节 ASCII.
 */
void ESP_CWJAP(const char* wifi_ssid, const char* wifi_pwd)
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifi_ssid, wifi_pwd);
  ESP_Send_AT_Cmd(cmd, 200);
}
/**
 * @brief 设置 MQTT 用户配置
 * 
 * @param clientid MQTT 客户端 ID，最大长度 256 字节
 * @param username 登录 MQTT Broker 的用户名，最大长度 64 字节
 * @param password 登录 MQTT Broker 的密码，最大长度 64 字节
 */
void ESP_MQTTUSERCFG(const char* clientid, const char* username, const char* password)
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", clientid, username, password);
  ESP_Send_AT_Cmd(cmd, 200);
}
/**
 * @brief 设置 MQTT 连接的配置
 * 
 * @param keepalive MQTT ping 的超时时间，范围[60, 7200]，单位：秒。 默认为 120 秒。
 * @param lwt_topic LWT（遗嘱）消息 Topic，最大长度 64 字节
 * @param lwt_msg LWT 消息，最大长度 64 字节
 */
void ESP_MQTTCONNCFG(uint16_t keepalive, const char* lwt_topic, const char* lwt_msg)
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+MQTTCONNCFG=0,%d,0,\"/%s\",\"%s\",0,0\r\n", keepalive, lwt_topic, lwt_msg);
  ESP_Send_AT_Cmd(cmd, 200);
}
/**
 * @brief 连接到 MQTT Broker
 * 
 * @param hostname MQTT Broker 域名，最大长度 128 字节
 * @param port MQTT Broker 端口, 端口最大值是 65535
 */
void ESP_MQTTCONN(const char* hostname, uint16_t port)
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+MQTTCONN=0,\"%s\",%d,1\r\n", hostname, port);
  ESP_Send_AT_Cmd(cmd, 200);
}
/**
 * @brief 订阅到 MQTT Topic
 * 
 * @param topic 订阅的 Topic
 */
void ESP_MQTTSUB(const char* topic)
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+MQTTSUB=0,\"%s\",0\r\n", topic);
  ESP_Send_AT_Cmd(cmd, 200);
}
/**
 * @brief 以字符串形式发布 MQTT 消息
 * 
 * @param topic MQTT Topic，最大长度 64 字节
 * @param data 字符串形式的 MQTT 消息
 */
void ESP_MQTTPUB(const char* topic, const char* data)
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+MQTTPUB=0,\"%s\",\"%s\",0,0\r\n", topic, data);
  ESP_Send_AT_Cmd(cmd, 200);
}
/**
 * @brief 以二进制形式发布 MQTT 消息
 * 
 * @param topic MQTT Topic，最大长度 64 字节
 * @param length MQTT 消息长度，默认最大长度为 1024 
 * @param qos 发布消息的 qos，可设置为 0、1 或 2。默认为 0
 * @param retain 链接保持标志
 */
void ESP_MQTTPUBRAW(const char* topic, uint16_t length, uint8_t qos, uint8_t retain)
{
  char cmd[AT_CMD_MAXLEN];
  sprintf(cmd, "AT+MQTTPUBRAW=0,\"%s\",%d,%d,%d\r\n", topic, length, qos, retain);
  ESP_Send_AT_Cmd(cmd, 200);
}

void ESP_UART_Rx_Callback(void)
{
  if (UART1_Rx_Cnt >= 255) //溢出判断
  {
    RTT_printf((const char *)&UART1_RxBuffer); //通过RTT发送到终端
    UART1_Rx_Cnt = 0;
  }
  else
  {
    UART1_RxBuffer[UART1_Rx_Cnt++] = UART1_aRxBuffer; //接收数据转存

    if ((UART1_RxBuffer[UART1_Rx_Cnt - 1] == 0x0A) && (UART1_RxBuffer[UART1_Rx_Cnt - 2] == 0x0D)) //判断结束位
    {
      UART1_RxBuffer[UART1_Rx_Cnt] = '\0';
      
      RTT_printf("[ESP] %s", (const char *)&UART1_RxBuffer); //通过RTT发送到终端
      
      if (s_espRxCpltCallback) s_espRxCpltCallback((const char *)UART1_RxBuffer);//回调函数

      if (strstr((const char *)UART1_RxBuffer, "OK"))//命令成功
      {
        esp_ready_flag = 1; //模块已准备接受下一条指令
      }
      else if (strstr((const char *)UART1_RxBuffer, "ready"))//初始化完成
      {
        esp_ready_flag = 1;
      }
      else if (strstr((const char *)UART1_RxBuffer, "+MQTTSUBRECV"))//MQTT订阅消息
      {
        esp_ready_flag = 1;
      }
      // else if (strstr((const char *)UART1_RxBuffer, "FAIL"))//命令失败
      // {
      //   esp_ready_flag = 1;
      // }
      // else if (strstr((const char *)UART1_RxBuffer, "ERROR"))//命令错误
      // {
      //   esp_ready_flag = 1;
      // }
      
      UART1_Rx_Cnt = 0;
    }
  }
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&UART1_aRxBuffer, 1); //再开启接收中断
}

