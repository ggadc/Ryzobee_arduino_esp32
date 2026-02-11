#include "rootmaker.h"

RootMaker::RootMaker(void) {
    // 构造函数中不做硬件初始化
}

void RootMaker::begin(bool LCDEnable   , 
                      bool LEDEnable   , 
                      bool SensorEnable, 
                      bool ButtonEnable) {
    if (LCDEnable) {
        lcd.init();
    }
    if (LEDEnable) {
        led.init();
    }
    if (ButtonEnable) {
        btn.init();
    }
}
