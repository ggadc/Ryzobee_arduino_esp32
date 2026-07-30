/*
 * Yoke_RGBW.ino — RGBW NeoPixel LED example
 */

#include "Ryzobee.h"

Ryzobee board(RYZOBEE_ROOTMAKER);
Yoke_rgbw rgbw;

void setup(void) {
    Serial.begin(115200);
    board.rootmaker.begin();
    rgbw.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT4, 1, 3);
    Serial.println("RGBW ready on PORT4");
}

void loop(void) {
    rgbw.setPixeColor(255, 0, 0, 0, 0); rgbw.setPixeColor(0, 255, 0, 0, 1); rgbw.setPixeColor(0, 0, 255, 0, 2);
    delay(1000); rgbw.setColor(0);
    rgbw.setPixeColor(0x8B3A3A, 0); rgbw.setPixeColor(0xFFFF00, 1); rgbw.setPixeColor(0x228B22, 2);
    delay(1000); rgbw.setColor(0);
    rgbw.setPixeColor(255, 0, 0, 0, 0, 200); rgbw.setPixeColor(255, 0, 0, 0, 0, 0);
    rgbw.setPixeColor(0, 255, 0, 0, 1, 200); rgbw.setPixeColor(0, 255, 0, 0, 1, 0);
    rgbw.setPixeColor(0, 0, 255, 0, 2, 200); rgbw.setPixeColor(0, 0, 255, 0, 2, 0);
    delay(1000); rgbw.setColor(0);
    rgbw.setColor(255, 0, 0, 0); rgbw.setColor(0, 255, 0, 0); rgbw.setColor(0, 0, 255, 0);
    delay(1000); rgbw.setColor(0);
    rgbw.setColor(0x8B3A3A); rgbw.setColor(0xFFFF00); rgbw.setColor(0x228B22);
    delay(1000); rgbw.setColor(0);
    rgbw.setColor(255, 0, 0, 0, 200); rgbw.setColor(255, 0, 0, 0, 0);
    rgbw.setColor(0, 255, 0, 0, 200); rgbw.setColor(0, 255, 0, 0, 0);
    rgbw.setColor(0, 0, 255, 0, 200); rgbw.setColor(0, 0, 255, 0, 0);
    delay(1000); rgbw.setColor(0);
}
