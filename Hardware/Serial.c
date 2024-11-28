#include "Serial.h" // 设备头文件

uint8_t Serial_RxData; // 存储接收到的串口数据
uint8_t Serial_RxFlag; // 标记是否接收到新的串口数据

// 定义接收和发送引脚
#define RX_Pin GPIO_Pin_10
#define TX_Pin GPIO_Pin_9

// 定义数据包的头和尾标识符
#define PKT_HEADER 0xA5
#define PKT_TAIL   0x5A
// 定义数据包最大长度为64字节
#define PKT_MAX_LEN 64

// 定义数据包结构体
typedef struct {
    uint8_t header;
    uint8_t data[PKT_MAX_LEN];
    uint8_t checksum;
    uint8_t tail;
} Packet;

// 定义全局变量用于存储接收到的数据包和接收状态
volatile Packet packet;
volatile uint8_t packet_index    = 0;
volatile uint8_t packet_received = 0;
volatile uint8_t avoid_flag      = 0;
volatile uint8_t avoid_sflag     = 0;

// 定义接收状态
typedef enum {
    STATE_WAIT_HEADER,
    STATE_RECEIVE_BOOL,
    STATE_RECEIVE_INT1,
    STATE_RECEIVE_INT2,
    STATE_RECEIVE_INT3,
    STATE_RECEIVE_INT4,
    STATE_RECEIVE_CHECKSUM,
    STATE_RECEIVE_TAIL
} RxState;

volatile RxState rx_state = STATE_WAIT_HEADER;

/**
 * @brief  初始化串口通信
 *         配置USART1进行串口通信，包括GPIO和NVIC设置。
 */
void Serial_Init(void)
{
    // 使能USART1和GPIOA的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PA9为复用推挽输出，用于USART1 TX
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = TX_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置PA10为上拉输入，用于USART1 RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin   = RX_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置USART1参数
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    // 使能USART1接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // 配置NVIC优先级组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 配置NVIC中断
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_Init(&NVIC_InitStructure);

    // 使能USART1
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief  发送一个字节的数据
 * @param  Byte: 要发送的字节
 */
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
 * @brief  发送一个数组
 * @param  Array: 要发送的数组指针
 * @param  Length: 数组长度
 */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++) {
        Serial_SendByte(Array[i]);
    }
}

/**
 * @brief  发送一个字符串
 * @param  String: 要发送的字符串指针
 */
void Serial_SendString(char *String)
{
    uint16_t i;
    for (i = 0; String[i] != '\0'; i++) {
        Serial_SendByte(String[i]);
    }
}

/**
 * @brief  计算幂
 * @param  X: 底数
 * @param  Y: 指数
 * @return 结果
 */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

/**
 * @brief  发送一个数字
 * @param  Number: 要发送的数字
 * @param  Length: 数字的位数
 */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  重定向标准输出
 * @param  ch: 要发送的字符
 * @param  f: 文件指针
 * @return 0
 */
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return 0;
}

/**
 * @brief  格式化输出
 * @param  format: 格式化字符串
 * @param  ...: 可变参数列表
 */
void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

/**
 * @brief  获取接收标志
 * @return 1: 接收到新数据，0: 无新数据
 */
uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1) {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief  获取接收到的数据
 * @return 接收到的数据
 */
uint8_t Serial_GetRxData(void)
{
    return Serial_RxData;
}

/**
 * @brief  从串口接收数据并原封不动地发送出去
 */
void echo_serial_data(void)
{
    // 检查是否有数据可读
    if (Serial_GetRxFlag()) {
        // 从串口接收一个字节
        uint8_t received_byte = Serial_GetRxData();

        // 将接收到的字节发送回串口
        Serial_SendByte(received_byte);
    }
}
/**
 * @brief  USART1中断处理函数
 */
/**
 * @brief USART1中断服务例程
 *
 * 本函数用于处理USART1的中断请求，主要负责串口接收数据的处理
 * 它通过检查接收寄存器非空中断标志来确定是否有数据接收
 * 如果有数据，则读取数据并根据通信协议解析数据包
 */
void USART1_IRQHandler(void)
{
    // 检查USART1的接收寄存器非空中断是否发生
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        // 读取串口接收的数据
        Serial_RxData = USART_ReceiveData(USART1);
        // 设置接收标志
        Serial_RxFlag = 1;
        // 再次读取数据，因为前面已经读取过了，这里应该是逻辑错误
        uint8_t received_byte = USART_ReceiveData(USART1);
        // 清除中断标志，准备下一次中断
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);

        // 根据接收状态处理数据
        switch (rx_state) {
            case STATE_WAIT_HEADER:
                // 等待头部状态，如果接收到头部，则进入接收数据状态
                if (received_byte == PKT_HEADER) {
                    rx_state      = STATE_RECEIVE_BOOL;
                    packet.header = received_byte;
                    packet_index  = 0;
                }
                break;

            case STATE_RECEIVE_BOOL:
                // 接收布尔类型数据
                packet.data[packet_index++] = received_byte;
                rx_state                    = STATE_RECEIVE_INT1;
                break;

            case STATE_RECEIVE_INT1:
                // 接收整型数据的第一部分
                packet.data[packet_index++] = received_byte;
                rx_state                    = STATE_RECEIVE_INT2;
                break;

            case STATE_RECEIVE_INT2:
                // 接收整型数据的第二部分
                packet.data[packet_index++] = received_byte;
                rx_state                    = STATE_RECEIVE_INT3;
                break;

            case STATE_RECEIVE_INT3:
                // 接收整型数据的第三部分
                packet.data[packet_index++] = received_byte;
                rx_state                    = STATE_RECEIVE_INT4;
                break;

            case STATE_RECEIVE_INT4:
                // 接收整型数据的第四部分
                packet.data[packet_index++] = received_byte;
                rx_state                    = STATE_RECEIVE_CHECKSUM;
                break;

            case STATE_RECEIVE_CHECKSUM:
                // 接收校验和
                packet.checksum = received_byte;
                rx_state        = STATE_RECEIVE_TAIL;
                break;

            case STATE_RECEIVE_TAIL:
                // 接收包尾，如果正确，则计算校验和并设置接收完成标志
                if (received_byte == PKT_TAIL) {
                    // 计算校验和
                    uint8_t calculated_checksum = 0;
                    for (int i = 0; i < packet_index; i++) {
                        calculated_checksum += packet.data[i];
                    }
                    calculated_checksum &= 0xFF; // 取低八位

                    // 比较接收到的校验和和计算出的校验和
                    if (calculated_checksum == packet.checksum) {
                        // 数据正确，设置接收完成标志
                        packet_received = 1;
                    } else {
                        // 校验和错误，忽略数据
                        packet_received = 0;
                    }
                } else {
                    // 包尾错误，忽略数据
                    packet_received = 0;
                }
                // 重置状态机
                rx_state = STATE_WAIT_HEADER;
                break;

            default:
                // 如果状态机处于未知状态，则重置到等待头部状态
                rx_state = STATE_WAIT_HEADER;
                break;
        }
    }
}

/**
 * @brief  处理接收到的数据包
 */
void ProcessReceivedPacket(void)
{
    if (packet_received) {
        // 提取bool值
        uint8_t bool_value = packet.data[0];

        // 提取int值（假设是小端存储）
        uint32_t int_value = (packet.data[4] << 24) |
                             (packet.data[3] << 16) |
                             (packet.data[2] << 8) |
                             packet.data[1];

        // 处理数据
        if (bool_value == 1) {
            GPIO_SetBits(GPIOB, motor_power_pin);
        } else if (bool_value == 0) {
            GPIO_ResetBits(GPIOB, motor_power_pin);
        }

        switch (int_value) {
            case 1:
                car_go();
                break;

            case 2:
                car_back();
                break;
            case 3:
                car_left();
                break;

            case 4:
                car_right();
                break;

            case 5:
                car_cicle();
                break;

            case 6:
                avoid_flag = 1; // 进入避免模式
                break;

            case 7:
                avoid_sflag = 1;
                break;

            case 0:
                car_stop();
                avoid_flag  = 0; // 退出避免模式
                avoid_sflag = 0;
                break;
        }

        // 清零接收标志
        packet_received = 0;
    }
}