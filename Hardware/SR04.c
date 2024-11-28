#include "SR04.h"

#define Echo GPIO_Pin_12 // 定义HC-SR04模块的Echo脚连接到GPIOB12
#define Trig GPIO_Pin_11 // 定义HC-SR04模块的Trig脚连接到GPIOB11

uint64_t time     = 0; // 声明计时变量
uint64_t time_end = 0; // 声明存储回波信号时间的变量
/**
 * @brief  初始化HC-SR04超声波模块的GPIO接口
 */
void HC_SR04_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 启用GPIOB的外设时钟
    GPIO_InitTypeDef GPIO_InitStructure;                  // 定义结构体
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     // 设置GPIO口为推挽输出
    GPIO_InitStructure.GPIO_Pin   = Trig;                 // 设置Trig脚的GPIO口
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // 设置GPIO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                // 初始化GPIOB

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 设置Echo脚的GPIO口为下拉输入模式
    GPIO_InitStructure.GPIO_Pin  = Echo;          // 设置Echo脚的GPIO口
    GPIO_Init(GPIOB, &GPIO_InitStructure);        // 初始化GPIOB
    GPIO_WriteBit(GPIOB, Trig, Bit_RESET);        // Trig脚输出低电平
    Delay_us(15);                                 // 延时15微秒
}

/**
 * @brief  测量距离并返回单位为毫米的距离结果
 * @return 距离结果，单位为毫米
 */
int16_t sonar_mm(void)
{
    uint32_t Distance, Distance_mm = 0;
    GPIO_WriteBit(GPIOB, Trig, Bit_SET);             // Trig脚输出高电平
    Delay_us(15);                                    // 延时15微秒
    GPIO_WriteBit(GPIOB, Trig, Bit_RESET);           // Trig脚输出低电平
    while (GPIO_ReadInputDataBit(GPIOB, Echo) == 0); // 等待Echo脚低电平结束
    time = 0;                                        // 计时清零
    while (GPIO_ReadInputDataBit(GPIOB, Echo) == 1); // 等待Echo脚高电平结束
    time_end = time;                                 // 记录结束时的时间
    if (time_end / 100 < 38)                         // 判断是否小于38毫秒，大于38毫秒为超时，直接返回0
    {
        Distance    = (time_end * 346) / 2; // 计算距离，25°C空气中的音速为346m/s
        Distance_mm = Distance / 100;       // 转换为毫米
    }
    return Distance_mm; // 返回测距结果，单位为毫米
}

/**
 * @brief  测量距离并返回单位为米的距离结果
 * @return 距离结果，单位为米
 */
float sonar(void)
{
    uint32_t Distance, Distance_mm = 0;
    float Distance_m = 0;
    GPIO_WriteBit(GPIOB, Trig, Bit_SET); // Trig脚输出高电平
    Delay_us(15);
    GPIO_WriteBit(GPIOB, Trig, Bit_RESET); // Trig脚输出低电平
    while (GPIO_ReadInputDataBit(GPIOB, Echo) == 0);
    time = 0;
    while (GPIO_ReadInputDataBit(GPIOB, Echo) == 1);
    time_end = time;
    if (time_end / 100 < 38) {
        Distance    = (time_end * 346) / 2;
        Distance_mm = Distance / 100;
        Distance_m  = Distance_mm / 1000; // 转换为米
    }
    return Distance_m; // 返回测距结果，单位为米
}

/**
 * @brief  获取距离测量结果，并格式化为字符串发送到串口
 * @param unit 单位，0表示毫米，1表示米
 */
void get_distance(int unit)
{
    int Distance_mm = sonar_mm(); // 获取距离测量结果，单位毫米（mm）

    char buffer[50]; // 用于存储格式化后的字符串

    // 格式化距离测量结果为字符串
    if (unit == 1) {
        snprintf(buffer, sizeof(buffer), "%dmm", Distance_mm);
        Serial_SendString(buffer); // 发送单位为mm的数据
    }

    Delay_ms(300); // 延时300毫秒
}

/**
 * @brief  TIM3中断服务函数，用于计时
//  */
// void TIM3_IRQHandler(void)
// {
//     if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) // 检查TIM3定时器的更新中断标志位
//     {
//         time++;                                     // 每10微秒计时变量加1
//         TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除更新中断标志位
//     }
// }

/**
 * @brief  TIM4中断服务函数，用于计时
 */
void TIM4_IRQHandler(void) // 更新中断函数
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) // 获取TIM4定时器的更新中断标志位
    {
        time++;
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // 清除更新中断标志位
    }
}