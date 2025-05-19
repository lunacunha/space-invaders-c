#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "controllers/keyboard/kb_interrupts.h"

#define TIMER 0
#define FREQ 60

extern uint8_t scancode;
extern int timer_counter;

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


int (proj_main_loop)(int argc, char *argv[]){
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
    }

    return 0;
}
