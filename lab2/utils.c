#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <sys/io.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL)
    return 1;
  *lsb = val & 0xff;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL)
    return 1;
  *msb = val & 0x00ff;
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL)
    return 1;
  u_int32_t data;
  if (sys_inb(port, &data) != 0)
    return 1;
  *value = (uint8_t)data;
  return 0;
}
