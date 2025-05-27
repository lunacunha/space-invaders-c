#ifndef SHIP_H
#define SHIP_H

#include <lcom/lcf.h>
#include "src/controllers/graphics/graphic.h"
#include "src/controllers/keyboard/kb_controller.h"
#include "src/xpm/game/player.h"
#include "src/xpm/messages.h"

#define FREQUENCY 60
#define MODE 0x14c
#define MAX_BULLETS 10 

#define BULLET_WIDTH 8
#define BULLET_HEIGHT 30

typedef struct {
    int x;         
    int y;          
    bool active;    
} Bullet;

Bullet bullets[MAX_BULLETS];

int (draw_ship)(uint16_t x);

void (init_bullets)();

void (ship_action)();

int (draw_all_bullets)();

int (draw_bullet)(Bullet* bullet_obj);

void (update_bullets)(); 

#endif
