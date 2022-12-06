#include "includes.h"

#include "err.h"  
#include "lwip/init.h"
#include "etharp.h"
#include "udp.h"
#include "tcp.h"
#include "httpd.h"

#include "stm32f10x.h"
#include "OV7725.h"
#include "Imag_App.h"	
#include "usart1.h"
#include "SCCB.h"

#include "Imag_App.h"

OS_STK task_led2_stk[TASK_LED2_STK_SIZE];
OS_STK task_led3_stk[TASK_LED3_STK_SIZE];

extern volatile u8 Frame_Count;

extern int UDP_task();
extern struct netif enc28j60;


void Task_LED(void *p_arg)
{
    (void)p_arg;                		// 'p_arg' 并没有用到，防止编译器提示警告

	OSTaskCreate(Task_LED3,(void *)0,	//先建立此任务，则此任务可以立即执行
	   &task_led3_stk[TASK_LED3_STK_SIZE-1], TASK_LED3_PRIO);
   	OSTaskCreate(Task_LED2,(void *)0,
	   &task_led2_stk[TASK_LED2_STK_SIZE-1], TASK_LED2_PRIO);

    while (1)						//TCP定时器
    {
        LED1( ON );
        OSTimeDlyHMSM(0, 0,0,250);
        LED1( OFF);
		tcp_tmr();
		OSTimeDlyHMSM(0, 0,0,250);
		tcp_tmr();	   //每250ms调用一次        
    }
}

void Task_LED2(void *p_arg)			//arp任务
{
	(void)p_arg;
	while(1)
	{
		LED2(ON);
		OSTimeDlyHMSM(0,0,5,0);
		etharp_tmr();  //每5s调用一次
		LED2(OFF);
		OSTimeDlyHMSM(0,0,5,0);
		etharp_tmr();  //每5s调用一次
	}
}

extern struct udp_pcb *UdpPcb;
extern struct pbuf *p;
extern Data OVdata;

void Task_LED3(void *p_arg)	   
{
	(void)p_arg;

		//摄像头
	SCCB_GPIO_Configuration();	//此任务的优先级最高，最先执行，故将初始化代码放在此处
    FIFO_GPIO_Configuration();
    while(Ov7725_Init() != SUCCESS);
    VSYNC_Init();    						
    Ov7725_vsync = 0;		 

	//LWIP初始化
	UDP_task();
	OSTimeDlyHMSM(0,0,0,100); //等待其他任务初始化完毕
	
//	p = pbuf_alloc(PBUF_RAW,sizeof(OVdata),PBUF_RAM);
//	p->payload = (void *)(&OVdata);
	while(1)
	{
		ethernetif_input(&enc28j60); 			//轮询是否接收到数据

        if( Ov7725_vsync == 2 )
        {
			FIFO_PREPARE;  			/*FIFO准备*/
            Get_imag_and_discor();	/*采集并显示*/
            Ov7725_vsync = 0;
			Frame_Count++;			/*帧计数器加1*/
        }

		LED3(ON);
		OSTimeDlyHMSM(0,0,0,4);
		LED3(OFF);
	}		
}	   
