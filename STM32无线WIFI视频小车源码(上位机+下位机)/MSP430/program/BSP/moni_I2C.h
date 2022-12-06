#ifndef moni_I2C_H
#define moni_I2C_H

void I2C_init();
void I2C_start();
void I2C_stop();
unsigned char I2C_respon();
void I2C_ACK();
void I2C_writebyte(unsigned char data);
char I2C_readbyte();

#endif