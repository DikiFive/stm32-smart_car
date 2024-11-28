#ifndef __DK_H
#define __DK_H

/**
 * @file DK.h
 * @brief 该头文件包含了汽车初始化相关的函数声明，以及对其他模块的引用。
 *
 * 本文件主要用途是通过包含必要的头文件和声明初始化函数，为汽车控制系统提供接口。
 * 它引用了STM32F10x系列微控制器的头文件，以及自定义的延迟、按键、OLED显示和串口通信模块的头文件。
 */

#include "stm32f10x.h" // 包含STM32F10x系列微控制器的头文件
#include "stdio.h"     // 包含标准输入输出库的头文件
#include <stdarg.h>    // 包含可变参数头文件
#include "Delay.h"     // 包含延迟函数模块的头文件
#include "Key.h"       // 包含按键模块的头文件
#include "OLED.h"      // 包含OLED显示模块的头文件
#include "Serial.h"    // 包含串口通信模块的头文件
#include "PWM.h"       // 包含PWM模块的头文件
#include "Servo.h"     // 包含舵机模块的头文件
#include "Motor.h"     // 包含电机模块的头文件
#include "SR04.h"      // 包含超声波模块的头文件
#include "Timer.h"     // 包含定时器模块的头文件
#include "RED.h"       // 包含红外模块的头文件
#include "track.h"     // 包含循迹模块的头文件

void car_init(void);
uint64_t numlen(uint64_t num);

#endif