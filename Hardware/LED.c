#include "stm32f10x.h"

/**
 * 初始化LED相关的GPIO引脚
 * 该函数配置了GPIOA的第1和第2引脚为推挽输出模式，用于驱动LED
 * 并且设置了这些引脚的最大输出速度为50MHz
 * 最后，该函数将这些引脚设置为高电平，以确保LED在初始化时处于关闭状态
 */
void LED_Init(void)
{
    // 启用GPIOA的时钟，这是配置GPIO引脚的前提
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 创建一个GPIO初始化结构体实例，用于配置GPIO引脚
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置GPIO引脚为推挽输出模式、指定引脚、设置最大输出速度
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;        // 推挽输出模式
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2; // 指定配置的引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // 最大输出速度
    // 应用上述配置到GPIOA
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将配置的GPIO引脚设置为高电平，以确保LED在初始化时处于关闭状态
    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

// 控制LED1开启，通过将GPIOA的第1引脚设置为低电平
void LED1_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

// 控制LED1关闭，通过将GPIOA的第1引脚设置为高电平
void LED1_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

// 控制LED1翻转，根据当前状态决定是开启还是关闭
void LED1_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0) {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
    } else {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    }
}

// 控制LED2开启，通过将GPIOA的第2引脚设置为低电平
void LED2_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

// 控制LED2关闭，通过将GPIOA的第2引脚设置为高电平
void LED2_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

// 控制LED2翻转，根据当前状态决定是开启还是关闭
void LED2_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0) {
        GPIO_SetBits(GPIOA, GPIO_Pin_2);
    } else {
        GPIO_ResetBits(GPIOA, GPIO_Pin_2);
    }
}