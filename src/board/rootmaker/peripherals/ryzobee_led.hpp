#pragma once


#include "../rootmaker_pin.h"
#include "driver/rmt.h"


#define LED_PIN    PIN_RGB_LED
#define NUM_LEDS   1

class ryzobee_led {
    public:
        //uint8_t brightness;
        
        /**
         * @brief Constructor
         */
        ryzobee_led(){};

        /**
         * @brief Initialize LED module
         */
        void init(){
            if (_initialized) return;

            _rmt_tx.rmt_mode = RMT_MODE_TX;
            _rmt_tx.channel = _getFirstFreeRMTChannel();
            _rmt_tx.gpio_num = (gpio_num_t)PIN_RGB_LED;
            _rmt_tx.mem_block_num = 1;
            _rmt_tx.clk_div = 8;   // 80MHz/8 = 10MHz
        
            _rmt_tx.tx_config.loop_en = false;
            _rmt_tx.tx_config.carrier_en = false;
            _rmt_tx.tx_config.idle_output_en = true;
            _rmt_tx.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
            
            if (_rmt_tx.channel == RMT_CHANNEL_MAX) {
                ESP_LOGE("ryzobee_led", "No free RMT channel found, please check the RMT channel status");
                return;
            }
            rmt_driver_uninstall(_rmt_tx.channel);
            rmt_config(&_rmt_tx);
            rmt_set_gpio(_rmt_tx.channel, RMT_MODE_TX, (gpio_num_t)PIN_RGB_LED, false);
            rmt_driver_install(_rmt_tx.channel, 0, 0);
            rmt_set_source_clk(_rmt_tx.channel, RMT_BASECLK_APB);
            rmt_tx_start(_rmt_tx.channel, true);

            _initialized = true;
        }

        void setColor(uint8_t r, uint8_t g, uint8_t b){
            _setRMTItem(r, g, b);
            rmt_write_items(_rmt_tx.channel, _rmt_items, NUM_LEDS * 24, true);
        }
        void setColor(uint32_t color){
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            _setRMTItem(r, g, b);
            rmt_write_items(_rmt_tx.channel, _rmt_items, NUM_LEDS * 24, true);
        }
        void setHSV(uint32_t h, uint32_t s, uint32_t v){setColor(hsv2rgb(h, s, v));};

        uint32_t hsv2rgb(uint32_t h, uint32_t s, uint32_t v) {
            h %= 360;
            uint32_t rgb_max = v * 2.55f;
            uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;
            uint32_t i = h / 60;
            uint32_t diff = h % 60;
            uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;
            uint8_t r, g, b;
            switch (i) {
            case 0: r = rgb_max; g = rgb_min + rgb_adj; b = rgb_min; break;
            case 1: r = rgb_max - rgb_adj; g = rgb_max; b = rgb_min; break;
            case 2: r = rgb_min; g = rgb_max; b = rgb_min + rgb_adj; break;
            case 3: r = rgb_min; g = rgb_max - rgb_adj; b = rgb_max; break;
            case 4: r = rgb_min + rgb_adj; g = rgb_min; b = rgb_max; break;
            default: r = rgb_max; g = rgb_min; b = rgb_max - rgb_adj; break; }
            return (r << 16) | (g << 8) | b;
        }
    private:
        rmt_config_t _rmt_tx;
        rmt_item32_t _rmt_items[NUM_LEDS * 24];
        void _setRMTItem(uint8_t r, uint8_t g, uint8_t b){
            uint32_t grb = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;
            for(int i=0;i<24;i++){
                bool bit = grb & (1 << (23-i));
                if(bit){ _rmt_items[i].level0 = 1; _rmt_items[i].duration0 = 7; _rmt_items[i].level1 = 0; _rmt_items[i].duration1 = 3;}
                else{ _rmt_items[i].level0 = 1; _rmt_items[i].duration0 = 3; _rmt_items[i].level1 = 0; _rmt_items[i].duration1 = 7; }
            }
        }
        rmt_channel_t _getFirstFreeRMTChannel(){
            rmt_channel_status_result_t status;
            rmt_get_channel_status(&status);
            for(int i=0;i<RMT_CHANNEL_MAX;i++){
                if(status.status[i] == RMT_CHANNEL_UNINIT) {
                    return (rmt_channel_t)i;
                }
            }
            return RMT_CHANNEL_MAX;
        }
        bool _initialized = false;
};
