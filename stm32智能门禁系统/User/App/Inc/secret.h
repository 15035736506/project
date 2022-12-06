/**
 * @file secret.h
 * @author Naming8104
 * @brief 
 * @version 0.1
 * @date 2022-04-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef INC_SECRET_H_
#define INC_SECRET_H_

#include "main.h"

#define PW_MAX_LEN 16
#define PW_MIN_LEN 8
#define SHA256_HASH_LEN 32

typedef enum
{
  Unkown_KT = 0, FingerPrint_KT, Password_KT
}KeyType; //钥匙类型

void Secret_Init();
void SavePWHash(uint8_t *password, uint8_t password_len);
void SaveFPHash(uint8_t pageId);
void ReadKeyHash(KeyType key_type, uint8_t *saved_fp_hash);
void ReadKeyID(KeyType lock_type, uint8_t *key_id);
uint8_t ValidatePW(uint8_t *pw_buff, uint8_t pw_len);
uint8_t ValidateFP();
uint8_t RegisterNewFP();

#endif /* INC_SECRET_H_ */
