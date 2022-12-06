#ifndef __DELAY_H
#define __DELAY_H

#include<msp430f149.h>

#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//void delayms(unsigned int Nms);
//void delayus(unsigned int Nus);

#endif