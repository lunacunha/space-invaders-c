// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include <kb_controller.h>
// Any header files included below this line should have been created by you
#include <graphic.h>
#include <kb_interrupts.h>

extern vbe_mode_info_t mode_info;
extern uint8_t scancode;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    if (set_graphical_mode(mode) != 0) return 1;
    sleep(delay);
    if (vg_exit() != 0) return 1;
    return 0;
}

int (waiting_ESC)() {
  int ipc_st;
  uint8_t irq_set;
  message msg;

  if ((kb_subscribe_int)(&irq_set)) {
    printf("Error subscribing keyboard\n");
    return 1;
  }

  while (scancode != KB_BREAK_ESC) {
    if (driver_receive(ANY, &msg, &ipc_st)) {   // Receber a mensagem
      printf("Error: driver_receive failed with: %d", ipc_st);
      continue;
    }
  
    if (is_ipc_notify(ipc_st)) {  // Verificar se a mensagem é uma notificação
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:  
          if (msg.m_notify.interrupts & BIT(irq_set)) {   // Verificar se a interrupção é do teclado
            kbc_ih();   // Tratar a interrupção
            break;
          }   
          break;
        default:
          break;
      }
    }
  }
  if ((kb_unsubscribe_int)()) {
    printf("Error unsubscribing keyboard\n");
    return 1;
  }
  return 0;

}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

    printf("nao\n");
                         
  
  if (set_frame_buffer(mode) != 0) return 1;

  if (set_graphical_mode(mode) != 0) return 1;

  uint32_t n_color;
  if (norm_color(color, &n_color) != 0) return 1;

  if (vg_draw_rectangle(x, y, width, height, n_color) != 0) return 1;
  
  if (waiting_ESC() != 0) return 1;

  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (set_frame_buffer(mode) != 0) return 1;
  if (set_graphical_mode(mode) != 0) return 1;

  int v = mode_info.YResolution/no_rectangles;
  int h = mode_info.XResolution/no_rectangles;

  for (int i = 0; i < no_rectangles; i++) {
    for (int j = 0; j < no_rectangles; j++) {
      uint32_t color;
      
      // direct mode
      if (mode_info.MemoryModel == 0x06) color = dir_mode(j, i, step, first);
      
      // indexed mode
      else color = idx_mode(j, i, step, first, no_rectangles);

      if (vg_draw_rectangle(j*h, i*v, h, v, color) != 0) return 1;
    }
  }

  if (waiting_ESC() != 0) return 1;
  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  uint16_t mode = 0x105; // indexed mode only
  if (set_frame_buffer(mode) != 0) return 1;
  if (set_graphical_mode(mode) != 0) return 1;
  if (print_xpm(xpm, x, y) != 0) return 1;
  if (waiting_ESC()!=0) return 1;
  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

  int ipc_st;
  message msg;
  uint8_t irq_keyboard;
  uint8_t irq_timer;
  uint8_t horizontal;

  if (xi < xf && yi == yf) horizontal = 1; 
  else if (xi == xf && yi < yf) horizontal = 0;
  else return 1;

  // Subscribing keyboard and timer interruptions
  if (kb_subscribe_int(&irq_keyboard) != 0) {
    printf("Error subscribing keyboard\n");
    return 1;
  }

  if (timer_subscribe_int(&irq_timer) != 0) {
    printf("Error subscribing timer\n");
    return 1;
  }

  uint16_t mode = 0x105; // indexed mode only

  if (set_frame_buffer(mode) != 0) return 1;
  if (set_graphical_mode(mode) != 0) return 1;
  
  // set timer frequency
  if (timer_set_frequency(0, fr_rate) != 0) return 1;

  // xpm initial position
  if (print_xpm(xpm, xi, yi) != 0) return 1;


  // handle interruption
  while (scancode != KB_BREAK_ESC && (xi < xf || yi < yf)) {
    if (driver_receive(ANY, &msg, &ipc_st)) {   
      printf("Error: driver_receive failed with: %d", ipc_st);
      continue;
    }
  
    if (is_ipc_notify(ipc_st)) {  
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:  
          if (msg.m_notify.interrupts & BIT(irq_keyboard)) {   // check keyboard interruption
            kbc_ih();   
            break;
          }   

          if (msg.m_notify.interrupts & BIT(irq_timer)) {   // Check timer interruption
            
            if (horizontal==1) {
              xi += speed;
              if(xi < xf) xi = xf;
            }
            else {
              yi += speed;
              if (yi < yf) yi = yf;
            }


            // print the xpm in the new position
            if(print_xpm(xpm, xi, yi) != 0) {
              printf("Error printing new xpm");
              return 1;
            }
                       
            break;
          }  
          break;
        default:
          break;
      }
    }
  }

  // Leave graphic mode
  if (vg_exit() != 0) return 1;

  // Unsubscrive interruptions
  if (timer_unsubscribe_int() != 0) return 1;
  if (kb_unsubscribe_int() != 0) return 1;

  return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
} 
