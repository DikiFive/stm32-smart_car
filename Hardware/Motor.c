#include "Motor.h" // Device header

#define stop 1
#define down 2
#define up   3

// 初始化电机的GPIO接口
// 初始化电机控制引脚
void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_Initstruct; // 定义GPIO结构体
    // 时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // 配置gpio结构体
    GPIO_Initstruct.GPIO_Pin   = motor_power_pin | left_motor_pin_1 | left_motor_pin_2 | right_motor_pin_1 | right_motor_pin_2;
    GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz; // 设置工作频率
    GPIO_Initstruct.GPIO_Mode  = GPIO_Mode_Out_PP; // 设置为 推挽输出
    // 初始化引脚
    GPIO_Init(GPIOB, &GPIO_Initstruct);

    // 5号引脚高电平
    GPIO_SetBits(GPIOB, motor_power_pin);
}

// 控制左电机
// 参数state: 控制电机的状态
uint8_t left_control(uint8_t state)
{
    // 根据不同的状态控制电机
    switch (state) {
        case stop:
            // 6号引脚低电平
            GPIO_ResetBits(GPIOB, left_motor_pin_1);
            // 7号引脚低电平
            GPIO_ResetBits(GPIOB, left_motor_pin_2);
            break;
        case down:
            // 6号引脚低电平
            GPIO_ResetBits(GPIOB, left_motor_pin_1);
            // 7号引脚高电平
            GPIO_SetBits(GPIOB, left_motor_pin_2);

            break;
        case up:
            // 6号引脚高电平
            GPIO_SetBits(GPIOB, left_motor_pin_1);
            // 7号引脚低电平
            GPIO_ResetBits(GPIOB, left_motor_pin_2);

            break;
    }
}

// 控制右电机
// 参数state: 控制电机的状态
uint8_t right_control(uint8_t state)
{
    // 根据不同的状态控制电机
    switch (state) {
        case stop:
            // 9号引脚低电平
            GPIO_ResetBits(GPIOB, right_motor_pin_2);
            // 8号引脚低电平
            GPIO_ResetBits(GPIOB, right_motor_pin_1);
            break;
        case down:
            // 9号引脚高电平
            GPIO_SetBits(GPIOB, right_motor_pin_2);
            // 8号引脚低电平
            GPIO_ResetBits(GPIOB, right_motor_pin_1);
            break;
        case up:
            // 9号引脚高电平
            GPIO_ResetBits(GPIOB, right_motor_pin_2);
            // 8号引脚低电平
            GPIO_SetBits(GPIOB, right_motor_pin_1);
            break;
    }
}

void car_cicle(void)
{
    // 右旋转
    left_control(up);
    right_control(down);
}

void car_go(void)
{
    left_control(up);
    right_control(up);
}

void car_back(void)
{
    left_control(down);
    right_control(down);
}

void car_left(void)
{
    left_control(down);
    right_control(up);
}

void car_right(void)
{
    left_control(up);
    right_control(down);
}

void car_stop(void)
{
    left_control(stop);
    right_control(stop);
}
