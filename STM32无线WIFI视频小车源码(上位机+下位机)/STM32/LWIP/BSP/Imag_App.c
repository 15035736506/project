#include "includes.h"
#include "Imag_App.h"
#include "udp.h"

extern struct udp_pcb *UdpPcb;
extern struct pbuf *p;
extern struct netif enc28j60;

/**�ⲿ����*/
extern err_t ethernetif_init( struct netif *netif );  //��ethernetif.c�ж���
extern err_t ethernetif_input( struct netif *netif );  //��ethernetif.c�ж���

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

            READ_FIFO_PIXEL(Camera_Data);		//��FIFO����һ��rgb565���ص�Camera_Data����
			OVdata.Data[j] = Camera_Data;
        }


		for(j = 0; j < 160; j++) //�������и��С�鷢�ͣ���֤enc28j60�ȶ�
        {

            READ_FIFO_PIXEL(Camera_Data);		//��FIFO����һ��rgb565���ص�Camera_Data����
			OVdata2.Data[j] = Camera_Data;		   
        }
//        READ_FIFO_PIXEL(Camera_Data);//��֪��ΪʲôҪ�����һ�����أ�û����

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
		ethernetif_input(&enc28j60); 	//��ѯ�Ƿ���յ�����
//		OSSchedUnlock();
		OS_EXIT_CRITICAL();
//		OSTimeDlyHMSM(0,0,0,100);
    }
}
