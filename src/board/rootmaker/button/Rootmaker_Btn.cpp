#include "Rootmaker_btn.h"



Rootmaker_btn::Rootmaker_btn() {
    
}

Button_state_t Rootmaker_btn::getState() {
    return button.isPressed() ? BUTTON_PRESSED : BUTTON_RELEASED;
}

void Rootmaker_btn::init() {
    
}

bool Rootmaker_btn::btn_change() {
    button.read();
    return button.wasPressed() || button.wasReleased();
}
