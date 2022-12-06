/******************************
˵�������ļ�����ʵ��С���ٶȼ�⼰��������
���ߣ�arvik
���ڣ�2013.8.8
*******************************/

#include"includes.h"
#include"motor.h"
#include"speed.h"
//#include    "ucos_ii.h"
#include"moniNRF24L01.h"
#include"USART.h"

/*����msp430f149��TimerA�Ĳ���ģʽ����*/



void speed_conf(SPEED* _speed)
{
  //�������
  if((_speed->ZKB)<600)
    _speed->ZKB = 600;
  if(_speed->ZKB>900)
    _speed->ZKB = 900;
  
  //������Ĭ���ٶ�,��Ĭ��ռ�ձ�
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
      //��д����ִ�д���
      //...........
      break;
  }
}


void MovementSpeed(void *p_arg)
{  
//  unsigned char RxBuf[3]={0,0,3};
  extern char ReceiveBuf0[];
  extern char RevSussFlag;//��Ϣ����״ָ̬ʾ
  speedZKB conf_speedZKB;
  p_arg = p_arg;
  
  conf_speedZKB.direction = stop;//stop;//С��Ĭ���˶�����
  conf_speedZKB.ZKB_left = 120;
  conf_speedZKB.ZKB_right = 110;
  speedconf(conf_speedZKB);
  
  InitUSART();
//  USARTSendStr("abcdefg1234567890");
  while(1)//С���˶����ƣ������Ǹ���ѭ��
  {
    
    if(RevSussFlag==1)
    {
      P3OUT ^= BIT4;
      RevSussFlag = 0;
//      USARTSendStr(ReceiveBuf0);
//      USARTSendStr("�յ�");
      if((ReceiveBuf0[2]-0x30)!=conf_speedZKB.direction)
      {
        conf_speedZKB.direction = ReceiveBuf0[2]-0x30;//��ȡң�ص�������
  //      conf_speedZKB.ZKB_left  = RxBuf[0]; //�����ֶ���ReceiveBufF[2]����
  //      conf_speedZKB.ZKB_right = RxBuf[1];
        //�����˶�״̬���� ����ʱ��
        Stop_conf();
        OSTimeDlyHMSM(0,0,0,200);//�رն�ʱ��һС��ʱ�������壬����������ǿ��ķ������
      
        speedconf(conf_speedZKB);//����С���˶�״̬
//        P3OUT^=BIT5;
  //      OSTimeDlyHMSM(0,0,0,40);
      }
    }
    OSTimeDlyHMSM(0,0,0,40);
    
  }
}


/*
//���⴫��������
void MovementSpeed1(void *p_arg)
{  
//  unsigned char RxBuf[3]={0,0,3};
  unsigned char direction;
  unsigned char NumSta = 0,sta=0;
  speedZKB conf_speedZKB;
  p_arg = p_arg;
  
  P6DIR |= 0xfc;
  
  conf_speedZKB.direction = forward;//stop;//С��Ĭ���˶�����
  conf_speedZKB.ZKB_left = 80;
  conf_speedZKB.ZKB_right = 75;
  speedconf(conf_speedZKB);
  
  while(1)//С���˶����ƣ������Ǹ���ѭ��
  {
    sta = P6IN&0x03;
    switch(sta)
    {
    case 3://���Ҷ��ޣ�ǰ��
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
     case 2://�������ޣ�����ת
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
     case 1://�������У�����ת
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
     case 0://���Ҷ��У��Ⱥ��ˣ�������ת
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
//����������������
extern unsigned int SR04check();
extern void timerA_conf();
extern void SR04_port_init();
void MovementSpeed2(void *p_arg)
{  
//  unsigned char RxBuf[3]={0,0,3};

  speedZKB conf_speedZKB;
  
  unsigned int distance;
  
  p_arg = p_arg;
    
  conf_speedZKB.direction = stop;//stop;//С��Ĭ���˶�����
  conf_speedZKB.ZKB_left = 100;//80;
  conf_speedZKB.ZKB_right = 95;//75;
//  speedconf(conf_speedZKB);
  
  SR04_port_init();
  timerA_conf();
  OSTimeDlyHMSM(0,0,3,0);
  while(1)//С���˶����ƣ������Ǹ���ѭ��
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