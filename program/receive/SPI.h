#ifndef _HARDWARESPI_
#define _HARDWARESPI_
#include "STC8H.H"

sbit SS=P1^7;

void hardwareSPIInit(void);
unsigned char hardwareSPIExchangeData(unsigned char info);

#endif