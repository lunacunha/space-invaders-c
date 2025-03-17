#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
int hook_id = 0;
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL) return 1;
  if (timer > 2 || timer < 0) return 1;
  uint8_t rbc = 0;
  rbc = (TIMER_RB_CMD | )
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  switch(timer) {
    
  }
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
