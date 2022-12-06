#ifndef __SERVER_H
#define __SERVER_H

#define SERVER_UDP_NULL    0x00
#define SERVER_UDP_DHCP    0x01
#define SERVER_UDP_UDP     0x02

void tcp_server_appcall(void);
void udp_server_appcall(void);

#endif
