# Yoke LCD1D54C Display and Touch Example

Uses a 240×240 ST7789 display and CST816T touch controller.

## Features

- Initializes the display module and touch input
- Draws a clickable red square on the screen
- Moves the square to a random position when tapped
- Draws green pixel markers near the touch point when other areas are tapped

## Usage

Install Ryzobee, Yoke and LovyanGFX, then build and upload `Yoke_LCD1D54C.ino`.
The example uses `lcd.begin(board.maker.useport("f_bus"))`, which returns `YOKE_PORT_ERROR_NONE` on successful initialization.
LCD1D54C is a detachable peripheral created and initialized separately by the example; do not use other peripherals on the same F_BUS at the same time.
Touch input reuses `getWire()` and `getI2CMutex()`; keep the board object alive longer than the lcd object.

| Signal | F_BUS Pin | RootMaker GPIO |
| --- | --- | --- |
| LCD SCK | 11 | 12 |
| LCD RST | 13 | 10 |
| LCD SDA/MOSI | 14 | 9 |
| LCD BL | 16 | 7 |
| LCD CS | 18 | 5 |
| LCD RS/DC | 20 | 3 |
| TP RST | 21 | 2 |
| TP INT | 22 | 1 |
| TP SDA | 23 | 41 |
| TP SCL | 24 | 40 |

`lcd` inherits the LovyanGFX graphics interface and supports `setBrightness()`, `fillRect()`, `drawString()` and `getTouch(&x, &y)`; `lcd.end()` turns off the backlight and releases the port.
`board.maker.begin()` initializes onboard peripherals only; its four-parameter version controls the LED, sensor, button and battery monitoring, in that order. The display driver is provided by Yoke.
