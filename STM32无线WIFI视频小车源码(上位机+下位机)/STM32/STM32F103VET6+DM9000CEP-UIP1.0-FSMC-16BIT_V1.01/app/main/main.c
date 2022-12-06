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
* 名    称：void Usart1_Init(void)
* 功    能：串口1初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void Usart1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);	 		//使能串口1时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 		//USART1 TX
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 		//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 		//A端口 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 		//USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 		//复用开漏输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 		//A端口 

  USART_InitStructure.USART_BaudRate = 115200;						//速率115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);							//配置串口参数函数   
   /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);	
  
}

/****************************************************************************
* 名    称：void Usart1_Init(void)
* 功    能：串口1初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void Usart2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE);	 		//使能串口1时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         		 		//USART1 TX
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 		//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 		//A端口 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	         	 		//USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 		//复用开漏输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 		//A端口 

  USART_InitStructure.USART_BaudRate = 115200;						//速率115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
  USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  /* Configure USART1 */
  USART_Init(USART2, &USART_InitStructure);							//配置串口参数函数   
   /* Enable the USART1 */
  USART_Cmd(USART2, ENABLE);	
  
}

/****************************************************************************
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void)
{
 // RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO  , ENABLE); 
   SystemInit();
}

struct timer periodic_timer, arp_timer;

/****************************************************************************
* 名    称：int main(void)
* 功    能：程序入口
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
int main(void)
{
	const uint8_t mac_addr[6] = {0x00,0xe0,0x3d,0xf4,0xdd,0xf7};
	int i;
	struct uip_eth_addr ethaddr;
	struct timer t;
    DHCP_SUCCESS_FLAG = 0x00;
	
	RCC_Configuration();				 //系统时钟设置
	LED_config();
	Usart1_Init();                       //串口1初始化
	Usart2_Init();

	timer_set(&t, 100);
    
	//基本时钟
    clock_arch_init(); 
    timer_set(&periodic_timer, CLOCK_SECOND / 2);
    timer_set(&arp_timer, CLOCK_SECOND * 10);	
	
	/* 显示例程Logo */
    printf("**** system is run *******\r\n");    	  //向串口1发送开机字符。	
    
	net_inital();		/* 初始化网络设备以及UIP协议栈，配置IP地址 */

	//应用层MAC初始化
    for (i = 0; i < 6; i++) 
		ethaddr.addr[i] = mac_addr[i];
	
    uip_setethaddr(ethaddr);//UIP MAC初始化

    //UIP初始化
    uip_init();

    //ARP初始化
    uip_arp_init();

    dhcpc_init(&mac_addr, 6);

	/* 创建一个TCP监听端口和http监听端口，端口号为1200，80 */
	uip_listen(HTONS(1200));
	uip_listen(HTONS(5005));	 
	while (1)
	{	
		/* 处理uip事件，必须插入到用户程序的循环体中 */
		UipPro();			  //中断触发读取网络接收缓存
		eth_poll();           //定时查询TCP及UDP连接收发状态	ARP表更新， 并响应
	}
}
/****************************************************************************
* 名    称：void eth_poll(void)
* 功    能：定时查询TCP连接收发状态	ARP表更新， 并响应
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
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
		
		/* 轮流处理每个TCP连接, UIP_CONNS缺省是10个 */
		for(i = 0; i < UIP_CONNS; i++)
		{
			uip_periodic(i);		/* 处理TCP通信事件 */
			/*
				当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
				需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)
			*/
			if(uip_len > 0)
			{
				uip_arp_out();		//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();		//发送数据到以太网（设备驱动程序）
			}
		}

	#if UIP_UDP
		/* 轮流处理每个UDP连接, UIP_UDP_CONNS缺省是10个 */
		for(i = 0; i < UIP_UDP_CONNS; i++)
		{
			uip_udp_periodic(i);	/*处理UDP通信事件 */
			/* 如果上面的函数调用导致数据应该被发送出去，全局变量uip_len设定值> 0 */
			if(uip_len > 0)
			{
				uip_arp_out();		//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();		//发送数据到以太网（设备驱动程序）
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
*	函数名：UipPro
*	输  入:
*	输  出:
*	功能说明：中断触发读取网络接收缓存
********************************************************************/
void UipPro(void)
{
	/*if(ETH_INT==1)*/{					//当网络接收到数据时，会产生中断
		rep:;
		ETH_INT=0;
		uip_len = tapdev_read();	//从网络设备读取一个IP包,返回数据长度
		if(uip_len > 0)			    //收到数据
		{
			/* 处理IP数据包(只有校验通过的IP包才会被接收) */
			if(BUF->type == htons(UIP_ETHTYPE_IP))   //是IP包吗？
			{
				uip_arp_ipin();		   //去除以太网头结构，更新ARP表
				uip_input();		   //IP包处理
				/*
					当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
					需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)
				*/
				if (uip_len > 0)		//有带外回应数据
				{
					uip_arp_out();		//加以太网头结构，在主动连接时可能要构造ARP请求
					tapdev_send();		//发送数据到以太网（设备驱动程序）
				}
			}
			/* 处理arp报文 */
			else if (BUF->type == htons(UIP_ETHTYPE_ARP))	//是ARP请求包
			{
				uip_arp_arpin();		//如是是ARP回应，更新ARP表；如果是请求，构造回应数据包
				/*
					当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
					需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)
				*/
				if (uip_len > 0)		//是ARP请求，要发送回应
				{
					tapdev_send();		//发ARP回应到以太网上
				}
			}
		}
  	}
//  	else{	                   //防止大包造成接收死机,当没有产生中断，而中断信号始终为低说明接收死机
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
*	函数名：InitNet
*	输  入:
*	输  出:
*	功能说明：初始化网络硬件、UIP协议栈、配置本机IP地址
************************************************************/
void net_inital(void)
{
	uip_ipaddr_t ipaddr;
	tapdev_init();                     		 //dm9000aep初始化
	
//	uip_init();								 //UIP协议栈初始化
	server_udp_flag = SERVER_UDP_NULL;
	server_udp_flag = SERVER_UDP_DHCP;
	
	#if 1
	printf("uip ip address : 192,168,1,8\r\n");
	uip_ipaddr(ipaddr, 192,168,1,8);		 //设置IP地址
	uip_sethostaddr(ipaddr);

	printf("uip route address : 192,168,1,1\r\n");
	uip_ipaddr(ipaddr, 192,168,1,1);		 //设置默认路由器IP地址
	uip_setdraddr(ipaddr);

	printf("uip net mask : 255,255,255,0\r\n");
	uip_ipaddr(ipaddr, 255,255,255,0);		 //设置网络掩码
	uip_setnetmask(ipaddr);
    #endif
}
/****************************************************************************
* 名    称：void GPIO_Configuration(void)
* 功    能：通用IO口配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
void GPIO_Configuration(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE, ENABLE);
  	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1控制
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;		 //LED2, LED3控制
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					 //SST25VF016B SPI片选
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_7;		 //PB12---VS1003 SPI片选（V2.1) 
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //PB7---触摸屏芯片XPT2046 SPI 片选
  
  /* 禁止SPI1总线上的其他设备 */
  GPIO_SetBits(GPIOB, GPIO_Pin_7);						     //触摸屏芯片XPT2046 SPI 片选禁止  
  GPIO_SetBits(GPIOB, GPIO_Pin_12);						     //VS1003 SPI片选（V2.1)禁止 
  GPIO_SetBits(GPIOC, GPIO_Pin_4);						     //SST25VF016B SPI片选禁止  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	         	 	//ENC28J60接收完成中断引脚 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   	 		//内部上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);		 
}


void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
}

/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
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


