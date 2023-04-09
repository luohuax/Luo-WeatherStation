#include "AHT20.h"

AHT20TypeDef_Data AHT20_Environmentinfo;

bit AHT20_ChangeCompleted,AHT20_DataAvailable;

void resetReg(unsigned char addr){
  unsigned char buffer[2]={0,0};
  hardwareI2CStart();
  hardwareI2CSendBeyt(0x70);
  hardwareI2CSendBeyt(addr);
  hardwareI2CSendBeyt(0x00);
  hardwareI2CSendBeyt(0x00);
  hardwareI2CStop();
  hardwareI2CStart();
  hardwareI2CSendBeyt(0x71);
  hardwareI2CReceiveBeyt(1);
  buffer[0]=hardwareI2CReceiveBeyt(1);
  buffer[1]=hardwareI2CReceiveBeyt(1);
  hardwareI2CStop();
  hardwareI2CStart();
  hardwareI2CSendBeyt(0x70);
  hardwareI2CSendBeyt(0xB0|addr);
  hardwareI2CSendBeyt(buffer[0]);
  hardwareI2CSendBeyt(buffer[1]);
  hardwareI2CStop();
}

void regInit(void){
  resetReg(0x1B);
  resetReg(0x1C);
  resetReg(0x1E);
}

void AHT20_Init(void){
  hardwareI2CStart();
  hardwareI2CSendBeyt(0x70);
  hardwareI2CSendBeyt(0xA8);
  hardwareI2CSendBeyt(0x00);
  hardwareI2CSendBeyt(0x00);
  hardwareI2CStop();
  hardwareI2CStart();
  hardwareI2CSendBeyt(0x70);
  hardwareI2CSendBeyt(0xBE);
  hardwareI2CSendBeyt(0x08);
  hardwareI2CSendBeyt(0x00);
  hardwareI2CStop();
  hardwareI2CStart();
  hardwareI2CSendBeyt(0x71);
  if((hardwareI2CReceiveBeyt(0)&0x18)!=0x18){
    hardwareI2CStop();
    regInit();
  }else
    hardwareI2CStop();
}

void AHT20_SendMeasureCmd(void){
  hardwareI2CStart();
  hardwareI2CSendBeyt(0x70);
  hardwareI2CSendBeyt(0xAC);
  hardwareI2CSendBeyt(0x33);
  hardwareI2CSendBeyt(0x00);
  hardwareI2CStop();
}

unsigned char CRC8Cheak(unsigned char* massage,unsigned char len){
  unsigned char i,j;
  unsigned char CRC=0xFF;
  for(j=0;j<len;j++){
    CRC^=(massage[j]);
    for(i=0;i<8;i++){
      if(CRC&0x80)
        CRC=(CRC<<1)^CRC8CODE;
      else
        CRC<<=1;
    }
  }
  return CRC;
}

void AHT20_ReadData(void){
  if(AHT20_ChangeCompleted){
  unsigned long temperature=0,humidity=0;
  unsigned char buffer[6];
  unsigned char CRC;
  unsigned char i;
  hardwareI2CStart();
  hardwareI2CSendBeyt(0x71);
  buffer[0]=hardwareI2CReceiveBeyt(1);
  if(buffer[0]&0x80!=0){
    AHT20_Environmentinfo.status=1;
    AHT20_Environmentinfo.humidity=0;
    AHT20_Environmentinfo.temperature=0;
    hardwareI2CStop();
    return;
  }
  for(i=0;i<5;i++)
    buffer[i+1]=hardwareI2CReceiveBeyt(1);
  CRC=hardwareI2CReceiveBeyt(0);
  hardwareI2CStop();
  AHT20_SendMeasureCmd();
  if(CRC8Cheak(buffer,6)!=CRC){
    AHT20_Environmentinfo.status=2;
    AHT20_Environmentinfo.humidity=0;
    AHT20_Environmentinfo.temperature=0;
    return;
  }
  AHT20_Environmentinfo.status=0;
  humidity|=buffer[1];
  humidity<<=8;
  humidity|=buffer[2];
  humidity<<=8;
  humidity|=buffer[3];
  humidity>>=4;
  temperature|=(buffer[3]&0x0F);
  temperature<<=8;
  temperature|=buffer[4];
  temperature<<=8;
  temperature|=buffer[5];
  AHT20_Environmentinfo.humidity=humidity*1000/1048576;
  AHT20_Environmentinfo.temperature=(float)temperature*100*200/1048576-5000;
  AHT20_ChangeCompleted=0;
  AHT20_DataAvailable=1;
  }
}

bit AHT20_GetDataAvailableFlag(void){
  return AHT20_DataAvailable;
}

void AHT20_ChangeCompletedFlagSet(void){
  AHT20_ChangeCompleted=1;
}

AHT20TypeDef_Data* AHT20_GetData(void){
  AHT20_DataAvailable=0;
  return &AHT20_Environmentinfo;
}

