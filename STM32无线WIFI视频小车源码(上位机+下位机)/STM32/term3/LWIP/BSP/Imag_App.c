#include "Imag_App.h"
#include "udp.h"

extern struct udp_pcb *UdpPcb;
extern struct pbuf *p;



Data OVdata;

void Get_imag_and_discor(void)
{
    u16 i, j;
    u16 Camera_Data;

    for(i = 0; i < 240; i++)
    {
        for(j = 0; j < 319; j++)
        {

            READ_FIFO_PIXEL(Camera_Data);		//��FIFO����һ��rgb565���ص�Camera_Data����
			OVdata.Data[j] = Camera_Data;
        }
        READ_FIFO_PIXEL(Camera_Data);//��֪��ΪʲôҪ�����һ�����أ�û����
//        LCD_WR_Data(0);

		p = pbuf_alloc(PBUF_RAW,sizeof(OVdata),PBUF_RAM);
		OVdata.x = i;
		OVdata.y = 0;
		p->payload = (void *)(&OVdata);
		udp_send(UdpPcb,p);
		pbuf_free(p);
    }
}
