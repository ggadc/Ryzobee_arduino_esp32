#include "Ryzobee.h"
// #include "Rootmaker_Lcd.h"

// #include "boards.h"

// #include "rootmaker.h"

#define CUBE_WIDTH    40

// Rootmaker_Lcd display;

// RootMaker display;

Ryzobee board(RYZOBEE_ROOTMAKER);


long randNumber_x = (SCREEN_WIDTH - CUBE_WIDTH) / 2;
long randNumber_y = (SCREEN_WIDTH - CUBE_WIDTH) / 2;

void setup(void)
{
  Serial.begin(115200);

  randomSeed(analogRead(A0));

  board.begin();  // 初始化板载硬件 (包括 LCD)

  board.rootmaker.lcd.setTextSize((std::max(board.rootmaker.lcd.width(), board.rootmaker.lcd.height()) + 255) >> 8);

  board.rootmaker.lcd.fillScreen(TFT_BLACK);

  board.rootmaker.lcd.fillRect(randNumber_x, randNumber_y, CUBE_WIDTH, CUBE_WIDTH, 0xf800);// color: red
  board.rootmaker.lcd.drawString("Please click the cube", 60, 230);
}

uint32_t count = ~0;
void loop(void)
{
  int32_t x, y;
  if (board.rootmaker.lcd.getTouch(&x, &y)) {

    if (x >= randNumber_x && x <= randNumber_x + 40 && y >= randNumber_y && y <= randNumber_y + 40) {
      randNumber_x = random(0, SCREEN_WIDTH - CUBE_WIDTH);
      randNumber_y = random(0, SCREEN_WIDTH - CUBE_WIDTH);
      board.rootmaker.lcd.clearDisplay();
      board.rootmaker.lcd.fillRect(randNumber_x, randNumber_y, CUBE_WIDTH, CUBE_WIDTH, 0xf800);// color: red
      board.rootmaker.lcd.drawString("Please click the cube", 60, 230);

    } else {
      if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_WIDTH) {
        board.rootmaker.lcd.fillRect(x-2, y-2, 1, 1, 0x07E0);
      }
    }
  }
}
