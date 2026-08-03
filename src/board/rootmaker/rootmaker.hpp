#pragma once

#include "../ryzobee_board.hpp"
#include <LovyanGFX.hpp>
#include <Wire.h>
#include <string>
#include <vector>
#include <cstdarg>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
//#include "rootmaker_pin.h"

#include "peripherals/ryzobee_led.hpp"
#include "peripherals/ryzobee_lis2dwtr.hpp"
#include "peripherals/rootmaker_bat.hpp"
#include "peripherals/ryzobee_btn.hpp"
#include "peripherals/rootmaker_lcd.hpp"

#define SCREEN_WIDTH  240

class rootmaker : public ryzobee_board {
public:
    rootmaker(void) : ryzobee_board("RootMaker") {
        ports["rs_port1"] = {PORT_TYPE_RS, false, false, 2, PORT1_PIN1, PORT1_PIN2};
        ports["rs_port2"] = {PORT_TYPE_RS, false, false, 2, PORT2_PIN1, PORT2_PIN2};
        ports["rs_port3"] = {PORT_TYPE_RS, false, false, 2, PORT3_PIN1, PORT3_PIN2};
        ports["rs_port4"] = {PORT_TYPE_RS, false, false, 2, PORT4_PIN1, PORT4_PIN2};
        ports["s_bus"] = {PORT_TYPE_S_BUS, false, false, 14, S_BUS1, S_BUS2, S_BUS3, S_BUS4, S_BUS5, S_BUS6, S_BUS7, S_BUS8, S_BUS9, S_BUS10, S_BUS11, S_BUS12, S_BUS13, S_BUS14};
        ports["f_bus"] = {PORT_TYPE_F_BUS, false, false, 16, F_BUS9, F_BUS10, F_BUS11, F_BUS12, F_BUS13, F_BUS14, F_BUS15, F_BUS16, F_BUS17, F_BUS24};
        
        _i2c_mutex = xSemaphoreCreateMutex();
        if (_i2c_mutex == nullptr) {
            ESP_LOGE("rootmaker", "Failed to create I2C mutex");
        }
        lcd = new rootmaker_lcd(&_dev_i2c, &_i2c_mutex);
    }
    ~rootmaker() {
        if (lcd != nullptr) {
            delete lcd;
            lcd = nullptr;
        }
        if (_i2c_mutex != nullptr) {
            vSemaphoreDelete(_i2c_mutex);
            _i2c_mutex = nullptr;
        }
    }
    std::string getBoardName() override { return _board_name; }
    std::string getBoardInfo() override { return "RootMaker board"; }   
    /* begin parameters:
        bool LEDEnable      = true,
        bool SensorEnable   = true,
        bool ButtonEnable   = true,
        bool BatteryEnable  = true,
        bool LCDEnable      = true
    */
    void begin(bool LEDEnable, bool SensorEnable, bool ButtonEnable, bool BatteryEnable) {
        begin(LEDEnable, SensorEnable, ButtonEnable, BatteryEnable, false);
    }
    void begin() override { begin(true, true, true, true, true);}
    void begin(bool LEDEnable, bool SensorEnable, bool ButtonEnable, bool BatteryEnable, bool LCDEnable) {
        ESP_LOGI("rootmaker", "CPU Freq: %lu Hz", getCpuFrequencyMhz() * 1000000UL);
        ESP_LOGI("rootmaker", "APB Freq: %lu Hz", getApbFrequency());
        if (!_i2c_initialized) {
            _dev_i2c.begin(BOADR_SDA, BOADR_SCL, 100000);
            _i2c_initialized = true;
        }
        if(SensorEnable) {
            gsensor.init();
        }
        if(LEDEnable) {
            led.init();  
        }
        if(BatteryEnable) {
            ec.init();
        }
        if(ButtonEnable) {
            btns.init(true);
            btns.add(BUTN_PIN, 1, DEBOUNCE_MS);
        }
        if (LCDEnable && lcd != nullptr) {
            lcd->init();
        }
    }
    TwoWire& getWire() { return _dev_i2c; }
public:
    ryzobee_led         led;
    ryzobee_lis2dwtr    gsensor = ryzobee_lis2dwtr((TwoWire*)&_dev_i2c,(SemaphoreHandle_t*)&_i2c_mutex);
    rootmaker_ec        ec = rootmaker_ec((TwoWire*)&_dev_i2c,(SemaphoreHandle_t*)&_i2c_mutex);
    ryzobee_btn         btns;
    rootmaker_lcd*      lcd = nullptr;


private:
    TwoWire _dev_i2c = TwoWire(0);
    bool _i2c_initialized = false;
    SemaphoreHandle_t _i2c_mutex;
};