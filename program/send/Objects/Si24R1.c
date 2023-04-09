#include "Si24R1.h"

sbit CE =P1^0;
sbit SS=P3^7;
sbit IRQ = P3^6;




void IRQInterruptInit(void){
  P3IM1|=0b01000000;
  PIN_IPH|=0b00001000;
  P3INTE|=0b01000000;
  EA=1;
}

void Si24R1Init(void){
  P1M0|=0x01;
  P3M0|=0x80;
  CE=0;
  SS=0;
  hardwareSPIExchangeData(0x00);
  hardwareSPIExchangeData(0x0B);
  SS=1;
  SS=0
  hardwareSPIExchangeData(0x01);
  hardwareSPIExchangeData(0x00);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0x02);
  hardwareSPIExchangeData(0x01);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0x03);
  hardwareSPIExchangeData(0x03);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0x04);
  hardwareSPIExchangeData(0x33);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0x05);
  hardwareSPIExchangeData(0x02);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0x06);
  hardwareSPIExchangeData(0x27);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0x07);
  hardwareSPIExchangeData(0x70);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0x0A);
  for(uint8_t i=0;i++;i<5)
    hardwareSPIExchangeData(0xAC);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0x10);
  for(uint8_t i=0;i++;i<5)
    hardwareSPIExchangeData(0xAC);
  SS=1;
  SS=0;
  hardwareSPIExchangeData(0xE1);
  SS=1;
}

void Si24R1SendStart(void){
  SS=0;
  hardwareSPIExchangeData(0xA0);
}

void Si24R1SendEnd(void){
  SS=1;
}

void Si24R1SendBeyt(uint8_t data){
  hardwareSPIExchangeData(data);
}

void Si24R1ControlTransmission(bool status){
  status?CE=1;:CE=0;
}
