#ifndef _SI24R1H_
#define _SI24R1H_
#include "SPI.h"
#include"STC8H.H"

void Si24R1Init(void);
void Si24R1SendStart(void);
void Si24R1SendEnd(void);
void Si24R1SendBeyt(uint8_t data);
void Si24R1ControlTransmission(bool status);
void IRQInterruptInit(void);
#endif