#ifndef __RED_H
#define __RED_H

#include "dk.h"

#define INFRARED_L_PIN GPIO_Pin_11
#define INFRARED_R_PIN GPIO_Pin_10

// 定义一个结构体，用于存储左右两个红外避障传感器的状态
typedef struct {
    uint8_t left;
    uint8_t right;
} InfraredStatus;

void INFRARED_Init(void);
InfraredStatus INFRARED_Get(void);

#endif
