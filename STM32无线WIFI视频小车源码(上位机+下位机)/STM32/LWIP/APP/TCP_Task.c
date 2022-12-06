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

	pcb = tcp_new();          //建立通信的TCP控制块（pcb）
	tcp_bind(pcb,IP_ADDR_ANY,80);	   //绑定本地IP地址和端口号
	pcb = tcp_listen(pcb);			//进入监听状态
	tcp_accept(pcb, CallbackTCP_Cmd);	 //设置连接请求时的回调函数
}

static err_t Callback_Cmd()
{
	tcp_setprio(pcb, TCP_PRIO_MIN); //设置回调函数优先级，当存在几个连接时特别重要，此函数必须调用

	tcp_recv(pcb, TCPcmd_recv);   //设置TCP段到时的回调函数

	err = ERR_OK;
	return;
}

static err_t TCPcmd_recv()
{
	if(P!=NULL)
	{
		//有命令数据到达，分析后传递给上层
		USART1_printf(USART1,p->payload);
		pbuf_free(p);
	}
//	tcp_close(pcb);
	err = ERR_OK;
	return err;
}