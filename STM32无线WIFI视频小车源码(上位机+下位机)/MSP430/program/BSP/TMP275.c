#include"moni_I2C.h"

int temperture=0;//�����¶�ȫ�ֱ���

//д�Ĵ�������
void writeTMP275_reg(unsigned char address,unsigned int data)
{
  unsigned char data_H,data_L;
  data_L = (unsigned char)data;
  data_H = (unsigned char)(data>>8);
   I2C_init();
  I2C_start();
  I2C_writebyte(0x90);//������ַ��Ϊ0,д
  I2C_respon();
  I2C_writebyte(address);
  I2C_respon();
  I2C_writebyte(data_H);
  I2C_respon();
  I2C_writebyte(data_L);
  I2C_respon();  
  I2C_stop();
}

//���¶Ⱥ���
void readTMP275_temp(unsigned char address)
{
  unsigned int temp = 0,xiaoshu=0;
  float f_temp;
  
   I2C_init();
  I2C_start();
  I2C_writebyte(0x90);//������ַ��Ϊ0��д
  I2C_respon();
  I2C_writebyte(address);
  I2C_respon();
  
  I2C_start();
  I2C_writebyte(0x91);//������ַ��Ϊ0����
  I2C_respon();
  
  temp = I2C_readbyte();//��һ�ֽ�,��0.625�ķֱ�����ÿλ����1��
  I2C_ACK();
//  zhuanhuan_4(0,2,temp);
  xiaoshu = I2C_readbyte();//�ڶ��ֽ�  
  I2C_ACK();
  I2C_stop();

  f_temp = (xiaoshu>>4)*0.0625;
  xiaoshu = (unsigned int)(f_temp*10 + 0.5);
//  xiaoshu = temp*10 +  (unsigned int)(xiaoshu);//�ǳ���֣����ݳ�����256��Ȼ�����������
//  zhuanhuan_4(0,3,xiaoshu);
//  return (xiaoshu);//returnֻ�ܷ���256���µ����ݣ�����
  //��֤ʵ��returnֻ�ܷ���һ��256���µ����ݣ�����ͻᷢ���������һ������ķ��֣�����
  temperture = temp*10 + xiaoshu;
}