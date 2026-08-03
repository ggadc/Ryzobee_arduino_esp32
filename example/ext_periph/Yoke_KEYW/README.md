# KEYW Module Example

[中文](./README_CN.md)

This example demonstrates how to use the KEYW expansion module (key + LED) on a Ryzobee board. The Yoke library does not yet provide a `Yoke_keyw` class, so this example accesses RS port pins directly via the board's port data.

SDA → key input (active LOW, internal pull-up), SCL → LED output (PWM-capable via `analogWrite`).

# Features

- Resolve pins from port data: `board.maker.useport("rs_portN")`
- Falling-edge interrupt with 30 ms software debounce
- Toggle LED on each key press
- Switchable digital on/off or PWM brightness mode (`LED_CONTROL` define)

# Hardware

| Expansion Port | KEYW Module |
|----------------|-------------|
| SDA            | Key input   |
| SCL            | LED output  |

Rootmaker GPIO mapping:

| Port  | SDA (Key) | SCL (LED) |
|-------|-----------|-----------|
| PORT1 | GPIO15    | GPIO16    |
| PORT2 | GPIO19    | GPIO20    |
| PORT3 | GPIO13    | GPIO14    |
| PORT4 | GPIO17    | GPIO18    |

# Dependencies

| Library | Description |
|---------|-------------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | Board support (Rootmaker) |

No Yoke library required (raw GPIO access).

# Quick Start

```cpp
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

Ryzobee board;
std::vector<uint8_t>* keywPort = nullptr;

void setup() {
    board.maker.begin();
    keywPort = board.maker.useport("rs_port1");
    int keyPin = keywPort->at(PORT_PIN_START);      // SDA
    int ledPin = keywPort->at(PORT_PIN_START + 1);  // SCL
    pinMode(keyPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(keyPin), onKeyPress, FALLING);
}
```

# Notes

- Key is active LOW with internal pull-up — no external pull-up needed
- PWM mode (`LED_CONTROL=1`) uses `analogWrite`; ensure the SCL pin supports PWM
- A dedicated `Yoke_keyw` class may be added to the Yoke library in the future
