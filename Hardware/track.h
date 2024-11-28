#ifndef __TRACK_H
#define __TRACK_H

#include "dk.h"

#define TKIN1 GPIO_Pin_13
#define TKIN2 GPIO_Pin_14
#define TKIN3 GPIO_Pin_15
#define TKIN4 GPIO_Pin_12

// 定义一个结构体，用于存储左右两个红外避障传感器的状态
typedef struct {
    uint8_t IN1;
    uint8_t IN2;
    uint8_t IN3;
    uint8_t IN4;
} track_state;

void track_init(void);
track_state track_get(void);

#endif
