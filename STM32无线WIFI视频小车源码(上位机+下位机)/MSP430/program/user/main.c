#include"APP_cfg.h"
//#include"ucos_ii.h"
#include "LCD12864.h"


#define TASK_STK_SIZE  64
OS_STK AppStk_send[TASK_STK_SIZE];
OS_STK AppStk_receive[TASK_STK_SIZE];

void APP_send(void *p_arg);
void APP_receive(void *p_arg);

OS_EVENT *pmailbox;//用于指向创建的消息邮箱


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
//  LCD_Init();
//  LCD1602_write_strAt(0,0,"hao");

  
  OSInit();
  OSTaskCreate(APP_send,NULL,
               (OS_STK *)&AppStk_send[TASK_STK_SIZE-1],
               (INT8U)3);
  OSTaskCreate(APP_receive,NULL,
               (OS_STK *)&AppStk_receive[TASK_STK_SIZE-1],
               (INT8U)5);
  OSStart();
}

void APP_send(void *p_arg)
{
  LCD12864_init();
  LCD12864_write_strAt(0,0,"hao");

  INT8U i = 0;
  INT8U err = 0;
  INT8U *buffer[3];
  buffer[0] = "test01";
  buffer[1] = "test02";
  buffer[2] = "test03";
  
  p_arg = p_arg;
  
  pmailbox = OSMboxCreate(NULL);//创建消息邮箱
  for(i=0;i<=2;)
  {
    if(err = OSMboxPost(pmailbox,buffer[i])==OS_NO_ERR)
    {           //发送消息，如果成功，发送下一个
      //LCD1602_write_strAt(0,1,"sender");
      LCD12864_write_strAt(0,0,"sender");
      LCD12864_write_strAt(4,0,buffer[i]);
      i++;
    }
    else if(err==OS_MBOX_FULL)
    {
      //LCD1602_write_strAt(0,1,"sender");
      //LCD1602_write_str(pmailbox->OSEventPtr);
      LCD12864_write_strAt(0,0,"sende1");
      LCD12864_write_str(pmailbox->OSEventPtr);
    }
    else
      //LCD1602_write_strAt(0,1,"error ");
      LCD12864_write_strAt(0,0,"error ");
    P3OUT ^= BIT4;
    OSTimeDlyHMSM(0,0,2,0);
  }
  OSTimeDlyHMSM(0,0,5,0);//休眠5秒后删除此消息邮箱
  OSMboxDel(pmailbox,OS_DEL_NO_PEND,&err);
  if(err==OS_ERR_TASK_WAITING)
    //LCD1602_write_strAt(0,1,"waiting");
    LCD12864_write_strAt(0,1,"error ");
  if(err=OSTaskDel(5)!=0)
  {
    LCD12864_write_strAt(0,2,"SendTask is Del");
  }
  while(1)
  {
    OSTimeDlyHMSM(0,0,1,0);
    P3OUT ^= BIT4;  
  }
}

void APP_receive(void *p_arg)
{
  INT8U err;
  INT16U timeout = 100;
  unsigned char *buffer;
  p_arg = p_arg;
  
  for(;;)
  {
    buffer = OSMboxPend(pmailbox,timeout,&err);//从消息邮箱中读取消息
    if(err==OS_NO_ERR)//若果获取成功，打印读取到的消息
    {
      //LCD1602_write_strAt(0,0,buffer);
      LCD12864_write_strAt(0,1,buffer);
    }
    else if(err==OS_TIMEOUT)//如果在指定的时间超时任然没有消息，则打印错误提示
    {
      //LCD1602_write_strAt(0,0,"timeout");
      LCD12864_write_strAt(0,1,"timeout");
    }
    else if(err == OS_ERR_EVENT_TYPE)//如果指定的消息类型不匹配，则打印错误提示
    {
      //LCD1602_write_strAt(0,1,"errtype");
      LCD12864_write_strAt(0,1,"timeout");
      break;
    }
    else
      break;
    
    OSTimeDlyHMSM(0,0,6,0);
    P3OUT ^= BIT5;
  }

}