#ifndef KB_CONTROLLER_H
#define KB_CONTROLLER_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

int read_keyboard_output(uint8_t port, uint8_t *output, uint8_t mouse);
int write_keyboard_command(uint8_t port, uint8_t command);

#endif // KB_CONTROLLER_H
