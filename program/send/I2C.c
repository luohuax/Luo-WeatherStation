#include "I2C.h"

bit I2CBusy;

void I2C_Isr() interrupt 24{
  if(I2CMSST&0x40){
    I2CMSST&=~0x40;//复位中断标志
    I2CMSCR&=0xF0;//清空I2C控制器指令寄存器
    I2CBusy=0;
  }
}

void hardwareI2CInit(void){
  P_SW2|=0x80;
  P1M0&=0xCF;
  P1M1&=0xCF;
  I2CCFG|=0xC1;//使能I2C控制器，选择主机模式，时钟系数为1
  I2CMSAUX=0x01;//使能自动发送
  I2CMSCR|=0x80;//使能I2C硬件中断
  EA=1;
  hardwareI2CSetClock(63);
}

void hardwareI2CSetClock(unsigned char clock){
  I2CCFG&=0xC0;
  I2CCFG|=clock;
}

void hardwareI2CStart(void){
  I2CBusy=1;
  I2CMSCR|=0x01;
  while(I2CBusy);
}

void hardwareI2CStop(void){
  I2CBusy=1;
  I2CMSCR|=0x06;
  while(I2CBusy);
}

bit hardwareI2CSendBeyt(unsigned char info){
  I2CBusy=1;
  I2CTXD=info;
  while(I2CBusy);
  if(I2CMSST&0x20!=0)
    return 0;
  else
    return 1;
}

unsigned char hardwareI2CReceiveBeyt(bit ACK){
  I2CBusy=1;
  I2CMSCR|=0x04;
  while(I2CBusy);
  if(ACK==1){
    I2CMSST&=~0x01;
  }else{
    I2CMSST|=0x01;
  }
  I2CBusy=1;
  I2CMSCR|=0x05;
  while(I2CBusy);
  return I2CRXD;
}
