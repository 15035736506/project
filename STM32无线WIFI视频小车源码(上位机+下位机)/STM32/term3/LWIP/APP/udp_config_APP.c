#include "includes.h"

#include "err.h"  
#include "lwip/init.h"
#include "etharp.h"
#include "udp.h"
#include "tcp.h"
#include "spi.h"


struct netif enc28j60;	  		//netif ����ӿڽṹ��������������Ӳ���ӿڵ�����
//uint8_t macaddress[6]={0x54,0x55,0x58,0x10,0x00,0x24};	//��������ӿڵ�mac��ַ


/**�ⲿ����*/
extern err_t ethernetif_init( struct netif *netif );  //��ethernetif.c�ж���
extern err_t ethernetif_input( struct netif *netif );  //��ethernetif.c�ж���

/*
 * ��������LWIP_Init
 * ����  ����ʼ��LWIPЭ��ջ����Ҫ�ǰ�enc28j60��LWIP����������
 			����IP��MAC��ַ���ӿں���
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void LwIP_Init( void )
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

   /*����LWIP��ʼ��������
   ��ʼ������ӿڽṹ�������ڴ�ء�pbuf�ṹ��*/
   lwip_init();	
	  
#if LWIP_DHCP			   					//��ʹ��DHCPЭ��
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0; 
#else										//
  IP4_ADDR(&ipaddr, 192, 168, 159, 18);  		//��������ӿڵ�ip��ַ
  IP4_ADDR(&netmask, 255, 255, 255, 0);		//��������
  IP4_ADDR(&gw, 192, 168, 159, 1);			//����

#endif
   
  /*��ʼ��enc28j60��LWIP�Ľӿڣ�����Ϊ����ӿڽṹ�塢ip��ַ��
  �������롢���ء�������Ϣָ�롢��ʼ�����������뺯��*/
  netif_add(&enc28j60, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
 
  /*��enc28j60����ΪĬ������ .*/
  netif_set_default(&enc28j60);


#if LWIP_DHCP	   			//��ʹ����DHCP
  /*  Creates a new DHCP client for this interface on the first call.
  Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
  the predefined regular intervals after starting the client.
  You can peek in the netif->dhcp struct for the actual DHCP status.*/
  dhcp_start(&enc28j60);   	//����DHCP
#endif

  /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&enc28j60); //ʹ��enc28j60�ӿ�
}

	struct udp_pcb *UdpPcb;
	struct pbuf *p;

int UDP_task()		  //UDP�ͻ���ͨ��ʵ��
{
	const static s8_t UDPData[] = "LWIP UDP �ͻ�����STM32F103VET6�Ĳ��� \r\n";
		

	struct ip_addr ipaddr1;//���Զ������ip	

	signed char err;

	/*��ʼ�� ��̫��SPI�ӿ�*/
	ENC_SPI_Init(); 

	/*��ʼ��LWIPЭ��ջ*/
	LwIP_Init();

	p = pbuf_alloc(PBUF_RAW,sizeof(UDPData),PBUF_RAM);
	p->payload = (void *)UDPData;

	IP4_ADDR(&ipaddr1,192,168,159,165);	 //����Զ��ip
	UdpPcb = udp_new();	 //����UDPͨ�ſ��ƿ�

	err = udp_bind(UdpPcb,IP_ADDR_ANY,1025); //�󶨱���IP��ַ
	err = udp_connect(UdpPcb,&ipaddr1,1025);  //����Զ������


	err = udp_send(UdpPcb,p);

	pbuf_free(p);



}
