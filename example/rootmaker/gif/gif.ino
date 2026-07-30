#include "Ryzobee.h"

Ryzobee board;

static uint8_t mode = 0;
static bool lastPressed = false;
static uint32_t lastAnimationAt = 0;
static uint16_t hue = 0;

void advanceMode()
{
    mode = (mode + 1) % 3;
    Serial.printf("Mode changed to %u\n", mode);
}

void renderMode()
{
    switch (mode) {
        case 0:
            board.maker.led.setColor(255, 0, 0);
            break;
        case 1:
            board.maker.led.setColor(0, 255, 0);
            break;
        default:
            board.maker.led.setHSV(hue, 100, 40);
            hue = (hue + 4) % 360;
            break;
    }
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    board.maker.begin(true, false, true, false);

    Serial.println();
    Serial.println("RootMaker legacy gif example");
    Serial.println("The refactored library no longer exposes the display GIF API.");
    Serial.println("This compatibility sketch keeps the folder buildable and uses the button to switch LED patterns instead.");
}

void loop(void)
{
    board.maker.btns.update(0);
    bool pressed = board.maker.btns[0].isPressed() != 0;

    if (!pressed && lastPressed) {
        advanceMode();
    }

    uint32_t now = millis();
    if (now - lastAnimationAt >= 30) {
        lastAnimationAt = now;
        renderMode();
    }

    lastPressed = pressed;
    delay(10);
}

