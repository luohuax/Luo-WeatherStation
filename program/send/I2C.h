/*
 * @Author: Luohua luohuax233@gmail.com
 * @Date: 2023-03-19 10:11:10
 * @LastEditors: Luohua luohuax233@gmail.com
 * @LastEditTime: 2023-03-26 14:34:29
 * @FilePath: \program\I2C.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _STC8HI2C_
#define _STC8HI2C_
#include "STC8H.H"
void hardwareI2CInit(void);
void hardwareI2CSetClock(unsigned char clock);
void hardwareI2CStart(void);
void hardwareI2CStop(void);
bit hardwareI2CSendBeyt(unsigned char info);
unsigned char hardwareI2CReceiveBeyt(bit ACK);
#endif