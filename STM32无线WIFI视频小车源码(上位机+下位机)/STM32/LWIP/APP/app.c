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
#include "ENC28J60.H"


OS_STK task_led2_stk[TASK_LED2_STK_SIZE];
OS_STK task_led3_stk[TASK_LED3_STK_SIZE];

extern volatile u8 Frame_Count;	  //����ͷ֡����
__IO uint32_t volatile localtime = 0;

#define TCP_TMR_INTERVAL       250  /* The TCP timer interval in milliseconds. */
#define INPUT_TMR_INTERVAL 	  2

__IO uint32_t TCPTimer = 0;
__IO uint32_t ARPTimer = 0;
__IO uint32_t INPUT_Timer=0;

#ifdef LWIP_DHCP
__IO uint32_t DHCPfineTimer = 0;
__IO uint32_t DHCPcoarseTimer = 0;

#endif 

extern struct udp_pcb *UdpPcb;
extern struct pbuf *p;
extern Data OVdata;

extern int UDP_task();
extern struct netif enc28j60;


void Task_LED(void *p_arg)
{
    (void)p_arg;                		// 'p_arg' ��û���õ�����ֹ��������ʾ����

	OSTaskCreate(Task_LED3,(void *)0,	//�Ƚ�����������������������ִ��
	   &task_led3_stk[TASK_LED3_STK_SIZE-1], TASK_LED3_PRIO);
   	OSTaskCreate(Task_LED2,(void *)0,
	   &task_led2_stk[TASK_LED2_STK_SIZE-1], TASK_LED2_PRIO);
    OSTimeDlyHMSM(0, 0,0,50);
    while (1)						//TCP��ʱ��
    {
        LED1( ON );
        OSTimeDlyHMSM(0, 0,0,250);
        LED1( OFF);
//		tcp_tmr();
		OSTimeDlyHMSM(0, 0,0,250);
//		tcp_tmr();	   //ÿ250ms����һ�� 
		
/*		OSTimeDlyHMSM(0, 0,0,10);
		localtime += 10; 
*/
		      
    }
}

void Task_LED2(void *p_arg)			//arp����
{
	(void)p_arg;
//	OSTimeDlyHMSM(0,0,0,400);
	while(1)
	{
		LED2(ON);
		OSTimeDlyHMSM(0,0,5,0);
//		etharp_tmr();  //ÿ5s����һ��
		LED2(OFF);
		OSTimeDlyHMSM(0,0,5,0);
//		etharp_tmr();  //ÿ5s����һ��

/*		LED2(ON);
		OSSchedLock();
		ethernetif_input(&enc28j60); 			//��ѯ�Ƿ���յ�����
		OSSchedUnlock();
		OSTimeDlyHMSM(0,0,0,5);
		LED2(OFF);
		OSTimeDlyHMSM(0,0,0,5);
*/



	}
}


	s8_t UDPData1[324] ={9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,};
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
	OSTimeDlyHMSM(0,0,0,400); //�ȴ����������ʼ�����
	
//	p = pbuf_alloc(PBUF_RAW,sizeof(OVdata),PBUF_RAM);
//	p->payload = (void *)(&OVdata);
	while(1)
	{
//#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
//    OS_CPU_SR  cpu_sr = 0u;
//#endif
//		OS_ENTER_CRITICAL();
	
		ethernetif_input(&enc28j60); 	//��ѯ�Ƿ���յ�����

//		OS_EXIT_CRITICAL();

		LED3(ON);
		OSTimeDlyHMSM(0,0,0,1);
		LED3(OFF);

        if( Ov7725_vsync == 2 )
        {
			FIFO_PREPARE;  			/*FIFO׼��*/
            Get_imag_and_discor();	/*�ɼ�����ʾ*/
            Ov7725_vsync = 0;
			Frame_Count++;			/*֡��������1*/
        }

/*		p = pbuf_alloc(PBUF_RAW,sizeof(UDPData1),PBUF_RAM);
		p->payload = (void *)UDPData1;
		udp_send(UdpPcb,p);
		pbuf_free(p);
*/
	}		
}	   
