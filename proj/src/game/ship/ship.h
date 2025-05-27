#ifndef SHIP_H
#define SHIP_H
#include "src/controllers/graphics/graphic.h"
#include "src/controllers/keyboard/kb_controller.h"
#include "src/game/enemies/enemies.h"
#include "src/xpm/game/player.h"
#include "src/xpm/messages.h"
#include <lcom/lcf.h>

#define FREQUENCY 60
#define MODE 0x14c
#define MAX_BULLETS 10

typedef struct {
  int x;
  int y;
  bool active;
} Bullet;

Bullet bullets[MAX_BULLETS];

int(draw_ship)(uint16_t x);
void(init_bullets)();
void(ship_action)();
void(shoot_bullets)();
void(draw_all_bullets)();
void(check_collisions)();

#endif
