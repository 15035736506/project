#include "includes.h"
#include "Imag_App.h"
#include "udp.h"

extern struct udp_pcb *UdpPcb;
extern struct pbuf *p;
extern struct netif enc28j60;

/**外部函数*/
extern err_t ethernetif_init( struct netif *netif );  //在ethernetif.c中定义
extern err_t ethernetif_input( struct netif *netif );  //在ethernetif.c中定义

Data OVdata,OVdata2;

void Get_imag_and_discor(void)
{
    u16 i, j;
    u16 Camera_Data;
#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0u;
#endif
    for(i = 0; i < 240; i++)
    {	OS_ENTER_CRITICAL();
        for(j = 0; j < 160; j++)
        {

            READ_FIFO_PIXEL(Camera_Data);		//从FIFO读出一个rgb565像素到Camera_Data变量
			OVdata.Data[j] = Camera_Data;
        }


		for(j = 0; j < 160; j++) //把数据切割成小块发送，保证enc28j60稳定
        {

            READ_FIFO_PIXEL(Camera_Data);		//从FIFO读出一个rgb565像素到Camera_Data变量
			OVdata2.Data[j] = Camera_Data;		   
        }
//        READ_FIFO_PIXEL(Camera_Data);//不知道为什么要多读出一个像素，没看懂

			p = pbuf_alloc(PBUF_RAW,sizeof(OVdata),PBUF_RAM);
			OVdata.x = i;
			OVdata.y = 0;
			p->payload = (void *)(&OVdata);
			udp_send(UdpPcb,p);
			pbuf_free(p);

			p = pbuf_alloc(PBUF_RAW,sizeof(OVdata2),PBUF_RAM);
			OVdata2.x = i;
			OVdata2.y = 1;
			p->payload = (void *)(&OVdata2);
			udp_send(UdpPcb,p);
			pbuf_free(p);
//        LCD_WR_Data(0);

		
//		OSSchedLock();

/*		p = pbuf_alloc(PBUF_RAW,sizeof(OVdata),PBUF_RAM);
//		p = pbuf_alloc(PBUF_RAW,sizeof(OVdata),PBUF_POOL);
		OVdata.x = i;
		OVdata.y = 0;
		p->payload = (void *)(&OVdata);
		udp_send(UdpPcb,p);
		pbuf_free(p);
*/
		ethernetif_input(&enc28j60); 	//轮询是否接收到数据
//		OSSchedUnlock();
		OS_EXIT_CRITICAL();
//		OSTimeDlyHMSM(0,0,0,100);
    }
}
