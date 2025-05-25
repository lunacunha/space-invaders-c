#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "controllers/keyboard/kb_interrupts.h"
#include "controllers/graphics/graphic.h"
#include "xpm/menu/title.h"
#include "xpm/menu/options.h"
#include "xpm/characters/letter_char.h"
#include "xpm/characters/number_char.h"
#include "xpm/characters/special_char.h"
#include "xpm/game/player.h"
#include "xpm/game/aliens.h"
#include "xpm/messages.h"
#include "confs.h"
#include "game/ship/ship.h"
#include "game/game_state.h"
#include "game/menu/menu.h"

#define TIMER 0
#define FREQ 60

extern uint16_t x;

extern uint8_t scancode;
extern int timer_counter;
extern vbe_mode_info_t mode_info;
uint8_t irq_set_keyboard, irq_set_timer;


// With characters test
/*static xpm_map_t letters[] = {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

static xpm_map_t numbers_spc[] = {
    n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, point, points, bar, sub
};*/

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}



int init_game() {
    if (timer_set_frequency(TIMER, FREQ)) return 1;
    if (set_frame_buffer(MODE)) return 1;   
    if (set_graphical_mode(MODE)) return 1;
    if ((kb_subscribe_int)(&irq_set_keyboard)) return 1;
    if ((timer_subscribe_int)(&irq_set_timer)) return 1;
    menu_init();
    return 0;
}

int close_game() {
    if (vg_exit()) return 1;
    if ((kb_unsubscribe_int)()) return 1;
    if ((timer_unsubscribe_int)()) return 1;
    return 0;
}

int menu_handler() {
    message msg;
    int ipc_status;
    while (scancode != KB_BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_status)) {
            printf("Error: driver_receive failed");
            continue;
        }
        
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // Teclado
                    if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {
                        kbc_ih();
                        menu_handle_input();
                    }
                    // Timer
                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) {
                        timer_int_handler();
                        MenuState state = menu_get_state();
                        
                        if (state == MENU_START) {
                            if (draw_ship(x) != 0) return 1;
                            init_bullets();
                            init_enemies();
                            if (game_state() != 0) return 1;
                        } else if (state == MENU_EXIT) {
                            return close_game();
                        } else {
                            menu_render();
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


int (proj_main_loop)(int argc, char *argv[]) {
    if (init_game() != 0) return close_game();
    if (menu_handler() != 0) return 1;
    return close_game();
}

