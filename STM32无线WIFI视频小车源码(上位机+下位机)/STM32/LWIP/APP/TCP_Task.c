#include "includes.h"
#include "usart1.h"

#include "err.h"  
#include "lwip/init.h"
#include "etharp.h"
#include "udp.h"
#include "tcp.h"
#include "spi.h"

void TCP_Task()
{
	struct tcp_pcb *pcb;

	pcb = tcp_new();          //����ͨ�ŵ�TCP���ƿ飨pcb��
	tcp_bind(pcb,IP_ADDR_ANY,80);	   //�󶨱���IP��ַ�Ͷ˿ں�
	pcb = tcp_listen(pcb);			//�������״̬
	tcp_accept(pcb, CallbackTCP_Cmd);	 //������������ʱ�Ļص�����
}

static err_t Callback_Cmd()
{
	tcp_setprio(pcb, TCP_PRIO_MIN); //���ûص��������ȼ��������ڼ�������ʱ�ر���Ҫ���˺����������

	tcp_recv(pcb, TCPcmd_recv);   //����TCP�ε�ʱ�Ļص�����

	err = ERR_OK;
	return;
}

static err_t TCPcmd_recv()
{
	if(P!=NULL)
	{
		//���������ݵ�������󴫵ݸ��ϲ�
		USART1_printf(USART1,p->payload);
		pbuf_free(p);
	}
//	tcp_close(pcb);
	err = ERR_OK;
	return err;
}