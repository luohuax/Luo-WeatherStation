#ifndef _AHT20H_
#define _AHT20H_
#include "STC8H.H"
#include "I2C.h"

#define CRC8CODE 0x31

typedef struct {
  int temperature;
  unsigned int humidity;
  unsigned char status;
}AHT20TypeDef_Data;

void resetReg(unsigned char addr);
void regInit(void);

void AHT20_Init(void);
unsigned char CRC8Cheak(unsigned char* massage,unsigned char len);
void AHT20_SendMeasureCmd(void);
void AHT20_ReadData(void);
bit AHT20_GetDataAvailableFlag(void);
void AHT20_ChangeCompletedFlagSet(void);
AHT20TypeDef_Data* AHT20_GetData(void);

#endif