#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_lcd1d54c/yoke_lcd1d54c_gif.hpp>

#include "Ryzo_20fps.h"
#include "Ryzo_25fps.h"

Ryzobee board;
Yoke_lcd1d54c lcd(&board.maker.getWire(), board.maker.getI2CMutex());

Yoke_lcd1d54c_gif player(lcd);

static uint8_t currentGif = 0;
static bool lastPressed = false;

static bool openCurrentGif()
{
    const uint8_t* gifData = currentGif == 0 ? Ryzo_20fps : Ryzo_25fps;
    uint32_t gifSize = currentGif == 0 ? sizeof(Ryzo_20fps) : sizeof(Ryzo_25fps);

    if (!player.open(gifData, gifSize)) {
        Serial.printf("Failed to open GIF %u\n", currentGif);
        return false;
    }

    Serial.printf("Playing GIF %u: %d x %d\n", currentGif, player.width(), player.height());
    return true;
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    board.maker.begin(false, false, true, false);

    if (lcd.begin(board.maker.useport("f_bus")) != YOKE_PORT_ERROR_NONE) {
        Serial.println("LCD is not available");
        return;
    }

    lcd.setBrightness(255);
    lcd.fillScreen(TFT_BLACK);
    openCurrentGif();

    Serial.println();
    Serial.println("RootMaker GIF example");
    Serial.println("Release the onboard button to switch to the next animation.");
}

void loop(void)
{
    if (!lcd.isInitialized()) {
        delay(100);
        return;
    }

    bool pressed = board.maker.btns[0].isPressed() != 0;

    if (!pressed && lastPressed) {
        currentGif = (currentGif + 1) % 2;
        openCurrentGif();
    }

    player.update();

    lastPressed = pressed;
    delay(1);
}

