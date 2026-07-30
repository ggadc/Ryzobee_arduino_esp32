# RGBW LED Example

[中文](./README_CN.md)

This example demonstrates how to control an RGBW NeoPixel LED strip using the RGBW expansion module on a Ryzobee board. It cycles through per-pixel and all-pixel color patterns including packed 32-bit colors and timed transitions.

# Features

- Initialize RGBW on any board expansion port via `begin(board, port, brt, num)`
- Configurable number of LEDs and brightness
- Per-pixel color control (RGBW components or packed 32-bit)
- All-pixel fill with or without delay
- Looping color demo

# Hardware Connection

| Rootmaker Port | NeoPixel Strip |
|----------------|----------------|
| SDA            | Data line (DIN)|

| Port  | SDA (DIN) |
|-------|-----------|
| PORT1 | GPIO15    |
| PORT2 | GPIO19    |
| PORT3 | GPIO13    |
| PORT4 | GPIO17    |

# Dependencies

| Name | Version | Description |
|---|---|---|
| [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) | >= v1.15.3 | Arduino library for single-wire RGB/RGBW LEDs |

# Usage

- Open `examples/ext_periph/Yoke_RGBW/Yoke_RGBW.ino`
- Build and upload
- The LED strip will cycle through various color patterns

# Multi-Port Usage

Create one instance per port:

```cpp
#include "Ryzobee.h"

Yoke_rgbw strip1, strip2;

void setup() {
    board.rootmaker.begin();
    strip1.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT1, 50, 3);
    strip2.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT3, 100, 5);
}

void loop() {
    strip1.setColor(255, 0, 0, 0);   // PORT1: red
    strip2.setColor(0, 0, 255, 0);   // PORT3: blue
    delay(500);
    strip1.setColor(0);
    strip2.setColor(0);
    delay(500);
}
```

# Notes

- The NeoPixel data line expects 3.3V logic; a level shifter may be needed for 5V strips
