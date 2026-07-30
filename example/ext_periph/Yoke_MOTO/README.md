# MOTO Module Example

[中文](./README_CN.md)

This example demonstrates how to drive a DC motor using the MOTO expansion module on a Ryzobee board. The module controls an H-bridge motor driver via two GPIO pins (IN1/IN2), supporting forward, reverse, stop, and PWM speed control. The module is decoupled from any specific board — the same code works on Rootmaker and future Ryzobee models.

# Features

- Initialize MOTO on any Ryzobee board expansion port via `begin(board, port)`
- Forward / reverse / stop control
- Independent PWM speed and direction control
- Ramp-up / ramp-down demonstration

# Hardware Connection

| Expansion Port | Motor Driver |
|----------------|-------------|
| SDA            | IN1         |
| SCL            | IN2         |

Rootmaker port-to-GPIO mapping:

| Port  | SDA (IN1) | SCL (IN2) |
|-------|-----------|-----------|
| PORT1 | GPIO15    | GPIO16    |
| PORT2 | GPIO19    | GPIO20    |
| PORT3 | GPIO13    | GPIO14    |
| PORT4 | GPIO17    | GPIO18    |

# Dependencies

No additional third-party libraries are required.

# Usage

- Open `examples/ext_periph/Yoke_MOTO/Yoke_MOTO.ino`
- Build and upload to a Rootmaker board
- The motor will cycle through forward, reverse, and speed ramps

# Multi-Port Usage

Create one independent instance per port:

```cpp
#include "Ryzobee.h"

Yoke_moto motorA, motorB;

void setup() {
    board.rootmaker.begin();
    motorA.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT2);
    motorB.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT3);
}

void loop() {
    motorA.motorForward(200);
    motorB.motorBackward(150);
    delay(1000);
    motorA.motorStop();
    motorB.motorStop();
    delay(500);
}
```

# Notes

- Ensure the motor driver and motor are properly powered
- `motorStop()` sets both IN1 and IN2 LOW (coast stop); for brake stop, set both HIGH
- `setSpeed()` only changes the PWM duty cycle for the current direction; it does not change direction
