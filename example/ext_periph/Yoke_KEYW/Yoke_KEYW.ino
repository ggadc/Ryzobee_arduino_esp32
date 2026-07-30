/*
 * Yoke_KEYW.ino — KEYW module example
 *
 * Hardware: KEYW module on a Rootmaker expansion port
 *   - SDA → key input (active LOW, internal pull-up)
 *   - SCL → LED output (PWM-capable)
 */

#include "Ryzobee.h"

#define LED_CONTROL (1)  // 0 = digital on/off, 1 = PWM brightness

Ryzobee board(RYZOBEE_ROOTMAKER);
Yoke_keyw keyw;

volatile bool pressFlag = false;
bool ledState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 30;

void setup() {
    Serial.begin(115200);
    board.rootmaker.begin();

    keyw.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT1);

    Serial.printf("KEYW ready: key=GPIO%d led=GPIO%d\n",
                  keyw.getKeyPin(), keyw.getLedPin());

    attachInterrupt(digitalPinToInterrupt(keyw.getKeyPin()), onKeyPress, FALLING);
    digitalWrite(keyw.getLedPin(), LOW);
}

void loop() {
    if (pressFlag) {
        unsigned long now = millis();
        if (now - lastDebounceTime >= debounceDelay) {
            if (digitalRead(keyw.getKeyPin()) == LOW) {
                ledState = !ledState;
#if LED_CONTROL
                keyw.setLedBrightness(ledState ? 255 : 0);
#else
                keyw.setLedState(ledState);
#endif
            }
            pressFlag = false;
        }
    }
}

void onKeyPress() {
    pressFlag = true;
    lastDebounceTime = millis();
}
