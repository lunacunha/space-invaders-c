#ifndef KB_INTERRUPTS_H
#define KB_INTERRUPTS_H

#include <stdint.h>
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "kb_controller.h"

/**
 * @brief Rotina de interrupção do teclado.
 * 
 * Lê o scancode do teclado a partir do buffer de saída do KBC.
 */
void (kbc_ih)();

/**
 * @brief Subscreve as interrupções do teclado.
 * 
 * @param bit_no Ponteiro para a variável onde será armazenado o bit da interrupção.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (kb_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Cancela a subscrição das interrupções do teclado.
 * 
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (kb_unsubscribe_int)();

/**
 * @brief Restaura as definições originais do teclado.
 * 
 * Garante que o teclado volta ao estado normal após o fim da execução do programa.
 * 
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (kb_restore_settings)();

#endif // KB_INTERRUPTS_H
