/******************************
说明：此文件用于实现小车速度监测及调整功能
作者：arvik
日期：2013.8.8
*******************************/

#include"includes.h"
#include"motor.h"
#include"speed.h"
//#include    "ucos_ii.h"
#include"moniNRF24L01.h"
#include"USART.h"

/*采用msp430f149的TimerA的捕获模式测速*/



void speed_conf(SPEED* _speed)
{
  //参数检查
  if((_speed->ZKB)<600)
    _speed->ZKB = 600;
  if(_speed->ZKB>900)
    _speed->ZKB = 900;
  
  //先配置默认速度,即默认占空比
  switch(_speed->direction)
  {
    case stop:
      Stop_conf();
      break;
    case forward:
      GoForward_conf(_speed->ZKB);
      break;
    case back:
      DrawBack_conf(_speed->ZKB);
      break;
    case left:
      TurnLeft_conf(_speed->ZKB);
      break;
    case right:
      TurnRight_conf(_speed->ZKB);
      break;
    default :
      //填写错误执行代码
      //...........
      break;
  }
}


void MovementSpeed(void *p_arg)
{  
//  unsigned char RxBuf[3]={0,0,3};
  extern char ReceiveBuf0[];
  extern char RevSussFlag;//消息接收状态指示
  speedZKB conf_speedZKB;
  p_arg = p_arg;
  
  conf_speedZKB.direction = stop;//stop;//小车默认运动控制
  conf_speedZKB.ZKB_left = 120;
  conf_speedZKB.ZKB_right = 110;
  speedconf(conf_speedZKB);
  
  InitUSART();
//  USARTSendStr("abcdefg1234567890");
  while(1)//小车运动控制，必须是个死循环
  {
    
    if(RevSussFlag==1)
    {
      P3OUT ^= BIT4;
      RevSussFlag = 0;
//      USARTSendStr(ReceiveBuf0);
//      USARTSendStr("收到");
      if((ReceiveBuf0[2]-0x30)!=conf_speedZKB.direction)
      {
        conf_speedZKB.direction = ReceiveBuf0[2]-0x30;//获取遥控的数据量
  //      conf_speedZKB.ZKB_left  = RxBuf[0]; //左右轮都是ReceiveBufF[2]控制
  //      conf_speedZKB.ZKB_right = RxBuf[1];
        //进入运动状态更改 缓冲时间
        Stop_conf();
        OSTimeDlyHMSM(0,0,0,200);//关闭定时器一小段时间做缓冲，以免电机产生强大的反向电流
      
        speedconf(conf_speedZKB);//更改小车运动状态
//        P3OUT^=BIT5;
  //      OSTimeDlyHMSM(0,0,0,40);
      }
    }
    OSTimeDlyHMSM(0,0,0,40);
    
  }
}


/*
//红外传感器控制
void MovementSpeed1(void *p_arg)
{  
//  unsigned char RxBuf[3]={0,0,3};
  unsigned char direction;
  unsigned char NumSta = 0,sta=0;
  speedZKB conf_speedZKB;
  p_arg = p_arg;
  
  P6DIR |= 0xfc;
  
  conf_speedZKB.direction = forward;//stop;//小车默认运动控制
  conf_speedZKB.ZKB_left = 80;
  conf_speedZKB.ZKB_right = 75;
  speedconf(conf_speedZKB);
  
  while(1)//小车运动控制，必须是个死循环
  {
    sta = P6IN&0x03;
    switch(sta)
    {
    case 3://左右都无，前进
      direction = forward;
      if(conf_speedZKB.direction != direction)
      {
        NumSta++;
        if(NumSta>100)
        {
          conf_speedZKB.direction = stop;
          speedconf(conf_speedZKB);          
          while(1);
        }
        conf_speedZKB.direction = direction;
      }      
      break;
     case 2://左有右无，向右转
      direction = right;
      if(conf_speedZKB.direction != direction)
      {
        NumSta++;
        if(NumSta>100)
        {
          conf_speedZKB.direction = stop;
          speedconf(conf_speedZKB);          
          while(1);
        }
        conf_speedZKB.direction = direction;
      }      
      break;
     case 1://左无右有，向左转
      direction = left;
      if(conf_speedZKB.direction != direction)
      {
        NumSta++;
        if(NumSta>100)
        {
          conf_speedZKB.direction = stop;
          speedconf(conf_speedZKB);          
          while(1);
        }
        conf_speedZKB.direction = direction;
      }      
      break;     
     case 0://左右都有，先后退，再向左转
      direction = stop;
      if(conf_speedZKB.direction != direction)
      {
        NumSta++;
        if(NumSta>100)
        {
          conf_speedZKB.direction = stop;
          speedconf(conf_speedZKB);          
          while(1);
        }
        conf_speedZKB.direction = direction;
      }      
      break;
    }
    if(direction != stop )
    {
      speedconf(conf_speedZKB);
      OSTimeDlyHMSM(0,0,0,800);      
    }
    else
    {
      speedconf(conf_speedZKB);
      OSTimeDlyHMSM(0,0,0,300);
      conf_speedZKB.direction = back;
      speedconf(conf_speedZKB);
      OSTimeDlyHMSM(0,0,2,0);
      conf_speedZKB.direction = left;
      speedconf(conf_speedZKB);
      OSTimeDlyHMSM(0,0,1,0);
      conf_speedZKB.direction = stop;
//      speedconf(conf_speedZKB);
//      OSTimeDlyHMSM(0,0,0,200);
    }
    OSTimeDlyHMSM(0,0,0,20);
  }
}
*/

/*
//超声波传感器控制
extern unsigned int SR04check();
extern void timerA_conf();
extern void SR04_port_init();
void MovementSpeed2(void *p_arg)
{  
//  unsigned char RxBuf[3]={0,0,3};

  speedZKB conf_speedZKB;
  
  unsigned int distance;
  
  p_arg = p_arg;
    
  conf_speedZKB.direction = stop;//stop;//小车默认运动控制
  conf_speedZKB.ZKB_left = 100;//80;
  conf_speedZKB.ZKB_right = 95;//75;
//  speedconf(conf_speedZKB);
  
  SR04_port_init();
  timerA_conf();
  OSTimeDlyHMSM(0,0,3,0);
  while(1)//小车运动控制，必须是个死循环
  {
    while((distance=SR04check())==0);
    if(distance<30&&distance>6)
    {
      conf_speedZKB.direction = stop;
      speedconf(conf_speedZKB);
      OSTimeDlyHMSM(0,0,0,300);
      conf_speedZKB.direction = back;
      speedconf(conf_speedZKB);
      OSTimeDlyHMSM(0,0,2,0);
      conf_speedZKB.direction = left;
      speedconf(conf_speedZKB);
      OSTimeDlyHMSM(0,0,1,500);
      conf_speedZKB.direction = forward;
      speedconf(conf_speedZKB);
      OSTimeDlyHMSM(0,0,0,200);
    }
    OSTimeDlyHMSM(0,0,0,100);
  }
}
*/