# KEYW Module Example

[中文](./README_CN.md)

This example demonstrates how to use the KEYW expansion module (key + LED) on a Ryzobee board. The module resolves pins automatically via `begin(board, port)`. The key input uses an internal pull-up with falling-edge interrupt and software debounce, while the LED supports digital on/off or PWM brightness control. The module is decoupled from any specific board — the same code works on Rootmaker and future Ryzobee models.

# Features

- Initialize KEYW on any Ryzobee board expansion port via `begin(board, port)`
- Detect key press with software debounce (30 ms)
- Toggle LED state on each press
- Switch between digital on/off and PWM brightness modes

# Hardware Connection

| Expansion Port | KEYW Module |
|----------------|-------------|
| SDA            | Key input   |
| SCL            | LED output  |

Rootmaker port-to-GPIO mapping:

| Port  | SDA (Key) | SCL (LED) |
|-------|-----------|-----------|
| PORT1 | GPIO15    | GPIO16    |
| PORT2 | GPIO19    | GPIO20    |
| PORT3 | GPIO13    | GPIO14    |
| PORT4 | GPIO17    | GPIO18    |

# Dependencies

No additional third-party libraries are required.

# Usage

- Open `examples/ext_periph/Yoke_KEYW/Yoke_KEYW.ino`
- Build and upload to a Rootmaker board
- Open the serial monitor at 115200 baud
- Press the key — the LED should toggle

# Multi-Port Usage

Create one independent instance per port:

```cpp
#include "Ryzobee.h"

Yoke_keyw k2, k4;

void setup() {
    board.rootmaker.begin();
    k2.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT2);
    k4.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT4);
}

void loop() {
    if (k2.readKeyState()) k2.setLedBrightness(128);
    if (k4.readKeyState()) k4.setLedState(true);
}
```

# Notes

- The key input is active LOW; an internal pull-up resistor is enabled — no external pull-up needed
- PWM mode (`LED_CONTROL = 1`) uses `analogWrite`; ensure the target SCL pin supports PWM
- If the key is unresponsive, check the wiring and port selection
