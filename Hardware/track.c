#include "track.h"

/**
 * @brief 初始化循迹传感器的GPIO配置
 *
 * 本函数负责初始化与循迹传感器相关的GPIO引脚，使其处于正确的输入模式和速度设置，
 * 以确保循迹传感器能够正常工作。这些初始化包括使能相应的时钟，以及设置GPIO的模式、引脚和速度。
 */
void track_init(void)
{
    // 使能GPIOC和GPIOB的时钟，确保它们可以正常工作
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 定义一个GPIO初始化结构体，用于配置GPIO的属性
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置GPIOB的引脚13、14、15为浮空输入模式，速度为50MHz
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = TKIN1 | TKIN2 | TKIN3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置GPIOA的引脚12为浮空输入模式，速度为50MHz
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = TKIN4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief 获取轨迹传感器的状态
 *
 * 本函数通过读取四个输入端口(IN1, IN2, IN3, IN4)的电平状态来获取轨迹传感器的状态，
 * 这些输入端口连接到不同的GPIO端口。读取的结果被封装在一个track_state结构体中，
 * 以便于后续处理和判断。
 *
 * @return track_state 包含四个输入端口状态的结构体
 */
track_state track_get(void)
{
    track_state state;

    // 读取IN1端口的状态
    state.IN1 = GPIO_ReadInputDataBit(GPIOB, TKIN1);
    // 读取IN2端口的状态
    state.IN2 = GPIO_ReadInputDataBit(GPIOB, TKIN2);
    // 读取IN3端口的状态
    state.IN3 = GPIO_ReadInputDataBit(GPIOB, TKIN3);
    // 读取IN4端口的状态
    state.IN4 = GPIO_ReadInputDataBit(GPIOA, TKIN4);

    return state;
}