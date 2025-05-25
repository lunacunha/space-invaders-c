#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ 1

#define KEYBOARD_STATUS 0x64
#define KEYBOARD_IN_CMD 0x64
#define KEYBOARD_OUT_CMD 0x60
#define KEYBOARD_READ_CMD 0x20
#define KEYBOARD_WRITE_CMD 0x60

#define KB_A 0x1E
#define KB_D 0x20
#define KB_SPACE 0x39
#define KB_ENTER 0x1C
#define KB_UP_ARROW 0x48
#define KB_DOWN_ARROW 0x50
#define KB_BREAK_ESC 0x81


#define KB_TWO_BYTES 0xE0
#define KB_BREAK_CODE BIT(7)

#define IN_BUFFER_FULL BIT(1)

#endif /* _LCOM_I8042_H */
