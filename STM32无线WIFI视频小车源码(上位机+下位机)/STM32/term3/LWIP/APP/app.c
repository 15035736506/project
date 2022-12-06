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
    (void)p_arg;                		// 'p_arg' ��û���õ�����ֹ��������ʾ����

	OSTaskCreate(Task_LED3,(void *)0,	//�Ƚ�����������������������ִ��
	   &task_led3_stk[TASK_LED3_STK_SIZE-1], TASK_LED3_PRIO);
   	OSTaskCreate(Task_LED2,(void *)0,
	   &task_led2_stk[TASK_LED2_STK_SIZE-1], TASK_LED2_PRIO);

    while (1)						//TCP��ʱ��
    {
        LED1( ON );
        OSTimeDlyHMSM(0, 0,0,250);
        LED1( OFF);
		tcp_tmr();
		OSTimeDlyHMSM(0, 0,0,250);
		tcp_tmr();	   //ÿ250ms����һ��        
    }
}

void Task_LED2(void *p_arg)			//arp����
{
	(void)p_arg;
	while(1)
	{
		LED2(ON);
		OSTimeDlyHMSM(0,0,5,0);
		etharp_tmr();  //ÿ5s����һ��
		LED2(OFF);
		OSTimeDlyHMSM(0,0,5,0);
		etharp_tmr();  //ÿ5s����һ��
	}
}

extern struct udp_pcb *UdpPcb;
extern struct pbuf *p;
extern Data OVdata;

void Task_LED3(void *p_arg)	   
{
	(void)p_arg;

		//����ͷ
	SCCB_GPIO_Configuration();	//����������ȼ���ߣ�����ִ�У��ʽ���ʼ��������ڴ˴�
    FIFO_GPIO_Configuration();
    while(Ov7725_Init() != SUCCESS);
    VSYNC_Init();    						
    Ov7725_vsync = 0;		 

	//LWIP��ʼ��
	UDP_task();
	OSTimeDlyHMSM(0,0,0,100); //�ȴ����������ʼ�����
	
//	p = pbuf_alloc(PBUF_RAW,sizeof(OVdata),PBUF_RAM);
//	p->payload = (void *)(&OVdata);
	while(1)
	{
		ethernetif_input(&enc28j60); 			//��ѯ�Ƿ���յ�����

        if( Ov7725_vsync == 2 )
        {
			FIFO_PREPARE;  			/*FIFO׼��*/
            Get_imag_and_discor();	/*�ɼ�����ʾ*/
            Ov7725_vsync = 0;
			Frame_Count++;			/*֡��������1*/
        }

		LED3(ON);
		OSTimeDlyHMSM(0,0,0,4);
		LED3(OFF);
	}		
}	   
