#ifndef _HARDWEARADC_
#define _HARDWEARADC_
#include "STC8H.H"

#define VREF 1190

void ADC_Init(void);
void ADC_Switch(bit status);
void ADC_Enable(void);
void ADC_DataCalculate(void);
unsigned int ADC_GetVoltage(void);
bit ADC_GetDataAvailableFlag(void);
#endif