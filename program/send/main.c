#include "STC8H.H"
#include "I2C.h"
#include "SPI.h"
#include "SI24R1.h"
#include "ADC.h"
#include "BMP280.h"
#include "AHT20.h"
#include "USART.h"

void SystemClockInit(void);//使用Timer0作为系统滴答计时器，每1ms触发一次中断
void SystemDataProcess(void);

unsigned char buffer[DATALENGTH];
unsigned char DataAvailableFlag=0x00;

struct{
  int temperature;
  unsigned long pressure;
  unsigned int humidity;
  unsigned int Voltage;
}SystemData;

void main(void){
  unsigned int i;
  hardwareI2CInit();
  hardwareSPIInit();
  USART_Init(115200);
  for(i=0;i<65535;i++);
  BMP280_Init();
  AHT20_Init();
  SI24R1_Init();
  ADC_Init();//所有总线及外设初始化
  SystemClockInit();//初始化系统滴答计时器
  while(1){
    BMP280_ReadData();
    AHT20_ReadData();
    ADC_DataCalculate();
    SystemDataProcess();
  }
}

void SystemClockInit(void){
  TMOD=0x03;//设置计数器0脉冲源为系统时钟，设置计数器0不受外部中断影响，设置计数器0为不可屏蔽的16位自动重载模式
  AUXR|=0x80;//设置计数器0的脉冲源为系统时钟且不分频
  INTCLKO&=~0x01;//关闭Timer0的时钟输出
  TH0=0x77;
  TL0=0x48;//计数器溢出时间为1ms
  ET0=1;//开启Timer0的硬件中断
  TR0=1;//启动Timer0
}

void TM0_Isr() interrupt 1{
  static unsigned int count=0;
  count++;
  if(count>=1000){
    BMP280_ChangeCompletedFlagSet();
    AHT20_ChangeCompletedFlagSet();
    ADC_Enable();
    count=0;
  }
}

void SystemDataProcess(void){
  if(BMP280_GetDataAvailableFlag()){
      unsigned char i;
      SystemData.pressure=BMP280_GetData()->pressure;
      USART_SendStr("BMP280:");
      USART_SendNumberInWord(SystemData.pressure,10);
      USART_SendByte('\r');
      USART_SendByte('\n');
      for(i=0;i<4;i++){
        buffer[2+i]=SystemData.pressure;
        SystemData.pressure>>=8;
      }
      DataAvailableFlag|=0x01;
    }
    if(AHT20_GetDataAvailableFlag()){
      unsigned char j;
      AHT20TypeDef_Data* i;
      i=AHT20_GetData();
      SystemData.temperature=i->temperature;
      SystemData.humidity=i->humidity;
      USART_SendStr("AHT20:");
      USART_SendNumberInWord(SystemData.temperature,6);
      USART_SendByte(' ');
      USART_SendNumberInWord(SystemData.humidity,6);
      USART_SendByte('\r');
      USART_SendByte('\n');
      for(j=0;j<2;j++){
        buffer[j]=SystemData.temperature;
        SystemData.temperature>>=8;
      }
      for(j=0;j<2;j++){
        buffer[6+j]=SystemData.humidity;
        SystemData.humidity>>=8;
      }
      DataAvailableFlag|=0x02;
    }
    if(ADC_GetDataAvailableFlag()){
      unsigned char i;
      SystemData.Voltage=ADC_GetVoltage();
      for(i=0;i<2;i++){
        buffer[8+i]=SystemData.Voltage;
        SystemData.Voltage>>=8;
      }
      DataAvailableFlag|=0x04;
    }
    if(!SI24R1_GetStatus())
      DataAvailableFlag|=0x08;
    if(DataAvailableFlag==0x0F){
      SI24R1_SendData(buffer,DATALENGTH);
      DataAvailableFlag=0x00;
    }
}