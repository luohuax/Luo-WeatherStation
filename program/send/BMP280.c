#include "BMP280.h"

BMP280TypeDef_Parameter BMP280;
BMP280TypeDef_Data BMP280_Environmentinfo;

bit BMP280_ChangeCompleted,BMP280_DataAvailable;

void BMP280_Restart(void) { // 传感器软重启函数
    hardwareI2CStart();
    hardwareI2CSendBeyt(0xEC); // 地址为0x76，最低位加一个0后变为0xEC，即为地址为0x76的器件，R/W选为写，后不再赘述
    hardwareI2CSendBeyt(0xE0);
    hardwareI2CSendBeyt(0xB6); // 0xE0寄存器写0xB6，软重启。
    hardwareI2CStop();
}

void BMP280_ReadParameter(void) // 修正参数读取函数。true表示向传感器发送ACK标志位继续传输数据
{
    hardwareI2CStart();
    hardwareI2CSendBeyt(0xEC);
    hardwareI2CSendBeyt(0x88); // 选定0x88寄存器，开始读取修正参数
    hardwareI2CStart();
    hardwareI2CSendBeyt(0xED); // 0x76地址加上1的最低位，R/W选为读
    BMP280.T1 = hardwareI2CReceiveBeyt(1);
    BMP280.T1|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.T2 = hardwareI2CReceiveBeyt(1);
    BMP280.T2|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.T3 = hardwareI2CReceiveBeyt(1);
    BMP280.T3|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P1 = hardwareI2CReceiveBeyt(1);
    BMP280.P1|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P2 = hardwareI2CReceiveBeyt(1);
    BMP280.P2|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P3 = hardwareI2CReceiveBeyt(1);
    BMP280.P3|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P4 = hardwareI2CReceiveBeyt(1);
    BMP280.P4|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P5 = hardwareI2CReceiveBeyt(1);
    BMP280.P5|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P6 = hardwareI2CReceiveBeyt(1);
    BMP280.P6|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P7 = hardwareI2CReceiveBeyt(1);
    BMP280.P7|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P8 = hardwareI2CReceiveBeyt(1);
    BMP280.P8|=((unsigned long )hardwareI2CReceiveBeyt(1)<<8);
    BMP280.P9 = hardwareI2CReceiveBeyt(1);
    BMP280.P9|=((unsigned long )hardwareI2CReceiveBeyt(0)<<8);
    hardwareI2CStop();
}

void BMP280_Init(void) // BMP280初始化函数
{
    unsigned int counter;
    BMP280_Restart();
    for(counter=0;counter<65535;counter++);
    hardwareI2CStart();
    hardwareI2CSendBeyt(0xEC);
    hardwareI2CSendBeyt(0xF4); // 寄存器地址
    hardwareI2CSendBeyt(0x55); // 0b0101 0101选择16倍气压过采样，2倍温度过采样，force mode
    hardwareI2CSendBeyt(0xF5); // 寄存器地址
    hardwareI2CSendBeyt(0x10); // 0b0001 0000由于使用force mode时间不关心，全填0，滤波器使用16倍，SPI模式不关心，写0
    hardwareI2CStop();
    BMP280_ReadParameter();
}

void BMP280_MeasureCmd(void) // 在mode上写0b01使能传感器又一次测量
{
    hardwareI2CStart();
    hardwareI2CSendBeyt(0xEC);
    hardwareI2CSendBeyt(0xF4);
    hardwareI2CSendBeyt(0x56);
    hardwareI2CStop();
}

void BMP280_ReadData(void) { 
    if(BMP280_ChangeCompleted){
    long T,p;
    hardwareI2CStart();
    hardwareI2CSendBeyt(0xEC);
    hardwareI2CSendBeyt(0xF7);
    hardwareI2CStart();
    hardwareI2CSendBeyt(0xED);
    p = (long)hardwareI2CReceiveBeyt(1);
    p <<= 8;
    p |= (long)hardwareI2CReceiveBeyt(1);
    p <<= 4;
    p |= (long)(hardwareI2CReceiveBeyt(1) >> 4);
    T = (long)hardwareI2CReceiveBeyt(1);
    T <<= 8;
    T |= (long)hardwareI2CReceiveBeyt(1);
    T <<= 4;
    T |= (long)(hardwareI2CReceiveBeyt(0) >> 4);
    hardwareI2CStop();
    BMP280_MeasureCmd();
    BMP280_Environmentinfo.temperature=BMP280_compensate_T(T);
    BMP280_Environmentinfo.pressure=BMP280_compensate_P(p);
    BMP280_ChangeCompleted=0;
    BMP280_DataAvailable=1;
    }
}

long BMP280_compensate_T(long adc_T) {
    long var1, var2, T;
    var1 = ((((adc_T >> 3) - ((long)BMP280.T1 << 1))) * ((long)BMP280.T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((long)BMP280.T1)) * ((adc_T >> 4) - ((long)BMP280.T1))) >> 12) * ((long)BMP280.T3)) >> 14;
    BMP280.t_fine = var1 + var2;
    T = (BMP280.t_fine * 5 + 128) >> 8;
    return T;
}

unsigned long BMP280_compensate_P(long adc_P) {
    long var1, var2;
    unsigned long p;
    var1 = (((long)BMP280.t_fine) >> 1) - (long)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((long)BMP280.P6);
    var2 = var2 + ((var1 * ((long)BMP280.P5)) << 1);
    var2 = (var2 >> 2) + (((long)BMP280.P4) << 16);
    var1 = (((BMP280.P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((long)BMP280.P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((long)BMP280.P1)) >> 15);
    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = (((unsigned long)(((long)1048576) - adc_P) - (var2 >> 12))) * 3125;
    if (p < 0x80000000) {
        p = (p << 1) / ((unsigned long)var1);
    } else {
        p = (p / (unsigned long)var1) * 2;
    }
    var1 = (((long)BMP280.P9) * ((long)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((long)(p >> 2)) * ((long)BMP280.P8)) >> 13;
    p = (unsigned long)((long)p + ((var1 + var2 + BMP280.P7) >> 4));
    return p;
}

bit BMP280_GetDataAvailableFlag(void){
    return BMP280_DataAvailable;
}

void BMP280_ChangeCompletedFlagSet(void){
    BMP280_ChangeCompleted=1;
}

BMP280TypeDef_Data* BMP280_GetData(void){
    BMP280_DataAvailable=0;
    return &BMP280_Environmentinfo;
}