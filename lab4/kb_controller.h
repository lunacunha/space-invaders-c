#ifndef KB_CONTROLLER_H
#define KB_CONTROLLER_H

#include "i8042.h"
#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>

int read_kb_controller_output(uint8_t port, uint8_t *output, uint8_t mouse);
int write_kb_controller_command(uint8_t port, uint8_t command);

#endif // KB_CONTROLLER_H
