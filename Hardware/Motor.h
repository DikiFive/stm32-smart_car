#ifndef __MOTOR_H
#define __MOTOR_H

#include "dk.h"

// 引脚配置
#define motor_power_pin   GPIO_Pin_5
#define left_motor_pin_1  GPIO_Pin_6
#define left_motor_pin_2  GPIO_Pin_7
#define right_motor_pin_1 GPIO_Pin_8
#define right_motor_pin_2 GPIO_Pin_9

void Motor_Init(void);
uint8_t left_control(uint8_t state);
uint8_t right_control(uint8_t state);
void car_cicle(void);
void car_stop(void);
void car_left(void);
void car_right(void);
void car_back(void);
void car_go(void);

#endif
