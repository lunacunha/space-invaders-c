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

#define TIMER 0
#define FREQ 60

extern uint8_t scancode;
extern int timer_counter;
extern vbe_mode_info_t mode_info;

// With characters test
/*static xpm_map_t letters[] = {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};*/

static xpm_map_t numbers_spc[] = {
    n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, point, points, bar, sub
};

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

int (waiting_ESC)() {
    int ipc_st;
    uint8_t irq_set;
    message msg;

    if ((kb_subscribe_int)(&irq_set)) {
        printf("Error subscribing keyboard\n");
        return 1;
    }

    while (scancode != KB_BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_st)) {   // Receber a mensagem
        printf("Error: driver_receive failed with: %d", ipc_st);
        continue;
        }
    
        if (is_ipc_notify(ipc_st)) {  // Verificar se a mensagem é uma notificação
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:  
            if (msg.m_notify.interrupts & BIT(irq_set)) {   // Verificar se a interrupção é do teclado
                kbc_ih();   // Tratar a interrupção
                break;
            }   
            break;
            default:
            break;
        }
        }
    }
    if ((kb_unsubscribe_int)()) {
        printf("Error unsubscribing keyboard\n");
        return 1;
    }
    
    return 0;
}


int (proj_main_loop)(int argc, char *argv[]) {

    if (timer_set_frequency(TIMER, FREQ)) return 1;
    int time_elapsed = 0;
    int n = 10;

    uint16_t mode = 0x14C;
    uint16_t x = 350;
    if (set_frame_buffer(mode)) return 1;   
    if (set_graphical_mode(mode)) return 1;
    if (print_xpm(ship, x, 600) != 0) return 1;

    int ipc_st;
    uint8_t irq_set_keyboard, irq_set_timer;
    message msg;

    if ((kb_subscribe_int)(&irq_set_keyboard)) {  
        printf("Error subscribing keyboard\n");
        return 1;
    }

    if ((timer_subscribe_int)(&irq_set_timer)) {  // timer
        printf("Error subscribing timer\n");
        return 1;
    }

    while (scancode != KB_BREAK_ESC && time_elapsed < n) {
        if (driver_receive(ANY, &msg, &ipc_st)) {   
            printf("Error: driver_receive failed with: %d", ipc_st);
            continue;
        }

        if (is_ipc_notify(ipc_st)) {  
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:  
                    if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {  
                        kbc_ih();   

                        bool is_break = scancode & KB_BREAK_CODE; 
                        int size = (scancode == KB_TWO_BYTES) ? 2 : 1;   

                        (kbd_print_scancode)(is_break, size, &scancode); 

                        if (!is_break) { 
                            if (scancode == 0x1e) { // 'A' key (make code)
                                x -= 30; // Mover para a esquerda
                                if (vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x000000)) return 1;
                                if (print_xpm(ship, x, 600) != 0) return 1;
                            } 
                            else if (scancode == 0x20) { // 'D' key (make code)
                                x += 30; // Mover para a direita
                                if (vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x000000)) return 1;
                                if (print_xpm(ship, x, 600) != 0) return 1; 
                            }
                        }

                        time_elapsed = 0;
                    }

                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) {   
                        timer_int_handler();  

                        if (timer_counter % 60 == 0) {
                            if (print_xpm(numbers_spc[9 - time_elapsed], 100, 100)) return 1; 
                            time_elapsed++;
                        }
                    }

                    break;

                default:
                    break;
            }
        }

    }


    if ((kb_unsubscribe_int)()) { 
        printf("Error unsubscribing keyboard\n");
        return 1;
    }

    if ((timer_unsubscribe_int)()) { 
        printf("Error unsubscribing timer\n");
        return 1;
    }

    //if (waiting_ESC()) return 1;

    if (vg_exit()) return 1;

    return 0;
}
