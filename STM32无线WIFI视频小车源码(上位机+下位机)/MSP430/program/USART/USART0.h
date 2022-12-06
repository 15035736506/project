#ifndef _USART0_H
#define _USART0_H

void InitUSART(void);
void USARTSend1Char(char sendchar);
void USARTSendStr(char *ptr);
unsigned char Get1Char(void);


#endif