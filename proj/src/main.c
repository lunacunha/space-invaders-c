#include "confs.h"
#include "controllers/graphics/graphic.h"
#include "controllers/keyboard/kb_interrupts.h"
#include "game/game_state.h"
#include "game/menu/menu.h"
#include "game/scores/score.h"
#include "game/ship/ship.h"
#include "xpm/characters/letter_char.h"
#include "xpm/characters/number_char.h"
#include "xpm/characters/special_char.h"
#include "xpm/game/aliens.h"
#include "xpm/game/player.h"
#include "xpm/menu/options.h"
#include "xpm/menu/title.h"
#include "xpm/messages.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define TIMER 0
#define FREQ 60

extern uint16_t x;

extern uint8_t scancode;
extern int timer_counter;
extern vbe_mode_info_t mode_info;
uint8_t irq_set_keyboard, irq_set_timer;
extern bool player_win;
extern bool player_lost;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int init_game() {
  if (timer_set_frequency(TIMER, FREQ))
    return 1;
  if (set_frame_buffer(MODE))
    return 1;
  if (set_graphical_mode(MODE))
    return 1;
  if ((kb_subscribe_int) (&irq_set_keyboard))
    return 1;
  if ((timer_subscribe_int) (&irq_set_timer))
    return 1;
  menu_init();
  return 0;
}

int close_game() {
  if ((kb_unsubscribe_int) ())
    return 1;
  if ((timer_unsubscribe_int) ())
    return 1;
  if (vg_exit())
    return 1;
  return 0;
}

int menu_handler() {
  message msg;
  int ipc_status;
  while (scancode != KB_BREAK_ESC) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Error: driver_receive failed");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // Teclado
          if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {
            kbc_ih();
            menu_handle_input();
          }
          // Timer
          if (msg.m_notify.interrupts & BIT(irq_set_timer)) {
            timer_int_handler();
            MenuState state = menu_get_state();

            if (state == MENU_START) {
              clear_back_buf(0x000000);

              ship_action();
              shoot_bullets();
              move_enemies();
              check_collisions();

              if (draw_ship(x) != 0)
                return 1;
              draw_all_bullets();
              draw_all_enemies();

              swap_buffers();

              if (game_state() != 0)
                return 1;
            }
            else if (state == MENU_SCORES) {
              if (vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x000000))
                return 1;

              if (!player_win && !player_lost) {
                if (print_xpm(score_board, 0, 0))
                  return 1;
                if (print_xpm(message1, 300, 500))
                  return 1;
              }
              else if (player_win) {
                if (print_xpm(message3, 400, 300))
                  return 1;
                if (print_xpm(message2, 300, 400))
                  return 1;
                if (print_xpm(message1, 300, 500))
                  return 1;
              }
              else if (player_lost) {
                if (print_xpm(message4, 400, 300))
                  return 1;
                if (print_xpm(message1, 300, 500))
                  return 1;
              }

              player_win = false;
              player_lost = false;
              if (score_state() != 0)
                return 1;

              if (vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x000000))
                return 1;
              menu_set_state(MENU_MAIN);
            }
            else if (state == MENU_EXIT) {
              return 0;
            }
            else {
              menu_render();
            }
          }
          break;
        default:
          break;
      }
    }
  }
  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  if (init_game() != 0)
    return 1;
  if (menu_handler() != 0)
    return 1;
  return close_game();
}
