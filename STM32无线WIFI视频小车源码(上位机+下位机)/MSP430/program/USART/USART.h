#ifndef _USART_H
#define _USART_H


void InitUSART();
void USARTSend1Char(char sendchar);
void USARTSendStr(char *ptr);
unsigned char Get1Char(void);

#endif