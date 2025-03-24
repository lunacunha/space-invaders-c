#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  
  if (timer > 2 || timer < 0) return 1;  // verificar se o timer é válido
  if (freq > TIMER_FREQ || freq < 19) return 1;  // verificar se a frequência é válida


  /* 1ª Parte: Atualizar o status do timer com o novo timer e o modo de inicialização (lsb follow by msb) */

  uint8_t control_word;
  if (timer_get_conf(timer, &control_word)) return 1;   // obter o status do timer

  control_word = control_word & 0x0F;   // limpar os bits de controlo
  control_word = control_word | TIMER_LSB_MSB;  // inicializar o contador LSB e MSB ???

  uint8_t selected_timer;

  switch(timer) {                                   // inicializar o timer
    case 0:
      control_word = control_word | TIMER_SEL0;     // 0x00
      selected_timer = TIMER_0;
      break;

    case 1:
      control_word = control_word | TIMER_SEL1;     // BIT(6)
      selected_timer = TIMER_1;
      break;

    case 2:
      control_word = control_word | TIMER_SEL2;     // BIT(7)
      selected_timer = TIMER_2;
      break;

    default:
      return 1;
  }

  if(sys_outb(TIMER_CTRL, control_word)) return 1;        // atualizar o status do timer


  /* 2ª Parte: Escrever a frequencia no timer */

  uint32_t initial_value = TIMER_FREQ / freq;   // calcular o valor inicial do contador
  uint8_t msb, lsb;

  if (util_get_MSB(initial_value, &msb)) return 1;  // obter o MSB do valor inicial
  if (util_get_LSB(initial_value, &lsb)) return 1;  // obter o LSB do valor inicial
                                            
  if(sys_outb(selected_timer, lsb)) return 1;   // escrever o LSB 
  if(sys_outb(selected_timer, msb)) return 1;   // escrever o MSB

  return 0;
}


int (timer_subscribe_int)(uint8_t *bit_no) {
  /* To be implemented by the students */

  if (bit_no == NULL) return 1;   // verificar se o endereço é nulo

  /* 1ª Parte: Atualizar o bit_no */
  *bit_no = hook_id;   

  /* 2ª Parte: Subscrever as interrupções do timer */
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return 1; 

  return 0;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  
  /* Dessubscrever as interrupções do timer */
  if (sys_irqrmpolicy(&hook_id)) return 1;  

  return 0;
}

void (timer_int_handler)() {
  /* To be implemented by the students */

  // Incrementar o contador de interrupções
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */

  if (st == NULL) return 1;   // verificar se o endereço é nulo
  if (timer > 2 || timer < 0) return 1;  // verificar se o timer é válido


  /* 1ª Parte: Escrever o read-back command no control register para avisar que se quer ler o status do timer */

  uint8_t rbc = 0;  // criar o read-back command
  rbc = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));  // atualizar o read-back command
  if (sys_outb(TIMER_CTRL, rbc)) return 1;  // escrever no control register


  /* 2ª Parte: Ler o status do timer */

  int timer_address = TIMER_0 + timer;  // endereço do timer a ser lido
  if (util_sys_inb(timer_address, st)) return 1;  // ler o status do timer

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  /* To be implemented by the students */

  /* 1ª Parte: Atualizar o campo do status do timer */

  union timer_status_field_val data;      // criar a union para guardar os campos do status do timer

  switch(field) {
    case tsf_all:
      data.byte = st;   // guardar o status do timer no campo byte
      break;

    case tsf_initial:
      st = (st >> 4) & 0x03; 

      switch(st) {      // initialization mode
        case 1:
          data.in_mode = LSB_only;
          break;
        case 2:
          data.in_mode = MSB_only;
          break;
        case 3: 
          data.in_mode = MSB_after_LSB;
          break;
        default:
          data.in_mode = INVAL_val;
          break;
      }
      break;

    case tsf_mode:
      st = (st >> 1) & 0x07; 
      
      switch(st) {      // operation mode
        case 6:
          data.count_mode = 2;
          break;
        case 7:
          data.count_mode = 3;
          break;
        default:
          data.count_mode = st;
          break;
      }
      break;

    case tsf_base:                // counting base
      data.bcd = st & TIMER_BCD;
      break;

    default:
      return 1;
  }

  /* 2ª Parte: Imprimir o campo do status do timer */

  if (timer_print_config(timer, field, data)) return 1;

  return 0;
}
