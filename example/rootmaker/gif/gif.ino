#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <AnimatedGIF.h>

#include "Ryzo_20fps.h"
#include "Ryzo_25fps.h"

Ryzobee board;

AnimatedGIF gif;

#define BUFFER_SIZE 240

static uint16_t lineBuffer[BUFFER_SIZE];
static uint8_t currentGif = 0;
static bool lastPressed = false;

static void drawGifLine(GIFDRAW* draw)
{
    if (board.maker.lcd == nullptr) {
        return;
    }

    uint16_t* palette = draw->pPalette;
    uint8_t* source = draw->pPixels;
    int width = draw->iWidth;
    int y = draw->iY + draw->y;

    if (y >= board.maker.lcd->height() || draw->iX >= board.maker.lcd->width()) {
        return;
    }

    if (draw->iX + width > board.maker.lcd->width()) {
        width = board.maker.lcd->width() - draw->iX;
    }
    if (width <= 0) {
        return;
    }

    if (draw->ucDisposalMethod == 2) {
        for (int index = 0; index < width; ++index) {
            if (source[index] == draw->ucTransparent) {
                source[index] = draw->ucBackground;
            }
        }
        draw->ucHasTransparency = 0;
    }

    if (draw->ucHasTransparency) {
        int start = 0;
        while (start < width) {
            while (start < width && source[start] == draw->ucTransparent) {
                ++start;
            }

            int count = 0;
            while (start + count < width && source[start + count] != draw->ucTransparent && count < BUFFER_SIZE) {
                lineBuffer[count] = palette[source[start + count]];
                ++count;
            }

            if (count > 0) {
                board.maker.lcd->pushImage(draw->iX + start, y, count, 1, lineBuffer);
                start += count;
            }
        }
        return;
    }

    for (int index = 0; index < width; ++index) {
        lineBuffer[index] = palette[source[index]];
    }

    board.maker.lcd->pushImage(draw->iX, y, width, 1, lineBuffer);
}

static bool openCurrentGif()
{
    const uint8_t* gifData = currentGif == 0 ? Ryzo_20fps : Ryzo_25fps;
    uint32_t gifSize = currentGif == 0 ? sizeof(Ryzo_20fps) : sizeof(Ryzo_25fps);

    if (board.maker.lcd != nullptr) {
        board.maker.lcd->fillScreen(TFT_BLACK);
    }

    if (!gif.open((uint8_t*)gifData, gifSize, drawGifLine)) {
        Serial.printf("Failed to open GIF %u\n", currentGif);
        return false;
    }

    Serial.printf("Playing GIF %u: %d x %d\n", currentGif, gif.getCanvasWidth(), gif.getCanvasHeight());
    return true;
}

void setup(void)
{
    Serial.begin(115200);
    delay(200);

    board.maker.begin(false, false, true, false, true);

    if (board.maker.lcd == nullptr) {
        Serial.println("LCD is not available");
        return;
    }

    board.maker.lcd->setBrightness(255);
    board.maker.lcd->fillScreen(TFT_BLACK);
    gif.begin(BIG_ENDIAN_PIXELS);
    openCurrentGif();

    Serial.println();
    Serial.println("RootMaker GIF example");
    Serial.println("Release the onboard button to switch to the next animation.");
}

void loop(void)
{
    if (board.maker.lcd == nullptr) {
        delay(100);
        return;
    }

    bool pressed = board.maker.btns[0].isPressed() != 0;

    if (!pressed && lastPressed) {
        gif.close();
        currentGif = (currentGif + 1) % 2;
        openCurrentGif();
    }

    board.maker.lcd->startWrite();
    bool hasFrame = gif.playFrame(true, nullptr);
    board.maker.lcd->endWrite();

    if (!hasFrame) {
        gif.reset();
    }

    lastPressed = pressed;
    delay(1);
}

