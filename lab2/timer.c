#include <cstddef>
#include <lcom/lcf.h>
#include <lcom/timer.h>

#include "utils.c"
#include <stdint.h>

#include "i8254.h"

uint8_t timer_conf;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint16_t divisor;
  uint8_t lsb, msb;
  uint8_t new_control_word;
  uint16_t timer_port;
  if (timer > 2 || freq <= 0 || freq > TIMER_FREQ)
    return 1;

  divisor = TIMER_FREQ / freq;

  if (timer_get_conf(timer, &timer_conf) != 0)
    return 1;

  switch (timer) {
    case 0:
      new_control_word = TIMER_SEL0;
      timer_port = TIMER_0;
      break;
    case 1:
      new_control_word = TIMER_SEL1;
      timer_port = TIMER_1;
      break;
    case 2:
      new_control_word = TIMER_SEL2;
      timer_port = TIMER_2;
      break;

    default: return 1;
  }
  new_control_word |= TIMER_LSB_MSB;
  new_control_word |= (timer_conf & 0x0F);

  if (sys_outb(TIMER_CTRL, new_control_word) != 0)
    return 1;
  if (util_get_LSB(divisor, &lsb) != 0)
    return 1;
  if (util_get_MSB(divisor, &msb) != 0)
    return 1;
  if (sys_outb(timer_port, lsb) != 0)
    return 1;
  if (sys_outb(timer_port, msb) != 0)
    return 1;
  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void(timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t rb_cmd;
  int timer_port;

  if (st == NULL)
    return 1;

  rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, rb_cmd) != 0)
    return 1;
  switch (timer) {
    case 0:
      timer_port = TIMER_0;
      break;
    case 1:
      timer_port = TIMER_1;
      break;
    case 2:
      timer_port = TIMER_2;
      break;

    default: return 1;
  }

  if (util_sys_inb(timer_port, st) != 0)
    return 1;

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
