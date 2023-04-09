#include "ADC.h"
#include "Intrins.h"

unsigned int VRefMeasurementValue;
unsigned int Voltage;

bit ADC_ChangeCompleted,ADC_DataAvailable;

void ADC_Isr() interrupt 5{
  ADC_CONTR&=~0x20;
  ADC_ChangeCompleted=1;
}

void ADC_Init(void){ //模拟信号-数字信号转换器（ADC）初始化函数
  unsigned int i;
  ADC_ChangeCompleted=ADC_DataAvailable=0;
  P_SW2|=0x80;//使能访问XFR
  P1M0&=~0x02;
  P1M1|=0x02;//设置P1.1 GPIO口为高阻态输入
  ADC_CONTR=0x0F;//关闭ADC，清零ADC转换完成标志位，关闭PWM触发ADC转换，使能ADC15通道
  ADCCFG=0x2F;//设置转换结果右对齐，设置ADC时钟1.0938MHz
  ADCTIM=0x3F;//设置ADC准备时间1时钟周期，保持时间2时钟周期，采样时间15时钟周期
  //ADCEXCFG=0x05;//关闭ETR上升沿触发ADC，设置结果为4次ADC转换平均值
  ADC_Switch(1);
  for(i=0;i<40000;i++);
  ADC_Enable();
  _nop_();
  _nop_();
  while(!(ADC_CONTR&0x20));
  VRefMeasurementValue|=ADC_RES;
  VRefMeasurementValue<<=8;
  VRefMeasurementValue|=ADC_RESL;
  EADC=1;
  EA=1;
}

void ADC_Enable(void){
  ADC_CONTR|=0x40;
}

void ADC_Switch(bit status){
  if(status)
    ADC_CONTR|=0x80;
  else
    ADC_CONTR&=~0x80;
}

void ADC_DataCalculate(void){
  if(ADC_ChangeCompleted){
    unsigned int i;
    i|=ADC_RES;
    i<<=8;
    i|=ADC_RESL;
    Voltage=(unsigned long)i*(unsigned long)VREF/VRefMeasurementValue;
    ADC_ChangeCompleted=0;
    ADC_DataAvailable=1;
  }
}

unsigned int ADC_GetVoltage(void){
  ADC_DataAvailable=0;
  return Voltage;
}

bit ADC_GetDataAvailableFlag(void){
  return ADC_DataAvailable;
}