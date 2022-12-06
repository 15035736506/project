#ifndef _TMP275_H
#define _TMP275_H


extern int temperture;

#define temp_reg 0x00;//指针寄存器指向温度寄存器
#define conf_reg 0x01;//指针寄存器指向配置寄存器
#define T_high_reg 0x02;//
#define T_low_reg 0x03;//

//配置寄存器配置样式
#define conf1 0x6000;//连续转换，分辨率0.0625，无OS
#define conf2 0x6001;//关断模式，分辨率0.0625，无OS
//在关断模式下，向配置寄存器的OS位写入1可启动一次单次转换，以节省功耗
#define confOS 0x8000;


void writeTMP275_reg(unsigned char address,unsigned int data);
void readTMP275_temp(unsigned char address);

#endif