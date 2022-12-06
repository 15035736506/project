#include "includes.h"
#include "usart1.h"

#include "err.h"  
#include "lwip/init.h"
#include "etharp.h"
#include "udp.h"
#include "tcp.h"
#include "spi.h"

#include "ENC28J60.H"

struct netif enc28j60;	  		//netif 网络接口结构，用于描述网络硬件接口的特性
//uint8_t macaddress[6]={0x54,0x55,0x58,0x10,0x00,0x24};	//设置网络接口的mac地址


/**外部函数*/
extern err_t ethernetif_init( struct netif *netif );  //在ethernetif.c中定义
extern err_t ethernetif_input( struct netif *netif );  //在ethernetif.c中定义

void UDP_receive(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/*
 * 函数名：LWIP_Init
 * 描述  ：初始化LWIP协议栈，主要是把enc28j60与LWIP连接起来。
 			包括IP、MAC地址，接口函数
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void LwIP_Init( void )
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

   /*调用LWIP初始化函数，
   初始化网络接口结构体链表、内存池、pbuf结构体*/
   lwip_init();	
	  
#if LWIP_DHCP			   					//若使用DHCP协议
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0; 
#else										//
  IP4_ADDR(&ipaddr, 192, 168, 159, 19);  		//设置网络接口的ip地址
  IP4_ADDR(&netmask, 255, 255, 255, 0);		//子网掩码
  IP4_ADDR(&gw, 192, 168, 159, 1);			//网关

#endif
   
  /*初始化enc28j60与LWIP的接口，参数为网络接口结构体、ip地址、
  子网掩码、网关、网卡信息指针、初始化函数、输入函数*/
  netif_add(&enc28j60, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
 
  /*把enc28j60设置为默认网卡 .*/
  netif_set_default(&enc28j60);


#if LWIP_DHCP	   			//若使用了DHCP
  /*  Creates a new DHCP client for this interface on the first call.
  Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
  the predefined regular intervals after starting the client.
  You can peek in the netif->dhcp struct for the actual DHCP status.*/
  dhcp_start(&enc28j60);   	//启动DHCP
#endif

  /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&enc28j60); //使能enc28j60接口
}

	struct udp_pcb *UdpPcb;
	struct pbuf *p;

int UDP_task()		  //UDP客户端通信实例
{
	const static s8_t UDPData[] = "LWIP UDP 客户端在STM32F103VET6的测试 \r\n";
		
	struct ip_addr ipaddr1;//存放远程主机ip	

	USART1_Config();
	USART1_printf(USART1,"starting!");

	/*初始化 以太网SPI接口*/
	ENC_SPI_Init(); 

	/*初始化LWIP协议栈*/
	LwIP_Init();

	p = pbuf_alloc(PBUF_RAW,sizeof(UDPData),PBUF_RAM);
	p->payload = (void *)UDPData;

	IP4_ADDR(&ipaddr1,192,168,159,18);	 //加载远程ip
//	IP4_ADDR(&ipaddr1,192,168,159,165);	 //加载远程ip
	UdpPcb = udp_new();	 //建立UDP通信控制块

	udp_bind(UdpPcb,IP_ADDR_ANY,1025); //绑定本地IP地址
	udp_connect(UdpPcb,&ipaddr1,1025);  //连接远程主机

	udp_recv(UdpPcb, UDP_receive, NULL);  //指定连接UdpPcb的回调函数UDP_receive

//	udp_send(UdpPcb,p);

	pbuf_free(p);

	return 0;
}

void UDP_receive(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
//	struct ip_addr destAddr = *addr;		//获取远程主机IP地址
//	unsigned char datalen;
#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	if(p != NULL)
	{
		OS_ENTER_CRITICAL();
//		USART_SendData(USART1,"success!");
//		datalen = (unsigned char)p->len;
//		USART1_printf(USART1,&datalen);

		USART1_printf(USART1,p->payload);
//		enc28j60_soft_RST();
	
//		USART1_printf(USART1,"success!");
		pbuf_free(p);
		OS_EXIT_CRITICAL();
	
	}
}
