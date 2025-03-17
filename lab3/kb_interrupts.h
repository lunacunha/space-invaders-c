#ifndef KB_INTERRUPTS_H
#define KB_INTERRUPTS_H

#include <stdint.h>
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "kb_controller.h"

void (kbc_ih)();
int (kb_subscribe_int)(uint8_t *bit_no);
int (kb_unsubscribe_int)();
int (kb_restore_settings)();

#endif // KB_INTERRUPTS_H
