#include "Rootmaker_led.h"

Rootmaker_led::Rootmaker_led() {
    
}

void Rootmaker_led::init(void) {
    strip.begin();
    strip.setBrightness(100);
    strip.show();
}