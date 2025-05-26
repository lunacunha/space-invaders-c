#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "mouse.h"

int mouse_hookID = 2;
uint8_t data;

int(mouse_subscribe_int)(uint8_t *irq_mask) {
  if (irq_mask == NULL)
    return 1;
  *irq_mask = BIT(mouse_hookID);
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookID) != 0) {
    printf("mouse_subscribe_int::Failed to set IRQ policy\n");
    return 1;
  }
  return 0;
}

int(mouse_unsubscribe_int)() {
  if (sys_irqrmpolicy(&mouse_hookID) != 0) {
    printf("mouse_unsubscribe_int::Failed to remove IRQ policy\n");
    return 1;
  }
  return 0;
}

void(mouse_parse_packet)(struct packet *pkt, uint8_t buf[], uint8_t len) {
  if (pkt == NULL || buf == NULL) return;
  pkt->lb = (buf[0] & PK_LB) != 0;
  pkt->rb = (buf[0] & PK_RB) != 0;
  pkt->mb = (buf[0] & PK_MB) != 0;

  int16_t x_move = buf[1];
  int16_t y_move = buf[2];
  if (buf[0] & PK_XSIGN) x_move |= 0xFF00;
  if (buf[0] & PK_YSIGN) y_move |= 0xFF00;

  pkt->delta_x = x_move;
  pkt->delta_y = y_move;

  pkt->x_ov = (buf[0] & PK_XOVF) != 0;
  pkt->y_ov = (buf[0] & PK_YOVF) != 0;

  for (uint8_t i = 0; i < len; i++) {
    pkt->bytes[i] = buf[i];
  }
}

void(mouse_ih)() {
  if (kbc_check_out_buf()) {
    if (kbc_read_buffer(&data) != 0) {
      printf("mouse_ih::Error reading mouse data\n");
    }
  }
}
