#include"includes.h"
#include"LCD12864.h"
#include"speed.h"

//电机模块
SPEED speed;//全局变量  

//配置任务栈空间大小
#define  TASK_STK_SIZE     80
 
//配置任务优先级
#define  TaskStartpiro  5
#define  Movementpiro   8
#define  LEDpiro        6


//分配任务栈空间
OS_STK  TaskStartStk[TASK_STK_SIZE];
//OS_STK  MovementStk[TASK_STK_SIZE];
OS_STK  LEDStk[TASK_STK_SIZE];


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
  BCSCTL2|=SELM_2+SELS+DIVS_3;//MCLK为8MHZ，SMCLK为1MHZ  
  WDTCTL = WDT_MDLY_8;//定时器模式，以SMCLK进行8ms的延迟
  IE1|=WDTIE;
  _EINT();

  P3OUT &= ~BIT5;
  P3OUT |= BIT4;
  P3DIR |= BIT4 + BIT5;
}

void TaskStart(void *p_arg);
void Movement(void *p_arg);
void LED(void *p_arg);

void main()
{
  System_Init();
  
  OSInit();
  
/*  OSTaskCreate(TaskStart,NULL,
               (OS_STK *)TaskStartStk[TASK_STK_SIZE-1],
               (INT8U)TaskStartpiro);  */
  OSTaskCreate(LED,NULL,
               (OS_STK *)LEDStk[TASK_STK_SIZE-1],
               (INT8U)LEDpiro);  
  
  OSStart();
}

void TaskStart(void *p_arg)
{
   p_arg = p_arg;

//基本外设初始化驱动程序程序都放置于此   
//   LCD12864_init();
   
//任务创建
/*  OSTaskCreate(Movement,NULL,
               (OS_STK *)MovementStk[TASK_STK_SIZE-1],
               (INT8U)Movementpiro); */


//  OSTimeDlyHMSM(0,0,0,16);  
//删除自己,
//  OSTaskDel(OS_PRIO_SELF);
  while(1)
  {
//    OSTimeDlyHMSM(0,0,5,0);
        P3OUT^= BIT5;
    OSTimeDlyHMSM(0,0,2,0);
  }
}

void Movement(void *p_arg)
{
  p_arg = p_arg;

  while(1)
  {
  speed.ZKB = 600;
  speed.direction = 1;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,10,0);
  
  speed.ZKB = 850;
  speed.direction = 1;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,10,0);
  
  speed.ZKB = 600;
  speed.direction = 0;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,3,0);  
  
  speed.ZKB = 600;
  speed.direction = 2;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,10,0);  
  
  speed.ZKB = 600;
  speed.direction = 0;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,2,0);   
  }
}

void LED(void *p_arg)
{
  p_arg = p_arg;
 
  while(1)
  {
    P3OUT^= BIT4;
    OSTimeDlyHMSM(0,0,2,500);
  }
}