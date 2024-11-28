#include "dk.h"

#define DISTANCE_THRESHOLD 125

int Distance_mm = 0; // 获取距离测量结果，单位毫米（mm）
uint8_t lred;
uint8_t rred;

/**
 * 使用SR04超声波传感器进行避障
 */
void avoid_SR04(void);

/**
 * 使用红外线传感器进行避障
 */
void avoid_Infrared(void);

/**
 * 主函数入口
 */
int main(void)
{
    car_init(); // 初始化汽车相关设置

    // 进入主循环，处理汽车的各种操作和事件
    while (1) {
        // 获取红外线传感器的左右读数
        lred = INFRARED_Get().left;
        rred = INFRARED_Get().right;

        // 更新距离测量值
        Distance_mm = sonar_mm();

        // 处理接收到的数据包
        ProcessReceivedPacket();

        avoid_SR04();
        avoid_Infrared();
    }
}

/**
 * 红外线避障函数
 */
void avoid_Infrared(void)
{
    // 根据障碍物 avoidance 标志和距离判断是否需要避障
    if (avoid_flag == 1) {
        if (Distance_mm > DISTANCE_THRESHOLD) {
            // 如果距离大于阈值，继续前进
            car_go();
        } else {
            // 如果距离小于等于阈值，根据红外线传感器读数决定避障方向
            if (lred == 1) {
                // 如果左侧检测到障碍物，向左转
                car_left();
                Delay_ms(200);
                car_go();
            } else if (rred == 1) {
                // 如果右侧检测到障碍物，向右转
                car_right();
                Delay_ms(200);
                car_go();
            } else {
                // 如果两侧都未检测到障碍物，后退并随机选择方向
                car_back();
                Delay_ms(200);
                // 随机选择左转或右转
                if (rand() % 2 == 0) {
                    car_left();
                } else {
                    car_right();
                }
                Delay_ms(200);
                car_go();
            }
        }
    }
}

/**
 * 超声波SR04避障函数
 */
void avoid_SR04(void)
{
    if (avoid_sflag == 1) {
        if (Distance_mm > DISTANCE_THRESHOLD) {
            car_go();
        } else {
            car_back();
            Delay_ms(200);
            // 随机选择左转或右转
            if (rand() % 2 == 0) {
                car_left();
            } else {
                car_right();
            }
            Delay_ms(200);
            car_go();
        }
    }
}