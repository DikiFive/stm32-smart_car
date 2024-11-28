#include "stm32f10x.h" // 设备头文件

/**
 * 初始化TIM2通道2 (PA1) 的PWM输出
 * 此函数配置TIM2和GPIOA以实现PWM输出
 */
void PWM_TIM2_Init(void)
{
    // 使能TIM2和GPIOA的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化GPIO结构体用于PWM输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  // 复用推挽输出模式
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;       // PA1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 最大速度50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);            // 初始化GPIOA

    // 配置TIM2内部时钟
    TIM_InternalClockConfig(TIM2);

    // 初始化TIM2定时器基本结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;       // 时钟分频因子
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period            = 20000 - 1;          // 自动重装载寄存器值 (ARR)
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 72 - 1;             // 预分频器值 (PSC)
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  // 重复计数器值
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);                   // 初始化TIM2定时器

    // 初始化TIM2输出比较结构体
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);                       // 使用默认值初始化结构体
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;        // PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;    // 高电平有效
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_Pulse       = 0;                      // 比较寄存器值 (CCR)
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);                      // 初始化TIM2通道2

    // 使能TIM2
    TIM_Cmd(TIM2, ENABLE);
}

void PWM_TIM3_Init(void)
{
    // 使能TIM3和GPIOB的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 初始化GPIO结构体用于PWM输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  // 复用推挽输出模式
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;       // PB1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 最大速度50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 初始化GPIOB

    // 配置TIM3内部时钟
    TIM_InternalClockConfig(TIM3);

    // 初始化TIM3定时器基本结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;       // 时钟分频因子
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period            = 20000 - 1;          // 自动重装载寄存器值 (ARR)
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 72 - 1;             // 预分频器值 (PSC)
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  // 重复计数器值
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);                   // 初始化TIM3定时器

    // 初始化TIM3输出比较结构体
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);                       // 使用默认值初始化结构体
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;        // PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;    // 高电平有效
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_Pulse       = 0;                      // 比较寄存器值 (CCR)
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      // 初始化TIM3通道4

    // 使能TIM3
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * 设置TIM3通道4的比较值
 * @param Compare 要设置的比较值
 */
void PWM_TIM3_SetCompare4(uint16_t Compare)
{
    TIM_SetCompare4(TIM3, Compare); // 设置TIM3通道4的比较值
}

/**
 * 设置TIM2通道2的比较值
 * @param Compare 要设置的比较值
 */
void PWM_TIM2_SetCompare2(uint16_t Compare)
{
    TIM_SetCompare2(TIM2, Compare); // 设置TIM2通道2的比较值
}

void PWM_SetCompare3(uint16_t Compare)
{
    TIM_SetCompare3(TIM2, Compare);
}