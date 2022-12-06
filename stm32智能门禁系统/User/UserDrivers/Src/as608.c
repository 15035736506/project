/*
 * as608.c
 *
 *  Created on: 2022年3月5日
 *      Author: Naming8104
 */
#include "as608.h"

#include "usart.h"
#include "tim.h"

#include "string.h"

uint8_t finger_confirm_flag = 0;

uint8_t UART2_RxBuffer[32]; //接收数据
// uint8_t UART2_aRxBuffer;              //接收中断缓冲
// uint8_t UART2_Rx_Cnt;                 //接收缓冲计数
// uint8_t UART2_State;                  //是否可用

static uint32_t AS608Addr = 0XFFFFFFFF; //默认

static void AS608_SendPkg(uint8_t data[], size_t size)
{
  // uint8_t i = 0;
  //RTT_printf("S:");
  // for (; i < size; i++)
  // {
  //   //RTT_printf("0x%02X ", data[i]); //打印发送的数据
  //   while (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);
  //   HAL_UART_Transmit(&huart2, &data[i], 1, 1);
  // }
  //RTT_printf("\r\n");

  while (HAL_UART_GetState(&huart2) == HAL_UART_STATE_BUSY_TX);
  HAL_UART_Transmit(&huart2, data, size, 10);
}
//判断中断接收的数组有没有应答包
// waittime为等待中断接收数据的时间（单位1ms）
//返回值：数据包首地址
static uint8_t *RecvAck(uint16_t waittime, uint8_t size)
{
  uint8_t str[7];
  str[0] = 0xef;
  str[1] = 0x01;
  str[2] = AS608Addr >> 24;
  str[3] = AS608Addr >> 16;
  str[4] = AS608Addr >> 8;
  str[5] = AS608Addr;
  str[6] = '\0';
  if (HAL_UART_Receive(&huart2, UART2_RxBuffer, size, waittime) == HAL_OK)
  {
    if (strstr((const char *)UART2_RxBuffer, (const char *)str))//
    {
      //打印接收到的数据
      // RTT_printf("[AS608] R:");
      // for (i = 0; i < size; i++)
      // {
      //   RTT_printf("0x%02X ", UART2_RxBuffer[i]);
      // }
      // RTT_printf("\r\n");

      if (UART2_RxBuffer[6] == 0x07)//应答包
      {
        RTT_printf("[AS608] 应答 %s\n", EnsureMessage(UART2_RxBuffer[9]));
        return (uint8_t *)UART2_RxBuffer;
      }
    }
  }
  return 0;
}

uint8_t RecvPkg(uint8_t *data)
{
  uint8_t UART2_RxBuffer_2[1024];
  uint8_t str[7];
  uint8_t ret;
  uint16_t pkg_data_len;
  uint16_t current_data_len = 0;
  uint8_t *pos = UART2_RxBuffer_2;
  str[0] = 0xef;
  str[1] = 0x01;
  str[2] = AS608Addr >> 24;
  str[3] = AS608Addr >> 16;
  str[4] = AS608Addr >> 8;
  str[5] = AS608Addr;
  str[6] = '\0';
  ret = HAL_UART_Receive(&huart2, UART2_RxBuffer_2, 1023, 800);
  UART2_RxBuffer_2[1023] = '\0';
  RTT_printf("[AS608] 数据接收完毕 ret = %d\n", ret);
  while (1)
  {
    pos = (uint8_t *)strstr((const char *)pos, (const char *)str);
    if (pos)
    {
      pkg_data_len = ((*(pos+7))<<8) + *(pos+8);
      RTT_printf("[AS608] 当前包 包长度 = %d\n", pkg_data_len-2);

      memcpy(&(data[current_data_len]), pos+9, pkg_data_len - 2);
      current_data_len += pkg_data_len - 2;
      RTT_printf("[AS608] 已保存的数据长度 = %d\n", current_data_len);
      
      if (*(pos+6) == 0x02)
      {
        pos += 9 + pkg_data_len;
      }
      else if (*(pos+6) == 0x08)
      {
        break;
      }
    }
    else
      break;
  }
  // for(i = 0; i < current_data_len; i++)
  // {
  //   RTT_printf("[AS608] %d 0x%02X\n", i, data[i]);
  //   HAL_Delay(1);
  // }
  return 1;
}

// 录入图像
// 功能说明： 探测手指，探测到后录入指纹图像存于 ImageBuffer。返回确认码表示：录入成功、无手指等。
// 输入参数： none
// 返回参数： 确认字
// 指令代码： 01H
uint8_t PS_GetImage(void)
{
  uint8_t ensure;
  uint8_t *data;
  uint8_t pkg[12] = {0xef, 0x01,//包头
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,//模块地址
                     0x01,//包标识
                     0x00, 0x03,//包长度
                     0x01,//指令代码
                     0x00, 0x05};//校验和
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 生成特征
// 功能说明： 将 ImageBuffer 中的原始图像生成指纹特征文件存于 CharBuffer1(0x01) 或 CharBuffer2(0x02)
// 输入参数： BufferID(特征缓冲区号)
// 返回参数： 确认字
// 指令代码： 02H
uint8_t PS_GenChar(uint8_t bufferId)
{
  uint8_t ensure;
  uint8_t *data;
  uint16_t sum = 0x01 + 0x04 + 0x02 + bufferId;
  uint8_t pkg[13] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x04,
                     0x02,
                     bufferId,
                     sum >> 8, sum};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 精确比对两枚指纹特征
// 功能说明： 精确比对 CharBuffer1 与 CharBuffer2 中的 特征文件
// 输入参数： none
// 返回参数： 确认字，比对得分
// 指令代码： 03H
uint8_t PS_Match(void)
{
  uint8_t ensure;
  uint8_t *data;
  uint8_t pkg[12] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x03,
                     0x03,
                     0x00, 0x07};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 14);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 搜索指纹
// 功能说明： 以 CharBuffer1 或 CharBuffer2 中的特征文件搜索整个或部分指纹库。若搜索到，则返回页码。
// 输入参数： BufferID， StartPage(起始页)，PageNum（页数）
// 返回参数： 确认字，页码（相配指纹模板）
// 指令代码：04H
uint8_t PS_Search(uint8_t bufferId, uint16_t startPage, uint16_t pageNum)
{
  uint8_t ensure;
  uint8_t *data;
  uint16_t sum = 0x01 + 0x08 + 0x04 + bufferId + startPage + pageNum;
  uint8_t pkg[17] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x08,
                     0x04,
                     bufferId,
                     startPage >> 8, startPage,
                     pageNum >> 8, pageNum,
                     sum >> 8, sum};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 16);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 合并特征（生成模板）
// 功能说明： 将 CharBuffer1 与 CharBuffer2 中的特征文件合并生成模板，结果存于 CharBuffer1 与 CharBuffer2。
// 输入参数： none
// 返回参数： 确认字
// 指令代码：05H
uint8_t PS_RegModel(void)
{
  uint8_t ensure;
  uint8_t *data;
  uint8_t pkg[12] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x03,
                     0x05,
                     0x00, 0x09};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 储存模板
// 功能说明： 将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号 flash 数据库位置。
// 输入参数： BufferID(缓冲区号)，PageID（指纹库位置号）
// 返回参数： 确认字
// 指令代码： 06H
uint8_t PS_StoreChar(uint8_t bufferId, uint16_t pageId)
{
  uint8_t ensure;
  uint8_t *data;
  uint16_t sum = 0x01 + 0x06 + 0x06 + bufferId + pageId;
  uint8_t pkg[15] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x06,
                     0x06,
                     bufferId,
                     pageId >> 8, pageId,
                     sum >> 8, sum};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 读出模板
// 功能说明： 将 flash 数据库中指定 ID 号的指纹模板读入到模板缓冲区 CharBuffer1 或 CharBuffer2
// 输入参数： BufferID(缓冲区号)，PageID（指纹库位置号）
// 返回参数： 确认字
// 指令代码： 07H
uint8_t PS_LoadChar(uint8_t bufferId, uint16_t pageId)
{
  uint8_t ensure;
  uint8_t *data;
  uint16_t sum = 0x01 + 0x06 + 0x07 + bufferId + pageId;
  uint8_t pkg[15] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x06,
                     0x07,
                     bufferId,
                     pageId >> 8, pageId,
                     sum >> 8, sum};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 上传特征或模板
// 功能说明： 将特征缓冲区中的特征文件上传给上位机
// 输入参数： BufferID(缓冲区号)  
// 返回参数： 确认字
// 指令代码： 08H
uint8_t PS_UpChar(uint8_t bufferId)
{
  uint8_t ensure;
  uint8_t *data;
  uint16_t sum = 0x01 + 0x04 + 0x08 + bufferId;
  uint8_t pkg[13] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x04,
                     0x08,
                     bufferId,
                     sum >> 8, sum};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 删除模板
// 功能说明： 删除 flash 数据库中指定 ID 号开始的 N 个指纹模板
// 输入参数： PageID(指纹库模板号)，N 删除的模板个数。
// 返回参数： 确认字
// 指令代码： 0cH
uint8_t PS_DeletChar(uint16_t pageId, uint16_t num)
{
  uint8_t ensure;
  uint8_t *data;
  uint16_t sum = 0x01 + 0x07 + 0x0C + pageId + num;
  uint8_t pkg[16] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x07,
                     0x0C,
                     pageId >> 8, pageId,
                     num >> 8, num,
                     sum >> 8, sum};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 清空指纹库
// 功能说明： 删除 flash 数据库中所有指纹模板
// 输入参数： none
// 返回参数： 确认字
// 指令代码： 0dH
uint8_t PS_Empty(void)
{
  uint8_t ensure;
  uint8_t *data;
  uint8_t pkg[12] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x03,
                     0x0D,
                     0x00, 0x11};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 写系统寄存器
// 功能说明： 写模块寄存器
// 输入参数： 寄存器序号（4/5/6） 和 写入的内容
// 返回参数： 确认字
// 指令代码： 0eH
uint8_t PS_WriteReg(uint8_t regId, uint16_t param)
{
  uint8_t ensure;
  uint8_t *data;
  uint16_t sum = 0x01 + 0x05 + 0x0E + regId + param;
  uint8_t pkg[14] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x05,
                     0x0E,
                     regId,
                     param,
                     sum >> 8, sum};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 12);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 读取模块的基本参数（波特率，包大小等）。
// 功能说明： 读取模块的基本参数（波特率，包大小等。
// 输入参数： none
// 返回参数： 确认字
// 指令代码： 0fH
uint8_t PS_ReadSysPara(void)
{
  uint8_t *data;
  uint8_t pkg[12] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x03,
                     0x0f,
                     0x00, 0x13};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 28);
  if (data)
  {
    if (data[9] == 0x00)
    {
      RTT_printf("[AS608] 状态寄存器：%d\r\n", (data[10]<<8) + data[11]);
      RTT_printf("[AS608] 传感器类型：%s\r\n", SensorType((data[12]<<8) + data[13]));
      RTT_printf("[AS608] 指纹库大小：%d\r\n", (data[14]<<8) + data[15]);
      RTT_printf("[AS608] 安全等级：%d\r\n", (data[16]<<8) + data[17]);
      RTT_printf("[AS608] 设备地址：0x%02X%02X%02X%02X\r\n", data[18], data[19], data[20], data[21]);
      RTT_printf("[AS608] 数据包大小：%d bytes\r\n", ((data[22]<<8) + data[23] + 1) * 32);
      RTT_printf("[AS608] 波特率设置：%d bps\r\n", ((data[24]<<8) + data[25]) * 9600);
      return data[9]; //确认码
    }
  }
  return 0xff;
}
// 高速搜索
// 功能说明： 以 CharBuffer1 或 CharBuffer2 中的特征文件高速搜索整个或部分指纹库。若搜索到，则返回页码。
// 			 该指令对于的确存在于指纹库中，且登录时质量很好的指纹，会很快给出搜索结果。
// 输入参数： BufferID， StartPage(起始页)，PageNum（页数）
// 返回参数： 确认字，页码（相配指纹模板）
// 指令代码： 1bH
uint8_t PS_HighSpeedSearch(uint8_t bufferId, uint16_t startPage, uint16_t pageNum)
{
  uint8_t ensure;
  uint8_t *data;
  uint16_t sum = 0x01 + 0x08 + 0x1b + bufferId + startPage + pageNum;
  uint8_t pkg[17] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x08,
                     0x1b,
                     bufferId,
                     startPage >> 8, startPage,
                     pageNum >> 8, pageNum,
                     sum >> 8, sum};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 16);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}
// 读有效模板个数 PS_ValidTempleteNum
// 功能说明： 读有效模板个数
// 输入参数： none
// 返回参数： 确认字，有效模板个数 ValidN
// 指令代码： 1dH
uint8_t PS_ValidTempleteNum(void)
{
  uint8_t ensure;
  uint8_t *data;
  uint8_t pkg[12] = {0xef, 0x01,
                     AS608Addr >> 24, AS608Addr >> 16, AS608Addr >> 8, AS608Addr,
                     0x01,
                     0x00, 0x1D,
                     0x0F,
                     0x00, 0x21};
  AS608_SendPkg(pkg, sizeof(pkg));
  data = RecvAck(2000, 14);
  if (data)
    ensure = data[9]; //确认码
  else
    ensure = 0xff;
  return ensure;
}

//指纹模块串口接收完成回调函数
// void FP_UART_Rx_Callback()
// {
//   if (UART2_Rx_Cnt >= 255) //溢出判断
//   {
//     UART2_Rx_Cnt = 0;
//     UART2_State = 1;
//     RTT_printf("[AS608] 溢出\n");
//   }
//   else
//   {
//     if (UART2_State == 0) //接收未完成
//     {
//       // __HAL_TIM_SetCounter(&htim6, 0); //每次接收数据，都重新清零定时器，重新计数
//       // // RTT_printf("计时清零 第%d个数据\r\n", UART2_Rx_Cnt);
//       // if (UART2_State == 0 && UART2_Rx_Cnt == 0) //是第一次接收数据
//       // {
//       //   HAL_TIM_Base_Start_IT(&htim6); //定时器开始计时
//       // }
//       UART2_RxBuffer[UART2_Rx_Cnt++] = UART2_aRxBuffer; //接收数据转存
//     }
//   }
//   HAL_UART_Receive_IT(&huart2, (uint8_t *)&UART2_aRxBuffer, 1); //再开启接收中断
// }

const char *SensorType(uint8_t data)
{
  const char *p;
  switch (data)
  {
  case 0:
    p = "fpc1011c";
    break;
  case 2:
    p = "祥群 c500";
    break;
  case 3:
    p = "祥群 s500 条状";
    break;
  case 7:
    p = "深圳芯微条状";
    break;
  case 9:
    p = "用户自定义传感器";
    break;
  default:
    p = "未知传感器";
    break;
  }
  return p;
}

//模块应答包确认码信息解析
//功能：解析确认码错误信息返回信息
//参数: ensure
const char *EnsureMessage(uint8_t ensure)
{
  const char *p;
  switch (ensure)
  {
  case 0x00:
    p = "OK";
    break;
  case 0x01:
    p = "数据包接收错误";
    break;
  case 0x02:
    p = "传感器上没有手指";
    break;
  case 0x03:
    p = "录入指纹图像失败";
    break;
  case 0x04:
    p = "指纹图像太干、太淡而生不成特征";
    break;
  case 0x05:
    p = "指纹图像太湿、太糊而生不成特征";
    break;
  case 0x06:
    p = "指纹图像太乱而生不成特征";
    break;
  case 0x07:
    p = "指纹图像正常，但特征点太少（或面积太小）而生不成特征";
    break;
  case 0x08:
    p = "指纹不匹配";
    break;
  case 0x09:
    p = "没搜索到指纹";
    break;
  case 0x0a:
    p = "特征合并失败";
    break;
  case 0x0b:
    p = "访问指纹库时地址序号超出指纹库范围";
    break;
  case 0x10:
    p = "删除模板失败";
    break;
  case 0x11:
    p = "清空指纹库失败";
    break;
  case 0x15:
    p = "缓冲区内没有有效原始图而生不成图像";
    break;
  case 0x17:
    p = "残留指纹或两次采集之间手指没有移动过";
    break;
  case 0x18:
    p = "读写 FLASH 出错";
    break;
  case 0x19:
    p = "未定义错误";
    break;
  case 0x1a:
    p = "无效寄存器号";
    break;
  case 0x1b:
    p = "寄存器设定内容错误";
    break;
  case 0x1c:
    p = "记事本页码指定错误";
    break;
  case 0x1f:
    p = "指纹库满";
    break;
  case 0x20:
    p = "地址错误";
    break;
  default:
    p = "模块返回确认码有误";
    break;
  }
  return p;
}
