/*
 * Yoke_MOTO.ino — Motor driver module example
 *
 * Uses the Yoke library's Yoke_moto class (RS port, MCPWM-based DC motor).
 * Speed range: 0-100, mapped to MCPWM duty percentage.
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>               // Yoke library base — triggers IDE library discovery
#include <yoke_moto/yoke_moto.hpp>        // Yoke_moto class (src/yoke_moto/)

Ryzobee board;
Yoke_moto moto;

void setup() {
    Serial.begin(115200);

    // MCPWM expects duty in percent; override the driver's legacy 0-255 mapping.
    moto.setSpeedMap(0, 100);
    if (moto.begin(board.maker.useport("rs_port4")) != YOKE_PORT_ERROR_NONE) {
        Serial.println("MOTO initialization failed");
        return;
    }
    Serial.println("MOTO ready on RS port 4");
}

void loop() {
    if (!moto.isInitialized()) {
        delay(100);
        return;
    }

    // Forward 2s → stop 1s → backward 2s → stop 1s
    moto.setmoto(Yoke_moto::DIR_FORWARD, 100);  delay(2000);
    moto.setmoto(Yoke_moto::DIR_FORWARD, 0);    delay(1000);
    moto.setmoto(Yoke_moto::DIR_REVERSE, 100);  delay(2000);
    moto.setmoto(Yoke_moto::DIR_REVERSE, 0);    delay(1000);

    // Speed ramp forward (0→100)
    for (int i = 0; i <= 100; i++) {
        moto.setmoto(Yoke_moto::DIR_FORWARD, i);
        delay(10);
    }

    // Speed ramp reverse (0→100)
    for (int i = 0; i <= 100; i++) {
        moto.setmoto(Yoke_moto::DIR_REVERSE, i);
        delay(10);
    }

    moto.setmoto(Yoke_moto::DIR_FORWARD, 0);  // stop
    delay(1000);
}
