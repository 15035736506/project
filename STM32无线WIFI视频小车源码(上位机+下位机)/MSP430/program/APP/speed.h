#ifndef _speech_h
#define _speech_h

#include "motor.h"

typedef struct
{
  INT16U ZKB;//占空比调节
  INT16U direction;//运动方向
}SPEED;


//SPEED的成员direction的参数
#define stop    0
#define forward 1
#define back    2
#define left    3
#define right   4





void speed_conf(SPEED* _speed);
void MovementSpeed(void *p_arg);
void MovementSpeed1(void *p_arg);
void MovementSpeed2(void *p_arg);

#endif