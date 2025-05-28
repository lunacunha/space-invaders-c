#ifndef SHIP_H
#define SHIP_H

#include <lcom/lcf.h>
#include "src/controllers/graphics/graphic.h"
#include "src/controllers/keyboard/kb_controller.h"
#include "src/xpm/game/player.h"
#include "src/xpm/messages.h"
#include "src/game/enemies/enemies.h"

#define FREQUENCY 60
#define MODE 0x14c
#define MAX_BULLETS 10 
#define MAX_ENEMY_BULLETS 10

#define BULLET_WIDTH 8
#define BULLET_HEIGHT 30
#define ENEMY_BULLET_WIDTH 5
#define ENEMY_BULLET_HEIGHT 17

typedef struct {
    int x;         
    int y;          
    bool active;    
} Bullet;

typedef struct {
    int x;
    int y;      
    bool active;    
} EnemyBullet;

Bullet bullets[MAX_BULLETS];
EnemyBullet enemy_bullets[MAX_ENEMY_BULLETS];

int (draw_ship)(uint16_t x);

void (init_bullets)();

void (init_enemy_bullets)();

void (ship_action)();

void (fire_enemy_bullet)();

int (draw_all_bullets)();

int (draw_bullet)(Bullet* bullet_obj);

void (draw_enemy_bullets)();

void (update_bullets)();    

void (update_enemy_bullets)();

#endif
