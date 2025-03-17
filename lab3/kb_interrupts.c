#include "kb_interrupts.h"

uint8_t scancode = 0;
int kb_hook_id = 1;

void (kbc_ih)() {
    // Lê o scancode
    if (read_keyboard_output(KEYBOARD_OUT_CMD, &scancode, 0)) {
        printf("Error reading scancode\n");
    }
}

int (kb_subscribe_int)(uint8_t *bit_no) {

    // Verifica se o bit_no é NULL
    if (bit_no == NULL) {
        printf("Error: bit_no is NULL\n");
        return 1;
    }

    *bit_no = kb_hook_id;   // Guarda o bit_no

    // Subscreve a interrupção
    if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kb_hook_id)) {
        printf("Error setting policy\n");
        return 1;
    }

    return 0;
}

int (kb_unsubscribe_int)() {

    // Remove a subscrição da interrupção
    if (sys_irqrmpolicy(&kb_hook_id)) {
        printf("Error removing policy\n");
        return 1;
    }

    return 0;
}

/* Restaurar as configurações do teclado */
int (kb_restore_settings)() {
    uint8_t config_byte;

    /* Parte 1: Pega o byte de configuração */
    // Escreve o comando para ler o byte de configuração
    if (write_keyboard_command(KEYBOARD_IN_CMD, KEYBOARD_READ_CMD)) {
        printf("Error writing command\n");
        return 1;
    }

    // Lê o byte de configuração
    if (read_keyboard_output(KEYBOARD_OUT_CMD, &config_byte, 0)) {
        printf("Error reading configuration byte\n");
        return 1;
    }

    config_byte = config_byte | BIT(0);   // Reativa as interrupções

    /* Parte 2: Atualiza o byte de configuração */
    // Solicita a escrita do byte de configuração
    if (write_keyboard_command(KEYBOARD_IN_CMD, KEYBOARD_WRITE_CMD)) {
        printf("Error writing command\n");
        return 1;
    }

    // Escreve o novo byte de configuração
    if (sys_outb(KEYBOARD_WRITE_CMD, config_byte)) {
        printf("Error writing configuration byte\n");
        return 1;
    }

    return 0;
}
