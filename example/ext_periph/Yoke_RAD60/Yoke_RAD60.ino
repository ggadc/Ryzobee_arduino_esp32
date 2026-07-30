/*
 * Yoke_RAD60.ino — AT6010 mmWave radar example
 */

#include "Ryzobee.h"

#define PRINT_INTERVAL_MS  200

Ryzobee board(RYZOBEE_ROOTMAKER);
Yoke_rad60 radar;

uint32_t last_print_ms = 0;
bool     last_detected  = false;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("=== RAD60 mmWave radar ===");

    board.rootmaker.begin();
    radar.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT4, UART_NUM_1, 115200);

    Serial.printf("[SYS] RAD60 ready on PORT4\n");
    Serial.println("[RAD] Waiting for radar startup (2s)...");
    delay(2000);

    Serial.println("[RAD] Trying communication mode...");
    radar.enterCommunicationMode(true);
    delay(200);

    Serial.println("[RAD] Testing communication...");
    uint8_t state = 0xFF;
    if (radar.getRadarState(&state)) {
        Serial.printf("[RAD] OK (state: 0x%02X)\n", state);
    } else {
        Serial.println("[FATAL] No response — check wiring/power!");
        while (1) { delay(1000); }
    }

    Serial.println("[RAD] Applying default config...");
    radar.applyDefaultConfig();
    Serial.println("[SYS] Data acquisition started...");
    Serial.println("----------------------------------------");
}

void loop() {
    radar.update();

    bool detected = radar.isDetected();
    if (detected != last_detected) {
        last_detected = detected;
        Serial.println(detected ? "[EVENT] >>> Target entered <<<" : "[EVENT] >>> Target left <<<");
    }

    if (millis() - last_print_ms >= PRINT_INTERVAL_MS) {
        last_print_ms = millis();
        if (!radar.isDetected()) { static uint32_t n=0; if(++n%10==0) Serial.println("[RAD] Monitoring... (vacant)"); return; }
        uint8_t n = radar.getTargetCount();
        Serial.println("--- Target detected ---");
        Serial.printf("  Count: %d\n", n);
        for (uint8_t i=0; i<n; i++)
            Serial.printf("  T%d: dist=%d cm angle=%d deg vel=%d m/s id=%d\n",
                          i+1, radar.getTargetDistance(i), radar.getTargetAngle(i),
                          radar.getTargetVelocity(i), radar.getTargetID(i));
        Serial.println("-----------------------");
    }

    if (!radar.isConnected()) { Serial.println("[WARN] Comm error!"); delay(1000); }
}
