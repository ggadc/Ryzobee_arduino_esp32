#pragma once

#include <LovyanGFX.hpp>
#include "rootmaker_pin.h"
#include "lcd/Touch_CST816T.hpp"
#include "lcd/Rootmaker_lcd.h"
#include "led/Rootmaker_led.h"
#include "button/Rootmaker_btn.h"

#define SCREEN_WIDTH  240

class RootMaker {
  public:
    RootMaker(void);

    void begin(bool LCDEnable      = true, 
               bool LEDEnable      = true, 
               bool SensorEnable   = true, 
               bool ButtonEnable   = true);

    Rootmaker_Lcd lcd;  // 公开的 LCD 成员，用户通过 device.lcd 访问屏幕
    Rootmaker_led led;  // 公开的 LED 成员，用户通过 device.led 访问 LED
    Rootmaker_btn btn;  // 公开的按钮成员，用户通过 device.button 访问按钮
};
