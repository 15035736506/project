#include"moni_I2C.h"

int temperture=0;//定义温度全局变量

//写寄存器函数
void writeTMP275_reg(unsigned char address,unsigned int data)
{
  unsigned char data_H,data_L;
  data_L = (unsigned char)data;
  data_H = (unsigned char)(data>>8);
   I2C_init();
  I2C_start();
  I2C_writebyte(0x90);//器件地址线为0,写
  I2C_respon();
  I2C_writebyte(address);
  I2C_respon();
  I2C_writebyte(data_H);
  I2C_respon();
  I2C_writebyte(data_L);
  I2C_respon();  
  I2C_stop();
}

//读温度函数
void readTMP275_temp(unsigned char address)
{
  unsigned int temp = 0,xiaoshu=0;
  float f_temp;
  
   I2C_init();
  I2C_start();
  I2C_writebyte(0x90);//器件地址线为0，写
  I2C_respon();
  I2C_writebyte(address);
  I2C_respon();
  
  I2C_start();
  I2C_writebyte(0x91);//器件地址线为0，读
  I2C_respon();
  
  temp = I2C_readbyte();//第一字节,在0.625的分辨率下每位代表1度
  I2C_ACK();
//  zhuanhuan_4(0,2,temp);
  xiaoshu = I2C_readbyte();//第二字节  
  I2C_ACK();
  I2C_stop();

  f_temp = (xiaoshu>>4)*0.0625;
  xiaoshu = (unsigned int)(f_temp*10 + 0.5);
//  xiaoshu = temp*10 +  (unsigned int)(xiaoshu);//非常奇怪，数据超过了256居然会溢出？？？
//  zhuanhuan_4(0,3,xiaoshu);
//  return (xiaoshu);//return只能返回256以下的数据？？？
  //已证实，return只能返回一个256以下的数据，否则就会发生溢出。又一个惊奇的发现！！！
  temperture = temp*10 + xiaoshu;
}