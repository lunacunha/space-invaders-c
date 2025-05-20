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


int (proj_main_loop)(int argc, char *argv[]){
    // ------------- Test keyboard and timer -------------------------
    /* 
    // Set timer frequency
    if (timer_set_frequency(TIMER, FREQ)) return 1;

    int ipc_st;
    uint8_t irq_set_keyboard, irq_set_timer;
    message msg;
    int time_elapsed = 0;
    int n = 10;

    // Subscrever as interrupções
    if ((kb_subscribe_int)(&irq_set_keyboard)) {  // teclado
        printf("Error subscribing keyboard\n");
        return 1;
    }

    if ((timer_subscribe_int)(&irq_set_timer)) {  // timer
        printf("Error subscribing timer\n");
        return 1;
    }

    // Ciclo de interrupções
    while (scancode != KB_BREAK_ESC && time_elapsed < n) {
        if (driver_receive(ANY, &msg, &ipc_st)) {   // Receber a mensagem
        printf("Error: driver_receive failed with: %d", ipc_st);
        continue;
        }

        if (is_ipc_notify(ipc_st)) {  // Verificar se a mensagem é uma notificação
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:  
            if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {   // Verificar se a interrupção é do teclado
                kbc_ih();   // Tratar a interrupção

                bool is_break = !(scancode & KB_MAKE_CODE);  // Verificar se é um break code
                int size = (scancode == KB_TWO_BYTES) ? 2 : 1;   // Verificar o tamanho do scancode

                (kbd_print_scancode)(is_break, size, &scancode);  // Imprimir o scancode

                time_elapsed = 0;  // Resetar o tempo
            }

            if (msg.m_notify.interrupts & BIT(irq_set_timer)) {   // Verificar se a interrupção é do timer
                timer_int_handler();  // Incrementar o contador de interrupções do timer

                if (timer_counter % 60 == 0) {
                time_elapsed++;  // Incrementar o tempo
                printf("Time elapsed: %d\n", time_elapsed);
                }
            }
            break;
            default:
            break;
        }
        }
    }

    // Cancelar a subscrição das interrupções
    if ((kb_unsubscribe_int)()) {  // teclado
        printf("Error unsubscribing keyboard\n");
        return 1;
    }

    if ((timer_unsubscribe_int)()) {  // timer
        printf("Error unsubscribing timer\n");
        return 1;
    }*/


    // -------------- Test Graphics with Sprites -----------------------

    // Initial Parameters 
    uint16_t mode = 0x14C;
    uint16_t x = 350;
    uint16_t y = 50;

    if (set_frame_buffer(mode)) return 1;   

    if (set_graphical_mode(mode)) return 1;


    // Test Rectangle 
    /*
    uint16_t width = 100;
    uint16_t height = 150;
    uint32_t color = 0x13;
    uint32_t n_color;
    if (norm_color(color, &n_color)) return 1;

    if (vg_draw_rectangle(x, y, width, height, n_color)) return 1;*/


    // Test menu 
    if (print_xpm(title, x, y) != 0) return 1;
    x += 100;
    if (print_xpm(start2, x, 400) != 0) return 1;
    if (print_xpm(scores1, x, 500) != 0) return 1;
    if (print_xpm(quit1, x, 600) != 0) return 1;


    //  Test Characters 
    /*for (int i = 0; i < 26; i++) {
        if (print_xpm(letters[i], x + i * 25, 500) != 0) return 1;
    }
    for (int j = 0; j < 14; j++) {
        if (print_xpm(numbers_spc[j], x + j * 25, 600) != 0) return 1;
    }*/


    // Test game 
    /*if (print_xpm(Alien1, x + 100, 500) != 0) return 1;
    if (print_xpm(Alien2, x + 200, 500) != 0) return 1;
    if (print_xpm(Alien3, x + 300, 500) != 0) return 1;
    if (print_xpm(Alien4, x + 400, 500) != 0) return 1;

    if (print_xpm(ship, x + 100, 600) != 0) return 1;
    if (print_xpm(bullet, x + 200, 600) != 0) return 1;
    if (print_xpm(enemy_bullet, x + 300, 600) != 0) return 1;*/


    //  Test messages 
    /*if (print_xpm(message1, x, 400) != 0) return 1;
    if (print_xpm(message2, x, 450) != 0) return 1;
    if (print_xpm(message3, x, 500) != 0) return 1;
    if (print_xpm(message4, x, 550) != 0) return 1;
    if (print_xpm(message5, x, 600) != 0) return 1;*/


    if (waiting_ESC()) return 1;

    if (vg_exit()) return 1;

    return 0;
}
