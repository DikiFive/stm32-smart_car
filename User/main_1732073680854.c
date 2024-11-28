#include "stm32f10x.h" // 设备头文件
#include "servo.h"     // 假设舵机控制函数在servo.h中定义

// 定义舵机角度范围
#define MIN_ANGLE 30
#define MAX_ANGLE 150

// 定义步进角度和延迟时间
#define STEP_ANGLE 1
#define DELAY_TIME 20 // 延迟时间，单位为毫秒

int main(void)
{
    // 初始化系统时钟、GPIO等
    SystemInit();
    // 初始化舵机
    Servo_Init();

    int angle = MIN_ANGLE;
    int direction = 1; // 1表示增加角度，-1表示减少角度

    while (1)
    {
        // 设置舵机角度
        Servo_SetAngle(angle);

        // 更新角度
        angle += direction * STEP_ANGLE;

        // 判断是否达到角度边界并改变方向
        if (angle >= MAX_ANGLE)
        {
            angle = MAX_ANGLE;
            direction = -1;
        }
        else if (angle <= MIN_ANGLE)
        {
            angle = MIN_ANGLE;
            direction = 1;
        }

        // 延迟一段时间
        Delay(DELAY_TIME);
    }
}

// 延迟函数，假设已经实现
void Delay(uint32_t ms)
{
    // 假设使用SysTick定时器实现延迟
    SysTick->LOAD = SystemCoreClock / 1000 * ms;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}