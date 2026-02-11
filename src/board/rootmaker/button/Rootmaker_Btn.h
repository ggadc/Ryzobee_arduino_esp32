#pragma once

#include "Button.h"

#include "../rootmaker_pin.h"

#define BUTN_PIN    BTN_PIN
#define DEBOUNCE_MS 10

enum Button_state_t {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED  = 1,
};

class Rootmaker_btn {
    public:

        Button button = Button(BUTN_PIN, true, DEBOUNCE_MS);

        Rootmaker_btn();
        void init();
        Button_state_t getState();
        bool btn_change();
};
