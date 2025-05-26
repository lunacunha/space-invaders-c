#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "mouse.h"

int(kbc_read_status)(uint8_t *stat) {
  uint32_t status;
  if (sys_inb(STATUS_REG, &status) != 0) {
    printf("kbc_read_status::Cannot read STATUS_REG\n");
    return 1;
  }
  *stat = (uint8_t) status;
  return 0;
}

int(kbc_read_buffer)(uint8_t *byte) {
  uint32_t output;
  if (sys_inb(OUT_BUF, &output) != 0) {
    printf("kbc_read_buffer::Cannot read OUT_BUF\n");
    return 1;
  }
  *byte = (uint8_t) output;
  return 0;
}

bool(kbc_check_out_buf)() {
  uint8_t status;
  if (kbc_read_status(&status) != 0) {
    return false;
  }
  if (status & KBC_OUTB) {
    if ((status & (KBC_PARE | KBC_TIMEO)) == 0)
      return true;
  }
  return false;
}

bool(kbc_check_in_buf)() {
  uint8_t status;
  if (kbc_read_status(&status) != 0) {
    return false;
  }
  return !(status & KBC_INPB);
}

int(kbc_write_command)(uint32_t cmd) {
  int tries = 0;
  while (tries++ < 3) {
    if (kbc_check_in_buf()) {
      if (sys_outb(CMD_REG, cmd) != 0) {
        printf("kbc_write_command::Failed sys_outb\n");
        return 1;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  printf("kbc_write_command::Max tries exceeded\n");
  return 1;
}

int(kbc_write_argument)(uint32_t arg) {
  int tries = 0;
  while (tries++ < 3) {
    if (kbc_check_in_buf()) {
      if (sys_outb(CMD_ARGS_REG, arg) != 0) {
        printf("kbc_write_argument::Failed sys_outb\n");
        return 1;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  printf("kbc_write_argument::Max attempts reached\n");
  return 1;
}

int(mouse_write_command)(uint32_t cmd, uint8_t *ans) {
  uint32_t output;
  int tries = 0;
  while (tries++ < 3) {
    if (kbc_check_in_buf()) {
      if (sys_outb(CMD_ARGS_REG, cmd) != 0) {
        printf("mouse_write_command::Failed sending command\n");
        return 1;
      }
      tickdelay(micros_to_ticks(DELAY_US));
      if (sys_inb(OUT_BUF, &output) != 0) {
        printf("mouse_write_command::Failed reading answer\n");
        return 1;
      }
      *ans = (uint8_t) output;
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  printf("mouse_write_command::Device not responding\n");
  return 1;
}

int(kbc_command_handler)(uint32_t cmd, uint32_t arg) {
  uint8_t ans;
  int tries = 0;
  do {
    if (kbc_write_command(cmd) != 0) {
      printf("kbc_command_handler::Failed to send command\n");
      return 1;
    }
    if (cmd == WRITE_B_MOUSE) {
      if (mouse_write_command(arg, &ans) != 0) {
        printf("kbc_command_handler::Failed mouse_write_command\n");
        return 1;
      }
      if (ans == ACK)
        return 0;
      else if (ans == NACK)
        tries++;
      else if (ans == ERROR) {
        printf("kbc_command_handler::Received ERROR from mouse\n");
        return 1;
      }
    } 
    else if (cmd == WRITE_CMD_B) {
      if (kbc_write_argument(arg) != 0) {
        printf("kbc_command_handler::Failed to write argument\n");
        return 1;
      }
      return 0;
    }
  } while (tries < 3);

  printf("kbc_command_handler::Max NACKs reached\n");
  return 1;
}
