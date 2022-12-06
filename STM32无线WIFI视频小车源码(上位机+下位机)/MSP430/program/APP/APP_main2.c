#include"APP_cfg.h"
//#include"ucos_ii.h"
//#include "LCD12864.h"
#include"speed.h"
#include"moniNRF24L01.h"
//#include"TMP275.h"

//温度模块
//extern int temperture;




#define TASK_STK_SIZE  48
#define MovementSpeed_STK_SIZE  60

OS_STK LEDStk[TASK_STK_SIZE];
OS_STK LED2Stk[TASK_STK_SIZE];
OS_STK  MovementSpeedStk[MovementSpeed_STK_SIZE];
//OS_STK  MovementSpeed1Stk[MovementSpeed_STK_SIZE];
//OS_STK  MovementSpeed2Stk[MovementSpeed_STK_SIZE];

void LED(void *p_arg);
void LED2(void *p_arg);


//OS_EVENT *pmailbox;//用于指向创建的消息邮箱


void System_Init()
{
  unsigned int i;
  WDTCTL=WDTPW+WDTHOLD;
  BCSCTL1&=~XT2OFF;
  do
  {
    IFG1&=~OFIFG;
    for(i=0xff;i>0;i--);
  }
  while(IFG1&OFIFG);
  WDTCTL = WDT_MDLY_32;//定时器模式，以SMCLK进行4ms的延迟
  IE1|=WDTIE;
  _EINT();
  BCSCTL2|=SELM_2+SELS;//MCLK为8MHZ，SMCLK为8MHZ
  P3OUT &= ~BIT4+BIT5;
  P3DIR |= BIT4+BIT5;
}


void main()
{
  System_Init();

  OSInit();
//  OSTaskCreate(LED,NULL,
//               (OS_STK *)&LEDStk[TASK_STK_SIZE-1],
//               (INT8U)13);
  OSTaskCreate(MovementSpeed,NULL,
               (OS_STK *)&MovementSpeedStk[MovementSpeed_STK_SIZE-1],
               (INT8U)10);
  
//  OSTaskCreate(MovementSpeed1,NULL,
//               (OS_STK *)&MovementSpeed1Stk[MovementSpeed_STK_SIZE-1],
//               (INT8U)11);
  
  OSTaskCreate(LED2,NULL,
               (OS_STK *)&LED2Stk[TASK_STK_SIZE-1],
               (INT8U)12);
//  OSTaskCreate(MovementSpeed2,NULL,
//               (OS_STK *)&MovementSpeed2Stk[MovementSpeed_STK_SIZE-1],
//               (INT8U)11);

  OSStart();
}


void LED(void *p_arg)
{
  p_arg = p_arg;
 
  while(1)
  {

    P3OUT ^= BIT4;
    OSTimeDlyHMSM(0,0,0,500);    
  }
}

void LED2(void *p_arg)
{
  p_arg = p_arg;
 
  P3OUT ^= BIT4;
  while(1)
  {
    P3OUT |= BIT5; //灭
    OSTimeDlyHMSM(0,0,2,900);
    P3OUT &= ~BIT5; //亮
    OSTimeDlyHMSM(0,0,0,100);    
  }
}
