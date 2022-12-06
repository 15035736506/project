#include"APP_cfg.h"
//#include"ucos_ii.h"
#include "LCD12864.h"
#include"speed.h"
#include"NRF24L01.h"
#include"TMP275.h"

//温度模块
extern int temperture;

//电机模块
SPEED speed;//全局变量  

#define TASK_STK_SIZE  64
OS_STK LEDStk[TASK_STK_SIZE];
OS_STK  MovementStk[TASK_STK_SIZE];
OS_STK  temp_sentStk[TASK_STK_SIZE];

void LED(void *p_arg);
void Movement(void *p_arg);
void temp_sent(void *parg);
//void APP_receive(void *p_arg);

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
  WDTCTL = WDT_MDLY_8;//定时器模式，以SMCLK进行8ms的延迟
  IE1|=WDTIE;
  _EINT();
  BCSCTL2|=SELM_2+SELS+DIVS_3;//MCLK为8MHZ，SMCLK为1MHZ
  P3OUT &= ~BIT4+BIT5;
  P3DIR |= BIT4+BIT5;
}


void main()
{
  System_Init();

  OSInit();
  OSTaskCreate(LED,NULL,
               (OS_STK *)&LEDStk[TASK_STK_SIZE-1],
               (INT8U)3);
  OSTaskCreate(Movement,NULL,
               (OS_STK *)&MovementStk[TASK_STK_SIZE-1],
               (INT8U)5);
  OSTaskCreate(temp_sent,NULL,
               (OS_STK *)&temp_sentStk[TASK_STK_SIZE-1],
               (INT8U)6);
  OSStart();
}

void LED(void *p_arg)
{
  p_arg = p_arg;
 
  while(1)
  {

    P3OUT ^= BIT4;
    OSTimeDlyHMSM(0,0,1,0);    
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
  OSTimeDlyHMSM(0,0,3,0);
  
  speed.ZKB = 850;
  speed.direction = 1;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,5,0);
  
  speed.ZKB = 600;
  speed.direction = 0;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,3,0);  
  
  speed.ZKB = 600;
  speed.direction = 2;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,5,0);  
  
  speed.ZKB = 600;
  speed.direction = 0;
  speed_conf(&speed);
  OSTimeDlyHMSM(0,0,3,0);   
  }
}

void temp_sent(void *p_arg)
{
  unsigned char TxData[4];
  p_arg = p_arg;
  
  NRF24L01_Init(); 
  writeTMP275_reg(0x01,0x0060);
  OSTimeDlyHMSM(0,0,1,0);
  while(1)
  {
    readTMP275_temp(0x00);
    OSTimeDlyHMSM(0,0,1,0);
    TxData[0] = temperture;
    TxData[1] = temperture>>8;
    TxData[2] = 0;
    TxData[3] = 0;
    NRFSetTxMode(TxData);//发送温度
    while(CheckACK());	//检测是否发送完毕
    P3OUT &= ~BIT4;
    OSTimeDlyHMSM(0,0,0,200);
    P3OUT |= BIT4;
  }
}