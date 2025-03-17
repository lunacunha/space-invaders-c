#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>
#include "kb_interrupts.h"

extern uint8_t scancode;
uint32_t counter_keyboard = 0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  /* To be completed by the students */
  int ipc_st;        
  uint8_t irq_set;    
  message msg;        

  // Subscrever as interrupções do teclado
  if ((kb_subscribe_int)(&irq_set)) {
    printf("Error subscribing keyboard\n");
    return 1;
  }

  // Ciclo de interrupções
  while(scancode != KB_BREAK_ESC) {
    if (driver_receive(ANY, &msg, &ipc_st)) {  // Receber a mensagem
      printf("Error: driver_receive failed with: %d", ipc_st);
      continue;
    }

    if (is_ipc_notify(ipc_st)) {  // Verificar se a mensagem é uma notificação
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:  
          if (msg.m_notify.interrupts & BIT(irq_set)) {   // Verificar se a interrupção é do teclado
            kbc_ih();   // Tratar a interrupção

            bool is_break = !(scancode & KB_MAKE_CODE);  // Verificar se é um break code
            int size = (scancode == KB_TWO_BYTES) ? 2 : 1;   // Verificar o tamanho do scancode

            (kbd_print_scancode)(is_break, size, &scancode);  // Imprimir o scancode
          }
          break;
        default:
          break;
      }
    }
  }

  // Cancelar a subscrição das interrupções do teclado
  if ((kb_unsubscribe_int)()) {
    printf("Error unsubscribing keyboard\n");
    return 1;
  }

  // Imprimir o número de sys_inb calls
  if((kbd_print_no_sysinb)(counter_keyboard)) {
    printf("Error printing number of sys_inb calls\n");
    return 1;
  }

  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  while (scancode != KB_BREAK_ESC)  {

    // Lê o scancode
    if (read_keyboard_output(KEYBOARD_OUT_CMD, &scancode, 0) == 0) {
      bool is_break = !(scancode & KB_MAKE_CODE);  // Verificar se é um break code
      int size = (scancode == KB_TWO_BYTES) ? 2 : 1;   // Verificar o tamanho do scancode

      (kbd_print_scancode)(is_break, size, &scancode);  // Imprimir o scancode
    }
  }

  return kb_restore_settings();
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
