#include "SI24R1.h"

bit SI24R1_Data;

unsigned char buffer[10];

void INT2_Isr() interrupt 10{
  SI24R1_Data=1;
}

unsigned char SI24R1_WriteRegAddress_Write(unsigned char address){
  return hardwareSPIExchangeData(0x20|address);
}

unsigned char SI24R1_WriteRegAddress_Read(unsigned char address){
  return hardwareSPIExchangeData(address);
}

unsigned char SI24R1_WriteCmd(unsigned char info){
  return hardwareSPIExchangeData(info);
}

void SI24R1_WriteData(unsigned char info){
  hardwareSPIExchangeData(info);
}

unsigned char SI24R1_ReadData(void){
  return hardwareSPIExchangeData(0x00);
}

void SI24R1_Switch(bit status){
  SS=0;
  SI24R1_WriteRegAddress_Write(0x00);
  if(status)
    SI24R1_WriteData(0x0B);
  else
    SI24R1_WriteData(0x09);
  SS=1;
}


void SI24R1_Init(void){
  unsigned char i;
  CE=0;
  P_SW2|=0x80;
  P3M0&=0xBF;
  P3M1&=0xBF;//P3.6口准双向输出用于中断
  P1M0|=0x40;
  P1M1&=~0x40;//P1.6口推挽输出用于使能发射机
  for(i=0;i<255;i++);
  INTCLKO|=0x10;//开启INT2中断
  EA=1;//开启总中断
  CE=0;
  SS=0;
  SI24R1_WriteCmd(0xFF);//第一字节发送错误，空指令填充
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x00);
  SI24R1_WriteData(0x09);//使能所有中断，开启CRC，长度为1byte，接收模式
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x01);
  SI24R1_WriteData(0x03);//仅使能数据管道0，1自动确认
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x02);
  SI24R1_WriteData(0x03);//仅使能数据管道0，1
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x03);
  SI24R1_WriteData(0x03);//5bytes地址宽度
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x04);
  SI24R1_WriteData(0x35);//1ms，5次重发
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x05);
  SI24R1_WriteData(0x4B);//使用信道75（2475Mhz无线电频率）
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x06);
  SI24R1_WriteData(0x27);//250kbps带宽，7dBm发射功率
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x07);
  SI24R1_WriteData(0x70);//清除所有中断标志位
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x0A);
  for(i=0;i<5;i++)
    SI24R1_WriteData(0x8D);//数据管道0接收地址0x8D8D8D8D8D
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x0B);
  for(i=0;i<5;i++)
    SI24R1_WriteData(0x8C);//数据管道1接收地址0x8C8C8C8C8C
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x10);
  for(i=0;i<5;i++)
    SI24R1_WriteData(0x8D);//数据发射地址0x8D8D8D8D8D
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x12);
  SI24R1_WriteData(DATALENGTH);//指定数据长度
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x1C);
  SI24R1_WriteData(0x00);//关闭所有接收管道的动态负载长度
  SS=1;
  SS=0;
  SI24R1_WriteRegAddress_Write(0x1D);
  SI24R1_WriteData(0x00);//关闭动态负载长度，关闭ACK负载，关闭NOACK
  SS=1;
  SS=0;
  SI24R1_WriteCmd(0xE1);//清空TX FIFO
  SS=1;
  SS=0;
  SI24R1_WriteCmd(0xE2);//清空RX FIFO
  SS=1;
  CE=1;
  SI24R1_Data=0;
  SI24R1_Switch(1);
}

void SI24R1_ReadReceiveData(void){
  unsigned char i;
  CE=0;
  for(i=0;i<120;i++);
  SS=0;
  SI24R1_WriteRegAddress_Write(0x07);
  SI24R1_WriteData(0x70);
  SS=1;
  SS=0;
  SI24R1_WriteCmd(0x61);
  for(i=0;i<DATALENGTH;i++)
    buffer[i]=SI24R1_ReadData();
  SS=1;
  CE=1;
}

unsigned char* SI24R1_GetData(void){
  return buffer;
}

bit SI24R1_GetStatus(void){
  return SI24R1_Data;
}

void SI24R1_ClearFlag(void){
  SI24R1_Data=0;
}