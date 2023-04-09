/*
 * @Author: Luohua luohuax233@gmail.com
 * @Date: 2023-03-19 10:09:54
 * @LastEditors: Luohua luohuax233@gmail.com
 * @LastEditTime: 2023-03-21 23:04:43
 * @FilePath: \program\ImitateI2C.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "lmitateI2C.h"
#include "intrins.h"

void lmitateI2CInit(void){
  P1M0&=0x3F;
  P1M1&=0x3F;
  SCL=1;
  SDA=1;
}

void lmitateI2CStart(void){
  SCL=1;
  SDA=1;
  SDA=0;
  SCL=0;
}

void lmitateI2CStop(void){
  SCL=0;
  SDA=0;
  SCL=1;
  SDA=1;
}

bit lmitateI2CSendBeyt(unsigned char info){
  unsigned char i;
  bit ACK;
  for(i=0;i<8;i++){
    SCL=0;
    if(0x80&info)
      SDA=1;
    else
      SDA=0;
    info<<=1;
    SCL=1;
  }
  SCL=0;
  SDA=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  SCL=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  ACK=SDA;
  SCL=0;
  return ~ACK;
}

unsigned char lmitateI2CReceiveBeyt(bit ACK){
  unsigned char i;
  unsigned char info=0x00;
  SDA=1;
  for(i=0;i<8;i++){
    info<<=1;
    SCL=1;
    if(SDA)
      info|=0x01;
    SCL=0;
  }
  SDA=~ACK;
  SCL=1;
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  SCL=0;
  SDA=1;
  return info;
}



