#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_lcd1d54c/yoke_lcd1d54c.hpp>

Ryzobee board;
Yoke_lcd1d54c lcd(&board.maker.getWire(), board.maker.getI2CMutex());

#define CUBE_WIDTH 40

static long cubeX = (YOKE_LCD1D54C_SIZE - CUBE_WIDTH) / 2;
static long cubeY = (YOKE_LCD1D54C_SIZE - CUBE_WIDTH) / 2;

/**
 * @brief 绘制红色方块和点击提示。
 */
static void drawScene()
{
    lcd.clear();
    lcd.fillRect(cubeX, cubeY, CUBE_WIDTH, CUBE_WIDTH, TFT_RED);
    lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    lcd.drawString("Please click the cube", 60, 220);
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    randomSeed(micros());

    board.maker.begin(false, false, false, false);

    if (lcd.begin(board.maker.useport("f_bus")) != YOKE_PORT_ERROR_NONE) {
        Serial.println("LCD is not available");
        return;
    }

    lcd.setBrightness(255);
    lcd.setTextSize((std::max(lcd.width(), lcd.height()) + 255) >> 8);
    drawScene();

    Serial.println("Yoke LCD1D54C touch example");
}

void loop(void)
{
    if (!lcd.isInitialized()) {
        delay(100);
        return;
    }

    uint16_t x = 0;
    uint16_t y = 0;
    if (lcd.getTouch(&x, &y)) {
        if (x >= cubeX && x < cubeX + CUBE_WIDTH && y >= cubeY && y < cubeY + CUBE_WIDTH) {
            cubeX = random(0, YOKE_LCD1D54C_SIZE - CUBE_WIDTH + 1);
            cubeY = random(0, YOKE_LCD1D54C_SIZE - CUBE_WIDTH + 1);
            drawScene();
        } else if (x < YOKE_LCD1D54C_SIZE && y < YOKE_LCD1D54C_SIZE) {
            lcd.fillRect(x, y, 3, 3, TFT_GREEN);
        }
    }

    delay(10);
}
