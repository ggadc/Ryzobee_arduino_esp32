#pragma once

#include <LovyanGFX.hpp>
#include <Wire.h>

#include "../rootmaker_pin.h"
#include "Touch_CST816T.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define LCD_WRITE_FREQ 40000000
#define LCD_READ_FREQ  16000000
#define LCD_BL_FREQ    44100
#define LCD_BL_CHANNEL 7

static constexpr uint16_t ROOTMAKER_LCD_SIZE = 240;

class rootmaker_lcd : public lgfx::LGFX_Device {
public:
    rootmaker_lcd(TwoWire* wire, SemaphoreHandle_t* mutex)
        : _dev_i2c(wire), _i2c_mutex(mutex) {
        configure();
    }

    void init() {
        if (_initialized) {
            return;
        }
        lgfx::LGFX_Device::init();
        _initialized = true;
    }

    void clear() {
        fillScreen(TFT_BLACK);
    }

    bool getTouch(uint16_t* x, uint16_t* y) {
        if (x == nullptr || y == nullptr || _i2c_mutex == nullptr) {
            return false;
        }

        bool touched = false;
        if (xSemaphoreTake(*_i2c_mutex, portMAX_DELAY) == pdTRUE) {
            lgfx::touch_point_t point;
            int count = lgfx::LGFX_Device::getTouch(&point, 1);
            if (count > 0) {
                *x = point.x;
                *y = point.y;
                touched = true;
            }
            xSemaphoreGive(*_i2c_mutex);
        }
        return touched;
    }

private:
    void configure() {
        {
            auto cfg = _bus_instance.config();

            cfg.spi_mode = 0;
            cfg.freq_write = LCD_WRITE_FREQ;
            cfg.freq_read = LCD_READ_FREQ;
            cfg.spi_3wire = true;
            cfg.use_lock = true;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            cfg.pin_sclk = LCD_SCK;
            cfg.pin_mosi = LCD_SDA;
            cfg.pin_miso = -1;
            cfg.pin_dc = LCD_RS;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();

            cfg.pin_cs = LCD_CS;
            cfg.pin_rst = LCD_RST;
            cfg.pin_busy = -1;
            cfg.panel_width = ROOTMAKER_LCD_SIZE;
            cfg.panel_height = ROOTMAKER_LCD_SIZE;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = true;
            cfg.invert = true;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;

            _panel_instance.config(cfg);
        }

        {
            auto cfg = _light_instance.config();

            cfg.pin_bl = LCD_BL;
            cfg.invert = false;
            cfg.freq = LCD_BL_FREQ;
            cfg.pwm_channel = LCD_BL_CHANNEL;

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

        {
            auto cfg = _touch_instance.config();

            cfg.x_min = 0;
            cfg.x_max = ROOTMAKER_LCD_SIZE;
            cfg.y_min = 0;
            cfg.y_max = ROOTMAKER_LCD_SIZE;
            cfg.pin_int = TP_INT;
            cfg.pin_rst = TP_RST;
            cfg.bus_shared = true;
            cfg.offset_rotation = 0;
            cfg.i2c_port = 0;
            cfg.i2c_addr = TP_ADDR;
            cfg.pin_sda = TP_SDA;
            cfg.pin_scl = TP_SCL;
            cfg.freq = TP_FREQ;

            _touch_instance.config(cfg);
            _touch_instance.setWire(_dev_i2c);
            _panel_instance.setTouch(&_touch_instance);
        }

        setPanel(&_panel_instance);
    }

    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;
    lgfx::Touch_CST816T _touch_instance;
    TwoWire* _dev_i2c = nullptr;
    SemaphoreHandle_t* _i2c_mutex = nullptr;
    bool _initialized = false;
};