
#ifndef SHIP_H
#define SHIP_H

#include <lcom/lcf.h>
#include "src/controllers/graphics/graphic.h"
#include "src/xpm/game/player.h"
#include "src/xpm/messages.h"

#define FREQUENCY 60
#define MODE 0x14c
#define MAX_BULLETS 10 

typedef struct {
    int x;         
    int y;          
    bool active;    
} Bullet;

Bullet bullets[MAX_BULLETS];

int (draw_ship)(uint16_t x);

void (init_bullets)();

void (ship_action)(uint8_t code, vbe_mode_info_t mode_info);

void (shoot_bullets)();

#endif
