#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

Ryzobee board;

#define CUBE_WIDTH 40

static long cubeX = (SCREEN_WIDTH - CUBE_WIDTH) / 2;
static long cubeY = (SCREEN_WIDTH - CUBE_WIDTH) / 2;

static void drawScene()
{
    board.maker.lcd->clear();
    board.maker.lcd->fillRect(cubeX, cubeY, CUBE_WIDTH, CUBE_WIDTH, TFT_RED);
    board.maker.lcd->setTextColor(TFT_WHITE, TFT_BLACK);
    board.maker.lcd->drawString("Please click the cube", 60, 220);
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    randomSeed(micros());

    board.maker.begin(false, false, false, false, true);

    if (board.maker.lcd == nullptr) {
        Serial.println("LCD is not available");
        return;
    }

    board.maker.lcd->setBrightness(255);
    board.maker.lcd->setTextSize((std::max(board.maker.lcd->width(), board.maker.lcd->height()) + 255) >> 8);
    drawScene();

    Serial.println("RootMaker LCD touch example");
}

void loop(void)
{
    if (board.maker.lcd == nullptr) {
        delay(100);
        return;
    }

    uint16_t x = 0;
    uint16_t y = 0;
    if (board.maker.lcd->getTouch(&x, &y)) {
        if (x >= cubeX && x <= cubeX + CUBE_WIDTH && y >= cubeY && y <= cubeY + CUBE_WIDTH) {
            cubeX = random(0, SCREEN_WIDTH - CUBE_WIDTH);
            cubeY = random(0, SCREEN_WIDTH - CUBE_WIDTH);
            drawScene();
        } else if (x < SCREEN_WIDTH && y < SCREEN_WIDTH) {
            board.maker.lcd->fillRect(x, y, 3, 3, TFT_GREEN);
        }
    }

    delay(10);
}
