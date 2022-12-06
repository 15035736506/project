#include<msp430f149.h>
#include "motor.h"





unsigned int Fre=200;//在ACLK=32768下，周期约为6.1ms
/*说明：若指定输入电压为8V，则占空比应为60%~100%*/
unsigned int ZKB1=150,ZKB2=150;//占空比，决定输出给电动机的电压 
/*
驱动芯片供电为稳定的8V，电机工作电压为5V~8V，所以PWM波占空比应在62.5%~100%之间
即 125<= ZKB <= 198
*/


speedZKB speed_ZKB;

unsigned char speedconf(speedZKB now_ZKB)
{
    TBCTL &= ~(MC0+MC1); //关闭定时器 
    switch(now_ZKB.direction)
    {
      case stop:
          P4DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;
          P4OUT &=~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
          P4SEL &=~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
          break;
      case forward:
          P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
          P4SEL |= BIT1 + BIT3;//第二功能
          P4SEL &= ~(BIT2 + BIT4);//引脚功能
          P4OUT &= ~(BIT2 + BIT4);//低电平
        break;
      case goback:
          P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
          P4SEL |= BIT2 + BIT4;
          P4SEL &= ~(BIT1 + BIT3);
          P4OUT &= ~(BIT1 + BIT3);
        break;
      case turnleft:   //左轮不转，右轮前转
          P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
          P4SEL |= BIT3;
          P4SEL &= ~(BIT1 + BIT2 + BIT4);
          P4OUT &= ~(BIT1 + BIT2 + BIT4);
          break;
      case turnright:  //右轮不转，左轮前转
          P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
          P4SEL |= BIT1;
          P4SEL &= ~(BIT2 + BIT3 + BIT4);
          P4OUT &= ~(BIT2 + BIT3 + BIT4);
          break;
      default :
          break;
    }
    
    TBCTL = TBSSEL_1 + TBCLR ; //时钟源为ACLK=32768HZ 
    TBCCTL1 = OUTMOD_7; //工作在复位/置位 模式 
    TBCCTL2 = OUTMOD_7;
    TBCCTL3 = OUTMOD_7;
    TBCCTL4 = OUTMOD_7;
    
    TBCCR1 = now_ZKB.ZKB_left;    //TBCCR1,TBCCR2控制左轮
    TBCCR2 = now_ZKB.ZKB_left;
    TBCCR3 = now_ZKB.ZKB_right;    //TBCCR3,TBCCR4控制右轮
    TBCCR4 = now_ZKB.ZKB_right;
    TBCCR0 = Fre;
    
    TBCTL |= MC_1; //打开定时器，计数模式2 (连续计数模式)
 
  if( (TBCTL & MC_0) == 0 ) //判断PWM是否运行 
    return 0; //处于停止模式
  else 
    return 1; //运行     
}

//用timerB产生的PWM控制小车运动

/*
函数名：timerB_start()
描述  ：配置timerB，产生PWM波
输入  ：无
输出  ：运行状态
*/
unsigned char timerB_start()
{
  TBCTL = TBSSEL_1 + TBCLR ; //时钟源为ACLK=32768HZ 
  TBCCTL1 = OUTMOD_7; //工作在复位/置位 模式 
  TBCCTL2 = OUTMOD_7;
  TBCCTL3 = OUTMOD_7;
  TBCCTL4 = OUTMOD_7;

  TBCTL &= ~(MC0+MC1); //关闭定时器 

    TBCCR1 = ZKB1;    //TBCCR1,TBCCR2控制左轮
    TBCCR2 = ZKB1;
    TBCCR3 = ZKB2;    //TBCCR3,TBCCR4控制右轮
    TBCCR4 = ZKB2;
    TBCCR0 = Fre;
    
    TBCTL |= MC_1; //打开定时器，计数模式2 (连续计数模式)
 
  if( (TBCTL & MC_0) == 0 ) //判断PWM是否运行 
    return 0; //处于停止模式
  else 
    return 1; //运行    
}

void Stop_conf()
{
  TBCTL &= ~(MC0+MC1); //关闭定时器 
}

//前进，600<ZKB<1000
void GoForward_conf(unsigned int ZKB)
{
  TBCTL &= ~(MC0+MC1); //关闭定时器  
  P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
  P4SEL |= BIT1 + BIT3;
  P4SEL &= ~(BIT2 + BIT4);
  P4OUT &= ~(BIT2 + BIT4);
  ZKB1 = ZKB;
  ZKB2 = ZKB;
  timerB_start();
}

//后退
void DrawBack_conf(unsigned int ZKB)
{
  TBCTL &= ~(MC0+MC1); //关闭定时器  
  P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
  P4SEL |= BIT2 + BIT4;
  P4SEL &= ~(BIT1 + BIT3);
  P4OUT &= ~(BIT1 + BIT3);
  ZKB1 = ZKB;
  ZKB2 = ZKB;
  timerB_start();
}

//左转
void TurnLeft_conf(unsigned int ZKB)
{
  TBCTL &= ~(MC0+MC1); //关闭定时器  
  P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
  P4SEL |= BIT2 + BIT3;
  P4SEL &= ~(BIT1 + BIT4);
  P4OUT &= ~(BIT1 + BIT4);
  ZKB1 = ZKB;
  ZKB2 = ZKB;
  timerB_start();
}

//右转
void TurnRight_conf(unsigned int ZKB)
{
  TBCTL &= ~(MC0+MC1); //关闭定时器  
  P4DIR |= BIT1 + BIT2 + BIT3 + BIT4;
  P4SEL |= BIT1 + BIT4;
  P4SEL &= ~(BIT2 + BIT3);
  P4OUT &= ~(BIT2 + BIT3);
  ZKB1 = ZKB;
  ZKB2 = ZKB;
  timerB_start();
}