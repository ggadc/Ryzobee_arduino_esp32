/*
 * Yoke_RAD60.ino — AT6010 mmWave radar example
 *
 * Uses the Yoke library's Yoke_rad60 class (RS port, UART-based).
 * RS port 4: host TX=GPIO17, RX=GPIO18. Baud rate fixed at 115200.
 * Reports the first target only: distance, angle, velocity, and ID.
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>                   // Yoke library base — triggers IDE discovery
#include <yoke_rad60/yoke_rad60.hpp>          // Yoke_rad60 class

Ryzobee board;
Yoke_rad60 radar(UART_NUM_1, 115200);        // UART1, 115200 baud

Rad60_TargetInfo_t last_target = {};
bool last_detected = false;
bool last_connected = true;

void setup() {
    Serial.begin(115200);
    delay(500);

    // begin() initializes the radar and starts its background UART task.
    // Use begin(port, false) for manual polling, or begin(port, true, false)
    // to keep the existing detection settings.
    // ULP firmware only: begin(port, true, true, true), within its power-on window.
    auto* port = board.maker.useport("rs_port4");
    if (port == nullptr || port->size() < PORT_PIN_START + 2) {
        Serial.println("RS port 4 is unavailable");
        return;
    }
    // Preserve the module's existing detection settings.
    int result = radar.begin(port, true, false);
    if (result != YOKE_PORT_ERROR_NONE) {
        Serial.printf("[ERROR] RAD60 initialization failed: %d\n", result);
        return;
    }

    Serial.println("RAD60 ready");
}

void loop() {
    if (!radar.isInitialized()) {
        delay(100);
        return;
    }

    delay(1);
    bool connected = radar.isConnected();
    if (connected != last_connected) {
        last_connected = connected;
        Serial.println(connected ? "[RAD] Communication restored"
                                 : "[WARN] RAD60 communication lost");
    }

    bool detected = radar.isDetected();
    if (!detected) {
        if (last_detected) Serial.println("[RAD] Target left");
        last_detected = false;
        return;
    }

    Rad60_TargetInfo_t target = {
        radar.getTargetDistance(),
        radar.getTargetAngle(),
        radar.getTargetVelocity(),
        radar.getTargetID()
    };
    if (!last_detected || target.range_val != last_target.range_val ||
        target.angle_val != last_target.angle_val ||
        target.velo_val != last_target.velo_val || target.objid != last_target.objid) {
        Serial.printf("[RAD] dist=%u cm angle=%d deg vel=%d cm/s id=%u\n",
                      target.range_val, target.angle_val, target.velo_val, target.objid);
        last_target = target;
    }
    last_detected = true;
}
