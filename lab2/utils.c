#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */

  if (lsb == NULL) return 1;  // verificar se o endereço é nulo

  /* Obter o LSB do valor */
  *lsb = val & 0xff;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */

  if (msb == NULL) return 1;  // verificar se o endereço é nulo

  /* Obter o MSB do valor */
  *msb = val >> 8;       

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */

  if (value == NULL) return 1;  // verificar se o endereço é nulo

  /* 1º Parte: Ler o valor do porto */
  uint32_t val;
  if (sys_inb(port, &val) != 0) return 1;

  /* 2º Parte: Passar de 32 bits para 8 bits */
  *value = val & 0xff;

  return 0;
}
