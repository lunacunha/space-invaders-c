
#ifndef ENEMIES_H
#define ENEMIES_H

#include <lcom/lcf.h>
#include "src/controllers/graphics/graphic.h"
#include "src/xpm/game/aliens.h"
#include "src/xpm/messages.h"

#define FREQUENCY 60
#define MODE 0x14c
#define MAX_ENEMIES 15
#define ENEMY_ROWS 3
#define ENEMY_COLS 5
#define ENEMY_WIDTH 40
#define ENEMY_HEIGHT 40
#define ENEMY_SPACING_X 80
#define ENEMY_SPACING_Y 60
#define ENEMY_START_X 100
#define ENEMY_START_Y 50

typedef struct {
    int x;
    int y;
    bool active;
    int type; // Different enemy types (0, 1, 2 for different rows)
} Enemy;

Enemy enemies[MAX_ENEMIES];


int (init_enemies)();

// Draw a single enemy
int (draw_enemy)(Enemy* enemy);

// Draw all active enemies
int (draw_all_enemies)();

// Clear enemy from screen
int (clear_enemy)(Enemy* enemy);

// Clear all enemies from screen
int (clear_all_enemies)();

// Move enemies in formation
int (move_enemies)();

// Count active enemies
int (count_active_enemies)();

#endif
