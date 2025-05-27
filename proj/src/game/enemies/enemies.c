#include "ship.h"

uint16_t x = 350;
int ship_width = 50;
int ship_height = 50;
int bullet_width = 50;
int bullet_height = 30;

extern vbe_mode_info_t mode_info;
extern uint8_t scancode;

int draw_ship(uint16_t x) {
  if (print_xpm(ship, x, 600) != 0)
    return 1;
  return 0;
}

void init_bullets() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    bullets[i].active = false;
  }
}

void ship_action() {
  if (scancode == KB_A) { // 'A' key (make code)
    if (x > ship_width) {
      x -= 30;
    }
    printf("Left key pressed, x: %d\n", x);
  }
  else if (scancode == KB_D) { // 'D' key (make code)
    x += 30;
    if (x > mode_info.XResolution - ship_width) {
      x = mode_info.XResolution - ship_width;
    }
  }
  else if (scancode == KB_SPACE) {
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (!bullets[i].active) {
        bullets[i].x = x;
        bullets[i].y = 570;
        bullets[i].active = true;
        break;
      }
    }
  }
}

void shoot_bullets() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].active) {
      bullets[i].y -= 10;
      if (bullets[i].y < 0) {
        bullets[i].active = false;
      }
    }
  }
}

void draw_all_bullets() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].active) {
      print_xpm(bullet, bullets[i].x, bullets[i].y);
    }
  }
}

void check_collisions() {
  for (int b = 0; b < MAX_BULLETS; b++) {
    if (!bullets[b].active)
      continue;

    for (int e = 0; e < MAX_ENEMIES; e++) {
      if (!enemies[e].active)
        continue;

      if (bullets[b].x < enemies[e].x + ENEMY_WIDTH &&
          bullets[b].x + bullet_width > enemies[e].x &&
          bullets[b].y < enemies[e].y + ENEMY_HEIGHT &&
          bullets[b].y + bullet_height > enemies[e].y) {

        bullets[b].active = false;
        enemies[e].active = false;
        break;
      }
    }
  }
}
