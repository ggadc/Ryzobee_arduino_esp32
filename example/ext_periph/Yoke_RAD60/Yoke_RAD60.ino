/*
 * Yoke_RAD60.ino — AT6010 mmWave radar example
 *
 * Uses the Yoke library's Yoke_rad60 class (RS port, UART-based).
 * SDA → TX, SCL → RX. Baud rate fixed at 115200.
 * Reports the first target only: distance, angle, velocity, and ID.
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>                   // Yoke library base — triggers IDE discovery
#include <yoke_rad60/yoke_rad60.hpp>          // Yoke_rad60 class

#define PRINT_INTERVAL_MS  200

Ryzobee board;
Yoke_rad60 radar();        // UART1, 115200 baud

uint32_t last_print_ms = 0;
bool     last_detected  = false;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("=== RAD60 mmWave radar ===");

    // Initialize UART and the radar on RS port 4 (TX=GPIO17, RX=GPIO18).
    if (radar.begin(board.maker.useport("rs_port4")) != YOKE_PORT_ERROR_NONE) {
        Serial.println("[FATAL] Radar initialization failed; check wiring and power");
        return;
    }

    Serial.println("[SYS] Data acquisition started...");
    Serial.println("----------------------------------------");
}

void loop() {
    if (!radar.isInitialized()) {
        delay(100);
        return;
    }

    radar.update();

    bool detected = radar.isDetected();
    if (detected != last_detected) {
        last_detected = detected;
        Serial.println(detected ? "[EVENT] >>> Target entered <<<"
                                : "[EVENT] >>> Target left <<<");
    }

    if (millis() - last_print_ms >= PRINT_INTERVAL_MS) {
        last_print_ms = millis();
        if (!radar.isDetected()) {
            static uint32_t n = 0;
            if (++n % 10 == 0) Serial.println("[RAD] Monitoring... (vacant)");
            return;
        }
        Serial.println("--- Target detected ---");
        Serial.printf("  dist=%u cm angle=%d deg vel=%d cm/s id=%u\n",
                      radar.getTargetDistance(),
                      radar.getTargetAngle(),
                      radar.getTargetVelocity(),
                      radar.getTargetID());
        Serial.println("-----------------------");
    }

    if (!radar.isConnected()) {
        Serial.println("[WARN] Comm error!");
        delay(1000);
    }
}
