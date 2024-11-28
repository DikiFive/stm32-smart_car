#include "RED.h"

/**
 * 初始化红外传感器的GPIO接口
 * 本函数配置了两个红外传感器的GPIO引脚，一个在GPIOA上，另一个在GPIOB上
 * 它们被配置为上拉输入模式，以便接收红外信号
 */
void INFRARED_Init(void)
{
    // 启用GPIO的时钟，确保GPIO可以正常工作
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 定义一个GPIO初始化结构体，用于配置GPIO口
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置GPIO口为上拉输入模式，用于接收红外信号
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // 指定配置PIN脚
    GPIO_InitStructure.GPIO_Pin = INFRARED_L_PIN;
    // 设置GPIO的速度为50MHz
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // 根据上述配置初始化GPIOA
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 指定配置PIN脚
    GPIO_InitStructure.GPIO_Pin = INFRARED_R_PIN;
    // 根据上述配置初始化GPIOB
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// 获取红外光管状态函数
// 本函数用于读取红外光管的当前状态，即是否有红外信号输入
// 返回值：返回一个包含左右两个红外避障传感器状态的结构体
InfraredStatus INFRARED_Get(void)
{
    InfraredStatus status;
    // 读取GPIOA的PIN11脚的状态
    status.left = GPIO_ReadInputDataBit(GPIOA, INFRARED_L_PIN);
    // 读取GPIOB的PIN10脚的状态
    status.right = GPIO_ReadInputDataBit(GPIOB, INFRARED_R_PIN);
    return status;
}