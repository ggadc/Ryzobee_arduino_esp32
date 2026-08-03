/*
 * Yoke_KEYW.ino — KEYW module example (key + LED)
 *
 * Note: The Yoke library does not yet provide a Yoke_keyw class.
 * This example accesses RS port pins directly via the Ryzobee board's
 * port data vector. SDA → key input (active LOW, internal pull-up),
 * SCL → LED output (PWM-capable).
 *
 * Hardware: KEYW module on a Rootmaker RS expansion port
 */

#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

// ==== LED control mode ====
#define LED_CONTROL (1)  // 0 = digital on/off, 1 = PWM brightness

Ryzobee board;

// ==== Port data (acquired from board at setup) ====
std::vector<uint8_t>* keywPort = nullptr;

volatile bool pressFlag = false;
bool ledState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 30;

void setup() {
    Serial.begin(115200);
    board.maker.begin();

    // ==== Resolve KEYW pins from RS port data ====
    // port vector layout: [type, used, invalid, pinCount, pin1(SDA), pin2(SCL)]
    keywPort = board.maker.useport("rs_port4");
    int keyPin = keywPort->at(PORT_PIN_START);      // SDA → key input
    int ledPin = keywPort->at(PORT_PIN_START + 1);  // SCL → LED output

    pinMode(keyPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    Serial.printf("KEYW ready: key=GPIO%d led=GPIO%d\n", keyPin, ledPin);

    attachInterrupt(digitalPinToInterrupt(keyPin), onKeyPress, FALLING);
}

void loop() {
    if (pressFlag) {
        unsigned long now = millis();
        if (now - lastDebounceTime >= debounceDelay) {
            int keyPin = keywPort->at(PORT_PIN_START);
            if (digitalRead(keyPin) == LOW) {
                ledState = !ledState;
                int ledPin = keywPort->at(PORT_PIN_START + 1);
#if LED_CONTROL
                analogWrite(ledPin, ledState ? 255 : 0);
#else
                digitalWrite(ledPin, ledState ? HIGH : LOW);
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
