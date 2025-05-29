#pragma once
#include <lcom/lcf.h>
#include <minix/sysutil.h>

extern uint8_t data;


int mouse_subscribe_int(uint8_t *irq_bitmask);

int mouse_unsubscribe_int();

int kbc_read_status(uint8_t *status);

int kbc_read_buffer(uint8_t *data);

bool kbc_check_out_buf();

bool kbc_check_in_buf();

int kbc_write_command(uint32_t cmd);

int kbc_write_argument(uint32_t arg);

int mouse_write_command(uint32_t cmd, uint8_t *resp);

int kbc_command_handler(uint32_t cmd, uint32_t arg);

void mouse_parse_packet(struct packet *pk, uint8_t bytes[], uint8_t size);
