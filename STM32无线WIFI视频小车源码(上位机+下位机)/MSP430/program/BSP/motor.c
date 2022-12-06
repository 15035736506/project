#include<msp430f149.h>
#include "motor.h"





unsigned int Fre=200;//��ACLK=32768�£�����ԼΪ6.1ms
/*˵������ָ�������ѹΪ8V����ռ�ձ�ӦΪ60%~100%*/
unsigned int ZKB1=150,ZKB2=150;//ռ�ձȣ�����������綯���ĵ�ѹ 
/*
����оƬ����Ϊ�ȶ���8V�����������ѹΪ5V~8V������PWM��ռ�ձ�Ӧ��62.5%~100%֮��
�� 125<= ZKB <= 198
*/


speedZKB speed_ZKB;

unsigned char speedconf(speedZKB now_ZKB)
{
    TBCTL &= ~(MC0+MC1); //�رն�ʱ�� 
    switch(now_ZKB.direction)
    {
      case stop:
          P4DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;
          P4OUT &=~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
          P4SEL &=~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
          break;
      case forward:
          P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
          P4SEL |= BIT1 + BIT3;//�ڶ�����
          P4SEL &= ~(BIT2 + BIT4);//���Ź���
          P4OUT &= ~(BIT2 + BIT4);//�͵�ƽ
        break;
      case goback:
          P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
          P4SEL |= BIT2 + BIT4;
          P4SEL &= ~(BIT1 + BIT3);
          P4OUT &= ~(BIT1 + BIT3);
        break;
      case turnleft:   //���ֲ�ת������ǰת
          P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
          P4SEL |= BIT3;
          P4SEL &= ~(BIT1 + BIT2 + BIT4);
          P4OUT &= ~(BIT1 + BIT2 + BIT4);
          break;
      case turnright:  //���ֲ�ת������ǰת
          P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
          P4SEL |= BIT1;
          P4SEL &= ~(BIT2 + BIT3 + BIT4);
          P4OUT &= ~(BIT2 + BIT3 + BIT4);
          break;
      default :
          break;
    }
    
    TBCTL = TBSSEL_1 + TBCLR ; //ʱ��ԴΪACLK=32768HZ 
    TBCCTL1 = OUTMOD_7; //�����ڸ�λ/��λ ģʽ 
    TBCCTL2 = OUTMOD_7;
    TBCCTL3 = OUTMOD_7;
    TBCCTL4 = OUTMOD_7;
    
    TBCCR1 = now_ZKB.ZKB_left;    //TBCCR1,TBCCR2��������
    TBCCR2 = now_ZKB.ZKB_left;
    TBCCR3 = now_ZKB.ZKB_right;    //TBCCR3,TBCCR4��������
    TBCCR4 = now_ZKB.ZKB_right;
    TBCCR0 = Fre;
    
    TBCTL |= MC_1; //�򿪶�ʱ��������ģʽ2 (��������ģʽ)
 
  if( (TBCTL & MC_0) == 0 ) //�ж�PWM�Ƿ����� 
    return 0; //����ֹͣģʽ
  else 
    return 1; //����     
}

//��timerB������PWM����С���˶�

/*
��������timerB_start()
����  ������timerB������PWM��
����  ����
���  ������״̬
*/
unsigned char timerB_start()
{
  TBCTL = TBSSEL_1 + TBCLR ; //ʱ��ԴΪACLK=32768HZ 
  TBCCTL1 = OUTMOD_7; //�����ڸ�λ/��λ ģʽ 
  TBCCTL2 = OUTMOD_7;
  TBCCTL3 = OUTMOD_7;
  TBCCTL4 = OUTMOD_7;

  TBCTL &= ~(MC0+MC1); //�رն�ʱ�� 

    TBCCR1 = ZKB1;    //TBCCR1,TBCCR2��������
    TBCCR2 = ZKB1;
    TBCCR3 = ZKB2;    //TBCCR3,TBCCR4��������
    TBCCR4 = ZKB2;
    TBCCR0 = Fre;
    
    TBCTL |= MC_1; //�򿪶�ʱ��������ģʽ2 (��������ģʽ)
 
  if( (TBCTL & MC_0) == 0 ) //�ж�PWM�Ƿ����� 
    return 0; //����ֹͣģʽ
  else 
    return 1; //����    
}

void Stop_conf()
{
  TBCTL &= ~(MC0+MC1); //�رն�ʱ�� 
}

//ǰ����600<ZKB<1000
void GoForward_conf(unsigned int ZKB)
{
  TBCTL &= ~(MC0+MC1); //�رն�ʱ��  
  P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
  P4SEL |= BIT1 + BIT3;
  P4SEL &= ~(BIT2 + BIT4);
  P4OUT &= ~(BIT2 + BIT4);
  ZKB1 = ZKB;
  ZKB2 = ZKB;
  timerB_start();
}

//����
void DrawBack_conf(unsigned int ZKB)
{
  TBCTL &= ~(MC0+MC1); //�رն�ʱ��  
  P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
  P4SEL |= BIT2 + BIT4;
  P4SEL &= ~(BIT1 + BIT3);
  P4OUT &= ~(BIT1 + BIT3);
  ZKB1 = ZKB;
  ZKB2 = ZKB;
  timerB_start();
}

//��ת
void TurnLeft_conf(unsigned int ZKB)
{
  TBCTL &= ~(MC0+MC1); //�رն�ʱ��  
  P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
  P4SEL |= BIT2 + BIT3;
  P4SEL &= ~(BIT1 + BIT4);
  P4OUT &= ~(BIT1 + BIT4);
  ZKB1 = ZKB;
  ZKB2 = ZKB;
  timerB_start();
}

//��ת
void TurnRight_conf(unsigned int ZKB)
{
  TBCTL &= ~(MC0+MC1); //�رն�ʱ��  
  P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
  P4SEL |= BIT1 + BIT4;
  P4SEL &= ~(BIT2 + BIT3);
  P4OUT &= ~(BIT2 + BIT3);
  ZKB1 = ZKB;
  ZKB2 = ZKB;
  timerB_start();
}