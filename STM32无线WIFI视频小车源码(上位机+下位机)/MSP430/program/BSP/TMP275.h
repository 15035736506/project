#ifndef _TMP275_H
#define _TMP275_H


extern int temperture;

#define temp_reg 0x00;//ָ��Ĵ���ָ���¶ȼĴ���
#define conf_reg 0x01;//ָ��Ĵ���ָ�����üĴ���
#define T_high_reg 0x02;//
#define T_low_reg 0x03;//

//���üĴ���������ʽ
#define conf1 0x6000;//����ת�����ֱ���0.0625����OS
#define conf2 0x6001;//�ض�ģʽ���ֱ���0.0625����OS
//�ڹض�ģʽ�£������üĴ�����OSλд��1������һ�ε���ת�����Խ�ʡ����
#define confOS 0x8000;


void writeTMP275_reg(unsigned char address,unsigned int data);
void readTMP275_temp(unsigned char address);

#endif