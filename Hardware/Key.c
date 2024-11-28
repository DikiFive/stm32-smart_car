#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
 * @brief  初始化按键
 *
 * 此函数初始化GPIOB的Pin 1和Pin 11为上拉输入模式，用于读取按键状态。
 */
void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // 使能GPIOB时钟

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 设置为上拉输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;  // 选择Pin 1和Pin 11
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 设置最大速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);  // 初始化GPIOB
}

/**
 * @brief  获取按键编号
 *
 * 此函数检测GPIOB的Pin 1和Pin 11的状态，返回按键编号。
 * - 如果Pin 1被按下，返回1
 * - 如果Pin 11被按下，返回2
 * - 如果没有按键被按下，返回0
 *
 * @return 按键编号 (0: 无按键按下, 1: Pin 1按下, 2: Pin 11按下)
 */
uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;  // 初始化按键编号为0

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)  // 检测Pin 1是否被按下
    {
        Delay_ms(20);  // 延时去抖动
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);  // 等待按键释放
        Delay_ms(20);  // 再次延时去抖动
        KeyNum = 1;  // 设置按键编号为1
    }

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)  // 检测Pin 11是否被按下
    {
        Delay_ms(20);  // 延时去抖动
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);  // 等待按键释放
        Delay_ms(20);  // 再次延时去抖动
        KeyNum = 2;  // 设置按键编号为2
    }

    return KeyNum;  // 返回按键编号
}