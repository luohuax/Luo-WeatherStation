#include "STC8H.H"
#include "SPI.h"
#include "USART.h"
#include "SI24R1.h"

void SystemDataProcess(void);

unsigned long delay;
unsigned char* bufferPtr;
char SendBuffer[27]={0};

struct{
  int temperature;
  unsigned long pressure;
  unsigned int humidity;
  unsigned int Voltage;
}SystemData;



void main(void){
  USART_Init(115200);
  hardwareSPIInit();
  for(delay=0;delay<65535;delay++);
  SI24R1_Init();
  bufferPtr=SI24R1_GetData();
  while(1){
    if(SI24R1_GetStatus()){
      SI24R1_ReadReceiveData();
      SystemDataProcess();
      USART_SendStr(SendBuffer);
      SI24R1_ClearFlag();
    }
  }
}

void SystemDataProcess(void){
  unsigned char i;
  for(i=2;i>0;i--){
    SystemData.temperature<<=8;
    SystemData.temperature|=*(bufferPtr+i-1);
  }
  for(i=4;i>0;i--){
    SystemData.pressure<<=8;
    SystemData.pressure|=*(bufferPtr+2+i-1);
  }
  for(i=2;i>0;i--){
    SystemData.humidity<<=8;
    SystemData.humidity|=*(bufferPtr+6+i-1);
  }
  for(i=2;i>0;i--){
    SystemData.Voltage<<=8;
    SystemData.Voltage|=*(bufferPtr+8+i-1);
  }
  SendBuffer[4]='0'+SystemData.temperature%10;
  SystemData.temperature/=10;
  SendBuffer[3]='0'+SystemData.temperature%10;
  SystemData.temperature/=10;
  SendBuffer[2]='.';
  SendBuffer[1]='0'+SystemData.temperature%10;
  SystemData.temperature/=10;
  SendBuffer[0]='0'+SystemData.temperature%10;
  SendBuffer[5]=',';

  SendBuffer[11]='0'+SystemData.pressure%10;
  SystemData.pressure/=10;
  SendBuffer[10]='0'+SystemData.pressure%10;
  SystemData.pressure/=10;
  SendBuffer[9]='0'+SystemData.pressure%10;
  SystemData.pressure/=10;
  SendBuffer[8]='.';
  SendBuffer[7]='0'+SystemData.pressure%10;
  SystemData.pressure/=10;
  SendBuffer[6]='0'+SystemData.pressure%10;
  SendBuffer[12]=',';

  SendBuffer[17]='0'+SystemData.humidity%10;
  SystemData.humidity/=10;
  SendBuffer[16]='.';
  SendBuffer[15]='0'+SystemData.humidity%10;
  SystemData.humidity/=10;
  SendBuffer[14]='0'+SystemData.humidity%10;
  SystemData.humidity/=10;
  SendBuffer[13]='0'+SystemData.humidity%10;
  SendBuffer[18]=',';

  SendBuffer[23]='0'+SystemData.Voltage%10;
  SystemData.Voltage/=10;
  SendBuffer[22]='0'+SystemData.Voltage%10;
  SystemData.Voltage/=10;
  SendBuffer[21]='.';
  SendBuffer[20]='0'+SystemData.Voltage%10;
  SystemData.Voltage/=10;
  SendBuffer[19]='0'+SystemData.Voltage%10;
  SendBuffer[24]='\r';
  SendBuffer[25]='\n';
  SendBuffer[26]='\0';
}
