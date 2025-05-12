#include "kb_controller.h"

int (read_keyboard_output)(uint8_t port, uint8_t *output, uint8_t mouse) {
    uint8_t status = 0;
    uint8_t trys = 10;
    
    /* Ler o output do teclado */
    for (int i = trys; i > 0; i--) {

        // Lê o status do teclado
        if(util_sys_inb(KEYBOARD_STATUS, &status)) {
            printf("Error reading status\n");
            return 1;
        }

        // Verifica se o output buffer está cheio
        if ((status & BIT(0))) {  

            // Lê o buffer de saída
            if (util_sys_inb(port, output)) {      
                printf("Error reading output\n");
                return 1;
            }

            // Verifica a paridade
            if (status & BIT(7)) { 
                printf("Parity Error\n");
                return 1;
            }

            // Verifica se houve timeout
            if (status & BIT(6)) {  
                printf("Timeout Error\n");
                return 1;
            }

            bool is_keyboard_output = !(status & BIT(5));  // Verifica se o output é do teclado
            
            // Verifica se o output é do dispositivo correto
            if ((mouse && is_keyboard_output) || (!mouse && !is_keyboard_output)) {
                printf("Error: %s output not found\n", mouse ? "Mouse" : "Keyboard");
                return 1;
            }

            return 0; // Sucesso na leitura da saída
        }

        tickdelay(micros_to_ticks(20000));  // Espera 20ms

    }
    return 1;   // Ultrapassou o número de tentativas
}


int (write_keyboard_command)(uint8_t port, uint8_t command) {
    uint8_t status;
    uint8_t trys = 10;

    /* Escrever o comando no teclado */
    for (int i = trys; i > 0; i--) {

        // Lê o status do teclado
        if (util_sys_inb(KEYBOARD_STATUS, &status)) {
            printf("Error reading status\n");
            return 1;
        }

        // Verifica se o input buffer está vazio
        if (!(status & IN_BUFFER_FULL)) {
            
            // Escreve o comando no buffer de entrada
            if (sys_outb(port, command)) {
                printf("Error writing command\n");
                return 1;
            }
            return 0;  // Sucesso na escrita do comando
        }

        tickdelay(micros_to_ticks(20000));  // Espera 20ms

    }
    return 1;   // Ultrapassou o número de tentativas
}
