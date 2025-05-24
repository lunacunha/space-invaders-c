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

#define TIMER 0
#define FREQ 60
#define MAX_BULLETS 10 

typedef struct {
    int x;         
    int y;          
    bool active;    
} Bullet;

Bullet bullets[MAX_BULLETS];

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

int init_game() {
    if (timer_set_frequency(TIMER, FREQ)) return 1;
    if (set_frame_buffer(MODE)) return 1;   
    if (set_graphical_mode(MODE)) return 1;
    if ((kb_subscribe_int)(&irq_set_keyboard)) return 1;
    if ((timer_subscribe_int)(&irq_set_timer)) return 1;
    return 0;
}

int close_game() {
    if (vg_exit()) return 1;
    if ((kb_unsubscribe_int)()) return 1;
    if ((timer_unsubscribe_int)()) return 1;
    return 0;
}

int draw_ship(uint16_t x) {
    if (print_xpm(ship, x, 600) != 0) return 1;
    return 0;
}

int player_game_loop(uint16_t x) {
    int prev_x = x; 
    int ship_width = 50; 
    int ship_height = 50;
    int bullet_width = 50;
    int bullet_height = 30;

    int ipc_st;
    message msg;

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }

    while (scancode != KB_BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_st)) {   
            printf("Error: driver_receive failed with: %d", ipc_st);
            continue;
        }

        if (is_ipc_notify(ipc_st)) {  
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:  
                    if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {  
                        keyboard_change();
                        kbc_ih();   

                        bool is_break = scancode & KB_BREAK_CODE; 
                        //int size = (scancode == KB_TWO_BYTES) ? 2 : 1;   

                        //(kbd_print_scancode)(is_break, size, &scancode); 

                        if (!is_break) { 
                            if (scancode == 0x1e) { // 'A' key (make code)
                                if (x > ship_width) { 
                                    x -= 30; 
                                }
                                printf("Left key pressed, x: %d\n", x);

                            } 
                            else if (scancode == 0x20) { // 'D' key (make code)
                                x += 30;
                                if (x > mode_info.XResolution - ship_width) { 
                                    x = mode_info.XResolution - ship_width;
                                }

                            } 
                            else if (scancode == 0x39) {
                                for (int i = 0; i < MAX_BULLETS; i++) {
                                    if (!bullets[i].active) { 
                                        bullets[i].x = x;    
                                        bullets[i].y = 570;   
                                        bullets[i].active = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) {   
                        timer_int_handler();  

                        for (int i = 0; i < MAX_BULLETS; i++) {
                            if (bullets[i].active) {
                                // Limpar a posição anterior da bala
                                vg_draw_rectangle(bullets[i].x, bullets[i].y, bullet_width, bullet_height, 0x000000);

                                // Atualizar a posição da bala
                                bullets[i].y -= 10;

                                // Desativar a bala se sair da tela
                                if (bullets[i].y < 0) { 
                                    bullets[i].active = false;
                                } else {
                                    // Redesenhar a bala na nova posição
                                    if (print_xpm(bullet, bullets[i].x, bullets[i].y) != 0) {
                                        printf("Error drawing bullet\n");
                                        return 1;
                                    }
                                }
                            }
                        }
                        
                    }
                    break;

                default:
                    break;
            }
        }

        // Limpar apenas a área onde a nave estava antes
        if (prev_x != x) { // Apenas limpe e redesenhe se a posição mudou
            vg_draw_rectangle(prev_x, 600, ship_width, ship_height, 0x000000);
            prev_x = x;
        }

        // Redesenhar a nave na nova posição
        if(draw_ship(prev_x)) return 1;

        // Desenhar balas
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                if (print_xpm(bullet, bullets[i].x, bullets[i].y) != 0) {
                    printf("Error drawing bullet\n");
                    return 1;
                }
            }
        }

    }
    return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

    //int time_elapsed = 0;
    //int n = 10;   
    uint16_t x = 350;

    if (init_game() != 0) return close_game();
    if (draw_ship(x) != 0) return 1;
    if (player_game_loop(x) != 0) return 1;
    //if (waiting_ESC()) return 1;
    return close_game();
}
