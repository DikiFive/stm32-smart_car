#include "dk.h"

/**
 * @brief 初始化汽车相关的硬件和系统
 *
 * 本函数在汽车系统启动时进行必要的初始化工作
 * 包括按键、LED、OLED屏和串口的初始化
 * 以确保汽车系统能够正常工作
 */
void car_init(void)
{
    Motor_Init();
    Serial_Init();
    Timer4_Init();  // 超声波计时中断
    HC_SR04_Init(); // 初始化超声波测距模块
    INFRARED_Init();
}

uint64_t numlen(uint64_t num) // 计算数字的长度
{
    uint64_t len = 1;      // 初始长度为1
    for (; num > 9; ++len) // 判断num是否大于9，否则长度+1
        num /= 10;         // 使用除法进行运算，直到num小于1
    return len;            // 返回长度的值
}
