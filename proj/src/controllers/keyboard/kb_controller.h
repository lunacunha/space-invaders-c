#ifndef KB_CONTROLLER_H
#define KB_CONTROLLER_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Lê um byte do teclado ou rato a partir de uma porta específica.
 * 
 * @param port Porta de onde será lido o byte (normalmente o 0x60).
 * @param output Ponteiro onde será armazenado o byte lido.
 * @param mouse Indica se a leitura é do rato (1) ou do teclado (0).
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int read_keyboard_output(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Escreve um comando para o teclado (ou rato) numa porta específica.
 * 
 * @param port Porta onde o comando será escrito (por ex. 0x64).
 * @param command Comando a enviar.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int write_keyboard_command(uint8_t port, uint8_t command);

#endif // KB_CONTROLLER_H
