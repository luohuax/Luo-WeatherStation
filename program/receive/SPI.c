#include "SPI.h"

bit SPIBusy;

void SPI_Isr() interrupt 9{
  SPSTAT=0xC0;
  SPIBusy=0;
}


void hardwareSPIInit(void){
  P_SW2|=0x80;
  P1M0&=0xC3;
  P1M1&=0x43;
  P1M0|=0x80;
  SPIBusy=0;
  SS=1;
  SPCTL=0x50;
  SPSTAT=0xC0;
  IE2|=0x02;
  EA=1;
}

unsigned char hardwareSPIExchangeData(unsigned char info){
  SPIBusy =1;
  SPDAT=info;
  while(SPIBusy);
  return SPDAT;
}


