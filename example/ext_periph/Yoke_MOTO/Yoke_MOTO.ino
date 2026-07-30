/*
 * Yoke_MOTO.ino — Motor driver module example
 */

#include "Ryzobee.h"

Ryzobee board(RYZOBEE_ROOTMAKER);
Yoke_moto moto;

void setup() {
    Serial.begin(115200);
    board.rootmaker.begin();
    moto.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT4);
    Serial.println("MOTO ready on PORT4");
}

void loop() {
    moto.motorForward(255); delay(2000);
    moto.motorStop();       delay(1000);
    moto.motorBackward(255); delay(2000);
    moto.motorStop();       delay(1000);
    moto.setDirection(true);
    for (int i = 0; i < 255; i++) { moto.setSpeed(i); delay(10); }
    moto.setDirection(false);
    for (int i = 0; i < 255; i++) { moto.setSpeed(i); delay(10); }
}
