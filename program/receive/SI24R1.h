
#ifndef _SI24R1H_
#define _SI24R1H_
#include "STC8H.H"
#include "SPI.h"
#include "USART.h"

#define DATALENGTH 10

sbit CE=P1^6;


void SI24R1_Init(void);
unsigned char SI24R1_WriteRegAddress_Write(unsigned char address);
unsigned char SI24R1_WriteRegAddress_Read(unsigned char address);
unsigned char SI24R1_WriteCmd(unsigned char info);
void SI24R1_WriteData(unsigned char info);
unsigned char SI24R1_ReadData(void);
void SI24R1_Switch(bit status);
void SI24R1_ReadReceiveData(void);
bit SI24R1_GetStatus(void);
unsigned char* SI24R1_GetData(void);
void SI24R1_ClearFlag(void);

#endif