#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "mouse.h"

extern unsigned int timer_interrupt_counter;

uint8_t size = 0; //size of the array of the packets
uint8_t bytes[3];//array with the bytes of the packets


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  uint8_t irq_bitmask;
  unsigned int packet_count = 0;
  int recv_status;
  message msg_data;
  int ipc_status;
  int sync_state = 0;  /* Track synchronization: 0=not synced, 1=synced */
  
  /* Enable data reporting for mouse device */
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_ENABLE_REPORT) != 0) {
    printf("Failed to enable mouse data reporting\n");
    return 1;
  }
  
  /* Register interrupt handler */
  if (mouse_subscribe_int(&irq_bitmask) != 0) {
    printf("Interrupt subscription failed\n");
    return 1;
  }
  
  /* Main processing loop for mouse packets */
  while (packet_count < cnt) {
    /* Wait for interrupt notification */
    if ((recv_status = driver_receive(ANY, &msg_data, &ipc_status)) != 0) {
      printf("Error %d while receiving message\n", recv_status);
      continue;
    }
    
    /* Process only hardware notifications */
    if (!is_ipc_notify(ipc_status)) continue;
    
    /* Check if notification is from hardware */
    if (_ENDPOINT_P(msg_data.m_source) != HARDWARE) continue;
    
    /* Check if it's our subscribed interrupt */
    if (!(msg_data.m_notify.interrupts & irq_bitmask)) continue;
    
    /* Handle mouse interrupt */
    mouse_ih();
    
    /* Synchronization check - first byte must have bit 3 set */
    if (size == 0) {
      if ((data & PK_BIT3) == 0) {
        /* Out of sync, discard and wait for next byte */
        sync_state = 0;
        continue;
      }
      else {
        /* First byte with bit 3 set - we are in sync */
        sync_state = 1;
      }
    }
    else if (!sync_state) {
      /* If we somehow got here without being in sync, reset */
      size = 0;
      continue;
    }
    
    /* Store the byte in our buffer */
    bytes[size++] = data;
    
    /* Process complete packet when we have all 3 bytes */
    if (size == 3) {
      struct packet pkt_data;
      
      /* Parse and print the packet */
      mouse_parse_packet(&pkt_data, bytes, size);
      mouse_print_packet(&pkt_data);
      
      /* Reset for next packet and increment counter */
      size = 0;
      packet_count++;
    }
  }
  
  /* Cleanup: unsubscribe from interrupts */
  if (mouse_unsubscribe_int() != 0) {
    printf("Failed to unsubscribe from mouse interrupts\n");
    return 1;
  }
  
  /* Disable data reporting */
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_DISABLE_REPORT) != 0) {
    printf("Failed to disable mouse data reporting\n");
    return 1;
  }
  
  return 0;
}


int(mouse_test_async)(uint8_t idle_time) {
  /* To be completed */
  printf("%s(%u): under construction\n", __func__, idle_time);
  return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance){
  /* To be completed */
  printf("%s: under construction\n", __func__);
  return 1;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* This year you need not implement this. */
  printf("%s(%u, %u): under construction\n", __func__, period, cnt);
  return 1;
}
