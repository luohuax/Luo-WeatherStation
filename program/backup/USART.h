#ifndef _USARTH_
#define _USARTH_
#include "STC8H.H"

#define FOSC 35000000UL

void USART_Init(unsigned long baud);
void USART_SendByte(char info);
void USART_SendStr(char* ptr);

#endif