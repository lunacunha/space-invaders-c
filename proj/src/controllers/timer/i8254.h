#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/**
 * @defgroup i8254 i8254
 * @brief Definições e constantes para programação do temporizador i8254.
 * @{
 */

/* Frequência e IRQ */
#define TIMER_FREQ 1193182 /**< @brief Frequência do clock do temporizador (Hz) */
#define TIMER0_IRQ 0        /**< @brief Linha de interrupção do Timer 0 */

/* Endereços das portas de I/O */
#define TIMER_0    0x40 /**< @brief Registo de contagem do Timer 0 */
#define TIMER_1    0x41 /**< @brief Registo de contagem do Timer 1 */
#define TIMER_2    0x42 /**< @brief Registo de contagem do Timer 2 */
#define TIMER_CTRL 0x43 /**< @brief Registo de controlo dos temporizadores */

#define SPEAKER_CTRL 0x61 /**< @brief Registo de controlo do altifalante (PC speaker) */

/* Seleção do temporizador: bits 7 e 6 */
#define TIMER_SEL0   0x00              /**< @brief Seleciona o Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Seleciona o Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Seleciona o Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Comando Read-Back */

/* Seleção do registo: bits 5 e 4 */
#define TIMER_LSB     BIT(4)                  /**< @brief Inicializa apenas o LSB (byte menos significativo) */
#define TIMER_MSB     BIT(5)                  /**< @brief Inicializa apenas o MSB (byte mais significativo) */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Inicializa primeiro o LSB e depois o MSB */

/* Modos de operação: bits 3, 2 e 1 */
#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Modo 3: gerador de onda quadrada */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Modo 2: gerador de taxa (rate generator) */

/* Modo de contagem: bit 0 */
#define TIMER_BCD 0x01 /**< @brief Contagem em BCD (Binary Coded Decimal) */
#define TIMER_BIN 0x00 /**< @brief Contagem em binário */

/* Formato do comando Read-Back */
#define TIMER_RB_COUNT_  BIT(5)       /**< @brief Lê o valor do contador */
#define TIMER_RB_STATUS_ BIT(4)       /**< @brief Lê o estado do temporizador */
#define TIMER_RB_SEL(n)  BIT((n) + 1) /**< @brief Seleciona o temporizador n no comando Read-Back */

/**@}*/

#endif /* _LCOM_I8254_H_ */
