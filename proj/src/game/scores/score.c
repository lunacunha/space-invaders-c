#include "score.h"

extern uint8_t scancode;
extern uint8_t irq_set_keyboard;

int score_state() {
    int ipc_status;
    message msg;

    while (scancode != KB_BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_status)) {
            printf("Error: driver_receive failed with: %d", ipc_status);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // keyboard
                    if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {
                        kbc_ih();
                        
                        if (scancode == KB_ENTER || scancode == KB_BREAK_ESC) {
                            return 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}
