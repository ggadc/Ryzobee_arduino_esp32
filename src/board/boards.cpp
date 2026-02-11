#include "boards.h"
#include "rootmaker/rootmaker_pin.h"

using namespace lgfx::v1;


Ryzobee::Ryzobee(Ryzobee_board_t board) {
    _board = board;
    setPanel(nullptr);
}

bool Ryzobee::init_impl(bool use_reset, bool use_clear) {
    Panel_Device* panel_ptr = nullptr;
    switch (_board) {
      case RYZOBEE_ROOTMAKER: {
        rootmaker.begin(true, true, true, true);
        panel_ptr = rootmaker.lcd.panel();
        break;
      }
      default: {
        break;
      }
    }

    if (panel_ptr == nullptr) {
      assert(0);
    }

    setPanel(panel_ptr);
    return LGFX_Device::init_impl(false, use_clear);
}

