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
uint32_t lastErrorPrint = 0;
bool readFailed = false;

void setup() {
    Serial.begin(115200);
    if (touch.begin(board.maker.useport("rs_port4")) != YOKE_PORT_ERROR_NONE) {
        Serial.println("TOUCH4 initialization failed; check wiring and power");
        return;
    }
    Serial.printf("TOUCH4 firmware: 0x%04X\n", touch.getFirmwareVersion());
}

void loop() {
    if (!touch.isInitialized()) {
        delay(100);
        return;
    }
    touch.update();
    if (!touch.isConnected()) {
        if (!readFailed || millis() - lastErrorPrint >= 5000) {
            Serial.println("TOUCH4 read timeout");
            lastErrorPrint = millis();
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
    delay(1);
}
