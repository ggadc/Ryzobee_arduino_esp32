#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

Ryzobee board;

static bool lastPressed = false;
static uint8_t page = 0;
static uint32_t lastPrintAt = 0;

void printPage()
{
    Serial.println();
    if (page == 0) {
        Serial.printf("Board: %s\n", board.maker.getBoardName().c_str());
        Serial.printf("Info: %s\n", board.maker.getBoardInfo().c_str());
    } else {
        Serial.println("Port summary:");
        Serial.print(board.maker.portsInfo().c_str());
    }
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    board.maker.begin(true, false, true, false);

    Serial.println();
    Serial.println("RootMaker legacy screen example");
    Serial.println("The refactored library no longer exposes the display/touch API.");
    Serial.println("This compatibility sketch prints board and port information to the serial monitor.");
    Serial.println("Release the onboard button to switch between pages.");

    printPage();
}

void loop(void)
{
    board.maker.btns.update(0);
    bool pressed = board.maker.btns[0].isPressed() != 0;

    if (!pressed && lastPressed) {
        page = (page + 1) % 2;
        printPage();
    }

    uint32_t now = millis();
    if (now - lastPrintAt >= 5000) {
        lastPrintAt = now;
        printPage();
    }

    lastPressed = pressed;
    board.maker.led.setHSV((now / 20) % 360, 100, 20);
    delay(10);
}
