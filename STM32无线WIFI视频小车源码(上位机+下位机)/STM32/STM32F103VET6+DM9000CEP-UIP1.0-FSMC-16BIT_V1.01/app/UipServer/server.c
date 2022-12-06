#include "stm32f10x.h"
#include  "uip.h"  
#include "uipopt.h"
#include "uip_arch.h"
#include <string.h>	
#include "dm9000x.h" 
#include "server.h"

void etherdev_init(void);
extern void httpd_appcall(void);
extern void tcp_demo_appcall(void);
extern void server_udp_appcall(void);
extern unsigned char server_udp_flag;

unsigned char mac_addr[6] = {0x00,0xe0,0x3d,0xf4,0xdd,0xf7};	//MAC地址   
/*******************************************************************************
*	函数名: etherdev_init
*	参  数: 无
*	返  回: 无
*	功  能: uIP 接口函数,初始化网卡
********************************************************************************/
void etherdev_init(void)
{	 
	dm9000x_inital(mac_addr);
//	dm9000x_read_id( );
	//dm9000x_set_macaddr(mac_addr);
}


/*******************************************************************************
*	函数名: void tcp_server_appcall(void)
*	参  数: 
*	返  回: 无
*	功  能: 应用接口函数--完成TCP服务器和HTTP服务器
**************************************************************************/
void tcp_server_appcall(void){		
	switch(uip_conn->lport) 
	{
		case HTONS(5005):			           //http服务器
			httpd_appcall(); 
			break;
		case HTONS(1200):				   //TCP服务器
		    tcp_demo_appcall(); 
			break;
	}
}

void udp_server_appcall(void){
	#if 0
	if(SERVER_UDP_DHCP == server_udp_flag)
    	{
		printf("#");
		dhcpc_appcall();
		}
	#endif

	server_udp_appcall();
}

