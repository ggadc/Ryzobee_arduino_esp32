#pragma once

#include <LovyanGFX.hpp>
// #include "boards.h"
#include "Touch_CST816T.hpp"

#define LCD_WRITE_FREQ 40000000
#define LCD_READ_FREQ  16000000
#define LCD_BL_FREQ    44100
#define LCD_BL_CHANNEL 7

#define SCREEN_WIDTH  240

class Rootmaker_Lcd : public lgfx::LGFX_Device {
    lgfx::Panel_ST7789  _panel_instance;
  
    lgfx::Bus_SPI       _bus_instance;
  
    lgfx::Light_PWM     _light_instance;
  
    lgfx::Touch_CST816T _touch_instance;
  
  public:
        Rootmaker_Lcd();
        void configure();  // 配置屏幕参数 (在构造函数中自动调用)
        void init();       // 初始化硬件 (需要在 setup() 中手动调用)
        void clear();
        lgfx::Panel_Device* panel() { return &_panel_instance; }
        // void drawString(const char* str, int16_t x, int16_t y);
        // void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        // void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
        // void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        // void drawPixel(int16_t x, int16_t y, uint16_t color);
        // void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, uint16_t color);
};
