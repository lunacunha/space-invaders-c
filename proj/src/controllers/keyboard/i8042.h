#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/// Linha IRQ do teclado (usada para interrupções)
#define KEYBOARD_IRQ 1

/// Porta de estado do teclado (KBC)
#define KEYBOARD_STATUS 0x64

/// Porta de entrada de comandos para o KBC
#define KEYBOARD_IN_CMD 0x64

/// Porta de saída para ler dados do KBC
#define KEYBOARD_OUT_CMD 0x60

/// Comando para ler o byte de comando do KBC
#define KEYBOARD_READ_CMD 0x20

/// Comando para escrever o byte de comando do KBC
#define KEYBOARD_WRITE_CMD 0x60

// -------------------------
// Códigos de teclas (make codes)
// -------------------------

/// Código da tecla 'A'
#define KB_A 0x1E

/// Código da tecla 'D'
#define KB_D 0x20

/// Código da tecla de espaço
#define KB_SPACE 0x39

/// Código da tecla Enter
#define KB_ENTER 0x1C

/// Código da seta para cima
#define KB_UP_ARROW 0x48

/// Código da seta para baixo
#define KB_DOWN_ARROW 0x50

/// Código de break (soltar) da tecla Escape
#define KB_BREAK_ESC 0x81

// -------------------------
// Códigos especiais
// -------------------------

/// Identificador de scancodes com dois bytes
#define KB_TWO_BYTES 0xE0

/// Máscara para identificar códigos de "break" (tecla largada)
#define KB_BREAK_CODE BIT(7)

/// Máscara para verificar se o buffer de entrada está cheio
#define IN_BUFFER_FULL BIT(1)

#endif /* _LCOM_I8042_H_ */
