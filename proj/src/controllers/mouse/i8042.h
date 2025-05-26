#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

// IRQ lines
#define KBD_IRQ 1    // Keyboard IRQ
#define MOUSE_IRQ 12 // Mouse IRQ

// Ports
#define OUT_BUF 0x60      // Output buffer
#define CMD_ARGS_REG 0x60 // Command arguments
#define STATUS_REG 0x64   // Status register
#define CMD_REG 0x64      // Command register

// Delay
#define DELAY_US 20000 // Command response delay (us)

// Important keys
#define ESC_KEY 0x81            // ESC breakcode
#define FIRST_OF_TWO_BYTES 0xE0 // 1st byte of two-byte scan code

// KBC Status Register bits
#define KBC_PARE BIT(7)  // Parity error
#define KBC_TIMEO BIT(6) // Timeout error
#define KBC_AUXB BIT(5)  // Mouse data
#define KBC_KEYL BIT(4)  // Keylock flag
#define KBC_CD BIT(3)    // Command/Data indicator
#define KBC_SYSF BIT(2)  // System flag
#define KBC_INPB BIT(1)  // Input buffer full
#define KBC_OUTB BIT(0)  // Output buffer full

// KBC Commands
#define READ_CMD_B 0x20  // Read command byte
#define WRITE_CMD_B 0x60 // Write command byte
#define CHECK_KBC 0xAA   // Self-test

// Keyboard-specific commands
#define CHECK_KB_IFC 0xAB // Test keyboard interface
#define DISABLE_KBD 0xAD  // Disable keyboard
#define ENABLE_KBS 0xAE   // Enable keyboard

// Mouse-specific commands
#define CHECK_MOUSE_IFC 0xA9 // Test mouse interface
#define DISABLE_MOUSE 0xA7   // Disable mouse
#define ENABLE_MOUSE 0xA8    // Enable mouse
#define WRITE_B_MOUSE 0xD4   // Write byte to mouse

// Mouse commands (sent through WRITE_B_MOUSE)
#define MOUSE_RESET 0xFF          // Reset mouse
#define MOUSE_ENABLE_REPORT 0xF4  // Enable reporting
#define MOUSE_DISABLE_REPORT 0xF5 // Disable reporting
#define MOUSE_REMOTE_MODE 0xF0    // Set remote mode
#define MOUSE_STREAM_MODE 0xEA    // Set stream mode
#define MOUSE_READ_DATA 0xEB       // Read data

// Mouse controller responses
#define ACK 0xFA   // Acknowledge
#define NACK 0xFE  // Invalid byte
#define ERROR 0xFC // Error

// Mouse packet bits
#define PK_LB BIT(0)    // Left button
#define PK_RB BIT(1)    // Right button
#define PK_MB BIT(2)    // Middle button
#define PK_BIT3 BIT(3)  // Always 1
#define PK_XSIGN BIT(4) // X sign
#define PK_YSIGN BIT(5) // Y sign
#define PK_XOVF BIT(6)  // X overflow
#define PK_YOVF BIT(7)  // Y overflow

// Command byte bits
#define ENABLE_KBD_IR BIT(0)   // Enable keyboard interrupts
#define ENABLE_MOUSE_IR BIT(1) // Enable mouse interrupts

#endif // _LCOM_I8042_H_
