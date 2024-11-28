#include "dk.h"

#define DISTANCE_THRESHOLD 150

int main(void)
{
    car_init();          // 初始化汽车相关设置
    int Distance_mm = 0; // 获取距离测量结果，单位毫米（mm）
    uint8_t lred;
    uint8_t rred;

    // 进入主循环，处理汽车的各种操作和事件
    while (1) {
        lred = INFRARED_Get().left;
        rred = INFRARED_Get().right;

        ProcessReceivedPacket();
        Distance_mm = sonar_mm();

        if (avoid_flag == 1) {
            if (Distance_mm < DISTANCE_THRESHOLD) {
                car_back();
                Delay_ms(200);
                car_cicle();
                Delay_ms(200);
            } else {
                car_go();
            }
        }

        // GPIO_ResetBits(GPIOB, motor_power_pin);
        // GPIO_SetBits(GPIOB, motor_power_pin);
    }
}