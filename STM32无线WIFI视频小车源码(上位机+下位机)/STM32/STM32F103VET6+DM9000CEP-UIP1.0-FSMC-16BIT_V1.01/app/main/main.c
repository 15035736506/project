#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include <stdio.h>	 
#include "systick.h"

#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"
#include  <stdarg.h>

#include "server.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	

void net_inital(void);	
void UipPro(void);
void Delay(vu32 nCount);
void GPIO_Configuration(void);
void RCC_Configuration(void);
char *itoa(int value, char *string, int radix);
void eth_poll(void);

unsigned char led_flag = 0x00;
unsigned char DHCP_SUCCESS_FLAG = 0x00;
unsigned char server_udp_flag;

unsigned char ETH_INT=0;

void LED_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 						 

    /* LED -> PB1 */					 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/****************************************************************************
* ��    �ƣ�void Usart1_Init(void)
* ��    �ܣ�����1��ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void Usart1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);	 		//ʹ�ܴ���1ʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 		//USART1 TX
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 		//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 		//A�˿� 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 		//USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 		//���ÿ�©����
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 		//A�˿� 

  USART_InitStructure.USART_BaudRate = 115200;						//����115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);							//���ô��ڲ�������   
   /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);	
  
}

/****************************************************************************
* ��    �ƣ�void Usart1_Init(void)
* ��    �ܣ�����1��ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void Usart2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE);	 		//ʹ�ܴ���1ʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         		 		//USART1 TX
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 		//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 		//A�˿� 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         	 		//USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 		//���ÿ�©����
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 		//A�˿� 

  USART_InitStructure.USART_BaudRate = 115200;						//����115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

  /* Configure USART1 */
  USART_Init(USART2, &USART_InitStructure);							//���ô��ڲ�������   
   /* Enable the USART1 */
  USART_Cmd(USART2, ENABLE);	
  
}

/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void RCC_Configuration(void)
{
 // RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO  , ENABLE); 
   SystemInit();
}

struct timer periodic_timer, arp_timer;

/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ��������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
int main(void)
{
	const uint8_t mac_addr[6] = {0x00,0xe0,0x3d,0xf4,0xdd,0xf7};
	int i;
	struct uip_eth_addr ethaddr;
	struct timer t;
    DHCP_SUCCESS_FLAG = 0x00;
	
	RCC_Configuration();				 //ϵͳʱ������
	LED_config();
	Usart1_Init();                       //����1��ʼ��
	Usart2_Init();

	timer_set(&t, 100);
    
	//����ʱ��
    clock_arch_init(); 
    timer_set(&periodic_timer, CLOCK_SECOND / 2);
    timer_set(&arp_timer, CLOCK_SECOND * 10);	
	
	/* ��ʾ����Logo */
    printf("**** system is run *******\r\n");    	  //�򴮿�1���Ϳ����ַ���	
    
	net_inital();		/* ��ʼ�������豸�Լ�UIPЭ��ջ������IP��ַ */

	//Ӧ�ò�MAC��ʼ��
    for (i = 0; i < 6; i++) 
		ethaddr.addr[i] = mac_addr[i];
	
    uip_setethaddr(ethaddr);//UIP MAC��ʼ��

    //UIP��ʼ��
    uip_init();

    //ARP��ʼ��
    uip_arp_init();

    dhcpc_init(&mac_addr, 6);

	/* ����һ��TCP�����˿ں�http�����˿ڣ��˿ں�Ϊ1200��80 */
	uip_listen(HTONS(1200));
	uip_listen(HTONS(5005));	 
	while (1)
	{	
		/* ����uip�¼���������뵽�û������ѭ������ */
		UipPro();			  //�жϴ�����ȡ������ջ���
		eth_poll();           //��ʱ��ѯTCP��UDP�����շ�״̬	ARP����£� ����Ӧ
	}
}
/****************************************************************************
* ��    �ƣ�void eth_poll(void)
* ��    �ܣ���ʱ��ѯTCP�����շ�״̬	ARP����£� ����Ӧ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void eth_poll(void){
	unsigned char i=0;
    if(timer_expired(&periodic_timer)) 
	{
        if( 0x02 == DHCP_SUCCESS_FLAG)
        	{
		    led_flag = ~led_flag;

	        (led_flag == 0x00) ? 						   \
		    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET) :	   \
		    GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
        	}

		timer_reset(&periodic_timer);
		
		/* ��������ÿ��TCP����, UIP_CONNSȱʡ��10�� */
		for(i = 0; i < UIP_CONNS; i++)
		{
			uip_periodic(i);		/* ����TCPͨ���¼� */
			/*
				������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
				��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)
			*/
			if(uip_len > 0)
			{
				uip_arp_out();		//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();		//�������ݵ���̫�����豸��������
			}
		}

	#if UIP_UDP
		/* ��������ÿ��UDP����, UIP_UDP_CONNSȱʡ��10�� */
		for(i = 0; i < UIP_UDP_CONNS; i++)
		{
			uip_udp_periodic(i);	/*����UDPͨ���¼� */
			/* �������ĺ������õ�������Ӧ�ñ����ͳ�ȥ��ȫ�ֱ���uip_len�趨ֵ> 0 */
			if(uip_len > 0)
			{
				uip_arp_out();		//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();		//�������ݵ���̫�����豸��������
			}
		}
	#endif /* UIP_UDP */
	
	/* Call the ARP timer function every 10 seconds. */
            if(timer_expired(&arp_timer)) 
            {
                timer_reset(&arp_timer);
                uip_arp_timer();
            }
	}
}
/*******************************************************************************
*	��������UipPro
*	��  ��:
*	��  ��:
*	����˵�����жϴ�����ȡ������ջ���
********************************************************************/
void UipPro(void)
{
	/*if(ETH_INT==1)*/{					//��������յ�����ʱ��������ж�
		rep:;
		ETH_INT=0;
		uip_len = tapdev_read();	//�������豸��ȡһ��IP��,�������ݳ���
		if(uip_len > 0)			    //�յ�����
		{
			/* ����IP���ݰ�(ֻ��У��ͨ����IP���Żᱻ����) */
			if(BUF->type == htons(UIP_ETHTYPE_IP))   //��IP����
			{
				uip_arp_ipin();		   //ȥ����̫��ͷ�ṹ������ARP��
				uip_input();		   //IP������
				/*
					������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
					��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)
				*/
				if (uip_len > 0)		//�д����Ӧ����
				{
					uip_arp_out();		//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
					tapdev_send();		//�������ݵ���̫�����豸��������
				}
			}
			/* ����arp���� */
			else if (BUF->type == htons(UIP_ETHTYPE_ARP))	//��ARP�����
			{
				uip_arp_arpin();		//������ARP��Ӧ������ARP����������󣬹����Ӧ���ݰ�
				/*
					������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
					��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)
				*/
				if (uip_len > 0)		//��ARP����Ҫ���ͻ�Ӧ
				{
					tapdev_send();		//��ARP��Ӧ����̫����
				}
			}
		}
  	}
//  	else{	                   //��ֹ�����ɽ�������,��û�в����жϣ����ж��ź�ʼ��Ϊ��˵����������
//  	  if(ETH_rec_f==0) goto rep; 	
//  	}
}

#ifdef __DHCPC_H__
void
dhcpc_configured(const struct dhcpc_state *s)
{
  uip_sethostaddr(s->ipaddr);
  uip_setnetmask(s->netmask);
  uip_setdraddr(s->default_router);
  //resolv_conf(s->dnsaddr);
 
  printf("Got IP address %d.%d.%d.%d\n",
	 uip_ipaddr1(s->ipaddr), uip_ipaddr2(s->ipaddr),
	 uip_ipaddr3(s->ipaddr), uip_ipaddr4(s->ipaddr));
   #if 0
   printf("Got netmask %d.%d.%d.%d\n",
	 uip_ipaddr1(s->netmask), uip_ipaddr2(s->netmask),
	 uip_ipaddr3(s->netmask), uip_ipaddr4(s->netmask));
  printf("Got DNS server %d.%d.%d.%d\n",
	 uip_ipaddr1(s->dnsaddr), uip_ipaddr2(s->dnsaddr),
	 uip_ipaddr3(s->dnsaddr), uip_ipaddr4(s->dnsaddr));
  printf("Got default router %d.%d.%d.%d\n",
	 uip_ipaddr1(s->default_router), uip_ipaddr2(s->default_router),
	 uip_ipaddr3(s->default_router), uip_ipaddr4(s->default_router));
  printf("Lease expires in %ld seconds\n",
	 ntohs(s->lease_time[0])*65536ul + ntohs(s->lease_time[1]));
  #endif

  DHCP_SUCCESS_FLAG = 0x02;
  server_udp_flag = SERVER_UDP_UDP;
}
#endif /* __DHCPC_H__ */


/*******************************************************************************
*	��������InitNet
*	��  ��:
*	��  ��:
*	����˵������ʼ������Ӳ����UIPЭ��ջ�����ñ���IP��ַ
************************************************************/
void net_inital(void)
{
	uip_ipaddr_t ipaddr;
	tapdev_init();                     		 //dm9000aep��ʼ��
	
//	uip_init();								 //UIPЭ��ջ��ʼ��
	server_udp_flag = SERVER_UDP_NULL;
	server_udp_flag = SERVER_UDP_DHCP;
	
	#if 1
	printf("uip ip address : 192,168,1,8\r\n");
	uip_ipaddr(ipaddr, 192,168,1,8);		 //����IP��ַ
	uip_sethostaddr(ipaddr);

	printf("uip route address : 192,168,1,1\r\n");
	uip_ipaddr(ipaddr, 192,168,1,1);		 //����Ĭ��·����IP��ַ
	uip_setdraddr(ipaddr);

	printf("uip net mask : 255,255,255,0\r\n");
	uip_ipaddr(ipaddr, 255,255,255,0);		 //������������
	uip_setnetmask(ipaddr);
    #endif
}
/****************************************************************************
* ��    �ƣ�void GPIO_Configuration(void)
* ��    �ܣ�ͨ��IO������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void GPIO_Configuration(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE, ENABLE);
  	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;		 //LED2, LED3����
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					 //SST25VF016B SPIƬѡ
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_7;		 //PB12---VS1003 SPIƬѡ��V2.1) 
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //PB7---������оƬXPT2046 SPI Ƭѡ
  
  /* ��ֹSPI1�����ϵ������豸 */
  GPIO_SetBits(GPIOB, GPIO_Pin_7);						     //������оƬXPT2046 SPI Ƭѡ��ֹ  
  GPIO_SetBits(GPIOB, GPIO_Pin_12);						     //VS1003 SPIƬѡ��V2.1)��ֹ 
  GPIO_SetBits(GPIOC, GPIO_Pin_4);						     //SST25VF016B SPIƬѡ��ֹ  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	         	 	//ENC28J60��������ж����� 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   	 		//�ڲ���������
  GPIO_Init(GPIOA, &GPIO_InitStructure);		 
}


void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
}

/******************************************************
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

int fputc(int _ch, FILE *f)
{
    if(_ch == '\n') putchar('\r');
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART2->DR = (uint8_t) _ch;

	return _ch;
}


