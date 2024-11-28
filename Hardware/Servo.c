#include "stm32f10x.h" // Device header
#include "PWM.h"

void Servo_Init(void)
{
    PWM_TIM3_Init();
}

void Servo_SetAngle(float Angle)
{
    PWM_TIM3_SetCompare4(Angle / 180 * 2000 + 500);
}
