# RAD60 mmWave Radar Example

[中文](./README_CN.md)

This example demonstrates how to use the AT6010 (RAD60) mmWave radar sensor on a Ryzobee board via UART. It applies default motion/breath detection parameters, and continuously reports multi-target detection data (distance, angle, velocity, ID). Communication is fixed at 115200 baud.

# Features

- Initialize RAD60 on any board expansion port via `begin(board, port, uart_num, baud)`
- Wake up ULP (ultra-low-power) radar if needed
- Apply default detection configuration (motion + breath)
- Real-time multi-target tracking (up to 2 targets)
- Vacancy / communication health monitoring

# Hardware Connection

| Rootmaker Port | RAD60 Module |
|----------------|-------------|
| SDA            | TX          |
| SCL            | RX          |
| VCC            | 3.3V        |
| GND            | GND         |

| Port  | SDA (TX)  | SCL (RX)  |
|-------|-----------|-----------|
| PORT1 | GPIO15    | GPIO16    |
| PORT2 | GPIO19    | GPIO20    |
| PORT3 | GPIO13    | GPIO14    |
| PORT4 | GPIO17    | GPIO18    |

# Dependencies

No additional third-party libraries are required. The RAD60 module uses the ESP-IDF native UART driver, matching the IDF `at6010` implementation.

# Usage

- Open `examples/ext_periph/Yoke_RAD60/Yoke_RAD60.ino`
- Build and upload
- Open the serial monitor at 115200 baud
- After startup (~3s), walk in front of the radar — target info will appear

# Multi-Port Usage

Create one instance per port (each radar needs its own UART):

```cpp
#include "Ryzobee.h"

Yoke_rad60 radarA, radarB;

void setup() {
    board.rootmaker.begin();
    radarA.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT1, UART_NUM_2, 115200);
    radarB.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT3, UART_NUM_1, 115200);
}

void loop() {
    radarA.update();
    radarB.update();
    if (radarA.isDetected()) {
        Serial.printf("PORT1: %d cm\n", radarA.getTargetDistance(0));
    }
    if (radarB.isDetected()) {
        Serial.printf("PORT3: %d cm\n", radarB.getTargetDistance(0));
    }
    delay(50);
}
```

# Notes

- The RAD60 module must be powered at 3.3V with sufficient current
- Each RAD60 requires its own UART (UART_NUM_1 = Serial1, UART_NUM_2 = Serial2)
- Communication baud rate is fixed at 115200
