#ifndef __PWM_H
#define __PWM_H

void PWM_TIM2_Init(void);
void PWM_TIM3_Init(void);
void PWM_TIM2_SetCompare2(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);
void PWM_TIM3_SetCompare4(uint16_t Compare);

#endif
