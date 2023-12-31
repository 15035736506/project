/*
 * hex2str.c
 *
 *  Created on: 2022年5月20日
 *      Author: Naming8104
 */

#include "main.h"

/**
 * @brief 将字节数组转为十六进制字符串
 */
void hex2str(uint8_t *input, uint16_t input_len, char *output)
{
  char *zEncode = "0123456789ABCDEF";
  int i = 0, j = 0;

  for (i = 0; i < input_len; i++)
  {
    output[j++] = zEncode[(input[i] >> 4) & 0xf];
    output[j++] = zEncode[(input[i]) & 0xf];
  }
  output[j] = '\0';
}
