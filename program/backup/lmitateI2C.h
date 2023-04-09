/*
 * @Author: Luohua luohuax233@gmail.com
 * @Date: 2023-03-19 10:10:58
 * @LastEditors: Luohua luohuax233@gmail.com
 * @LastEditTime: 2023-03-20 23:31:28
 * @FilePath: \program\lmitateI2C.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _LMITATEI2C_
#define _LMITATEI2C_
#include "STC8H.H"
sbit SCL =P1^6;
sbit SDA= P1^7;

void lmitateI2CInit(void);
void lmitateI2CStart(void);
void lmitateI2CStop(void);
bit lmitateI2CSendBeyt(unsigned char info);
unsigned char lmitateI2CReceiveBeyt(bit ACK);
#endif