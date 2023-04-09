#include "USART.h"

bit USART_busy;

char USART_SendBuffer[10];

void UsartIsr() interrupt 4{
  if(TI){
    TI=0;
    USART_busy=0;
  }
  if(RI){
    RI=0;
  }
}

void USART_Init(unsigned long baud){
  unsigned int i=(int)(65536-FOSC/4/baud);
  P_SW2|=0x80;//使能访问XFR
  P3M0&=~0x03;
  P3M1&=~0x03;//设置P3.1和P3.0为准双向GPIO

  AUXR|=0x04;//选择1T模式
  T2L=i;
  T2H=i>>8;//设置定时器重装载值

  SCON=0x50;//设置UART1为模式一，使能UART1接收
  AUXR|=0x01;//选择定时器二为波特率发生器
  ES=1;//开启中断
  AUXR|=0x10;//启动定时器二
  EA=1;
}

void USART_SendByte(char info){
  USART_busy=1;
  SBUF=info;
  while(USART_busy);
}

void USART_SendStr(char* ptr){
  while(*ptr!='\0'){
    USART_SendByte(*ptr);
    ptr++;
  }
}

void USART_SendNumberInWord(unsigned long number,unsigned char length){
  unsigned char i;
  for(i=0;i<length;i++){
    USART_SendBuffer[i]=number%10;
    number/=10;
  }
  for(i=length;i>0;i--)
    USART_SendByte('0'+USART_SendBuffer[i-1]);
}