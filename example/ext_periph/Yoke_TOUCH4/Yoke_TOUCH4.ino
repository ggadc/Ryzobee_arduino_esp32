/*
 * Yoke_TOUCH4 - SC92F837x four-channel touch module, UART 9600 8-N-1.
 * Host TX -> module RX; host RX <- module TX; common ground required.
 */
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_touch4/yoke_touch4.hpp>

Ryzobee board;
Yoke_touch4 touch(UART_NUM_1);
bool readFailed = false;

void setup() {
    Serial.begin(115200);
    delay(500);
    // begin() initializes the module and starts background polling.
    // Use begin(port, false, config) for manual polling with custom settings.
    auto* port = board.maker.useport("rs_port4");
    if (port == nullptr || port->size() < PORT_PIN_START + 2) {
        Serial.println("TOUCH4 RS port 4 is unavailable");
        return;
    }
    Touch4Config config;
    int result = touch.begin(port, true, config);
    if (result != YOKE_PORT_ERROR_NONE) {
        Serial.printf("TOUCH4 initialization failed: %d\n", result);
        return;
    }
    Serial.printf("TOUCH4 firmware: 0x%04X\n", touch.getFirmwareVersion());
}

void loop() {
    if (!touch.isInitialized()) {
        delay(100);
        return;
    }
    delay(1);
    if (!touch.isConnected()) {
        if (!readFailed) {
            Serial.println("TOUCH4 read timeout");
        }
        readFailed = true;
        return;
    }
    if (readFailed) {
        Serial.println("TOUCH4 communication restored");
        readFailed = false;
    }
    for (uint8_t channel = 0; channel < Yoke_touch4::CHANNEL_COUNT; channel++) {
        if (touch.wasPressed(channel)) {
            Serial.printf("Channel %u pressed\n", channel + 1);
        }
        if (touch.wasReleased(channel)) {
            Serial.printf("Channel %u released\n", channel + 1);
        }
    }
}
