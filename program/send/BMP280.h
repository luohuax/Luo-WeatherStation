#ifndef _BMP280H_
#define _BMP280H_
#include "STC8H.H"
#include "I2C.h"

typedef struct {
  unsigned int T1;
  int T2;
  int T3;
  unsigned int P1;
  int P2;
  int P3;
  int P4;
  int P5;
  int P6;
  int P7;
  int P8;
  int P9;
  long t_fine;
}BMP280TypeDef_Parameter;

typedef struct {
  long temperature;
  unsigned long pressure;
}BMP280TypeDef_Data;

void BMP280_Restart(void);
void BMP280_ReadParameter(void);
void BMP280_Init(void);
void BMP280_MeasureCmd(void);
void BMP280_ReadData(void);
long BMP280_compensate_T(long adc_T);
unsigned long BMP280_compensate_P(long adc_P);
BMP280TypeDef_Data* BMP280_GetData(void);
void BMP280_ChangeCompletedFlagSet(void);
bit BMP280_GetDataAvailableFlag(void);

#endif