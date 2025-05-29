#ifndef SHIP_H
#define SHIP_H

#include <lcom/lcf.h>
#include "src/controllers/graphics/graphic.h"
#include "src/controllers/keyboard/kb_controller.h"
#include "src/controllers/mouse/mouse.h"
#include "src/xpm/game/player.h"
#include "src/xpm/messages.h"
#include "src/game/enemies/enemies.h"
#include "src/xpm/game/shields.h"

#define FREQUENCY 60
#define MODE 0x14c
#define MAX_BULLETS 10 
#define MAX_ENEMY_BULLETS 10

#define BULLET_WIDTH 8
#define BULLET_HEIGHT 30
#define ENEMY_BULLET_WIDTH 5
#define ENEMY_BULLET_HEIGHT 17

#define NUM_SHIELDS 4
#define SHIELD_WIDTH 47
#define SHIELD_HEIGHT 34

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

typedef enum {
    SHIELD_FULL,
    SHIELD_BROKEN,
    SHIELD_NONE
} ShieldState;

typedef struct {
    int x, y;          
    ShieldState state;  
} Shield;

Bullet bullets[MAX_BULLETS];
EnemyBullet enemy_bullets[MAX_ENEMY_BULLETS];
Shield shields[NUM_SHIELDS];

int (draw_ship)(uint16_t x);

void (init_bullets)();

void (init_enemy_bullets)();

void (init_shields)();

void (ship_action)();

void (handle_mouse_input)();

void (fire_enemy_bullet)();

void (draw_shields)();

int (draw_all_bullets)();

int (draw_bullet)(Bullet* bullet_obj);

void (draw_enemy_bullets)();

void (update_bullets)();    

void (update_enemy_bullets)();

#endif