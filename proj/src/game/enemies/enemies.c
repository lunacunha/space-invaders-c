#include "enemies.h"
extern vbe_mode_info_t mode_info;
int enemy_direction = 1; // 1 for right, -1 for left
int enemy_move_timer = 0;
int enemy_drop_timer = 0;
bool should_drop = false;


int init_enemies() {
    int enemy_index = 0;
    
    for (int row = 0; row < ENEMY_ROWS; row++) {
        for (int col = 0; col < ENEMY_COLS; col++) {
            if (enemy_index < MAX_ENEMIES) {
                enemies[enemy_index].x = ENEMY_START_X + (col * ENEMY_SPACING_X);
                enemies[enemy_index].y = ENEMY_START_Y + (row * ENEMY_SPACING_Y);
                enemies[enemy_index].active = true;
                enemies[enemy_index].type = row; // Different types for different rows
                enemy_index++;
            }
        }
    }
    
    return 0;
}


// Draw a single enemy
int draw_enemy(Enemy* enemy) {
    if (!enemy->active) return 0;
    
    // Use different alien sprites based on enemy type
    switch (enemy->type) {
        case 0:
            return print_xpm(Alien1, enemy->x, enemy->y);
        case 1:
            return print_xpm(Alien2, enemy->x, enemy->y);
        case 2:
            return print_xpm(Alien3, enemy->x, enemy->y);
        default:
            return print_xpm(Alien1, enemy->x, enemy->y);
    }
}

// Draw all active enemies
int draw_all_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            if (draw_enemy(&enemies[i]) != 0) {
                printf("Error drawing enemy %d\n", i);
                return 1;
            }
        }
    }
    return 0;
}

// Clear enemy from screen
int clear_enemy(Enemy* enemy) {
    if (!enemy->active) return 0;
    return vg_draw_rectangle(enemy->x, enemy->y, ENEMY_WIDTH, ENEMY_HEIGHT, 0x000000);
}

// Clear all enemies from screen
int clear_all_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            if (clear_enemy(&enemies[i]) != 0) {
                printf("Error clearing enemy %d\n", i);
                return 1;
            }
        }
    }
    return 0;
}

// Move enemies in formation
int move_enemies() {
    bool hit_edge = false;
    
    // Check if any enemy hits the screen edge
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            if ((enemy_direction > 0 && enemies[i].x + ENEMY_WIDTH >= mode_info.XResolution - 20) ||
                (enemy_direction < 0 && enemies[i].x <= 20)) {
                hit_edge = true;
                break;
            }
        }
    }
    
    // If hit edge, change direction and mark for dropping down
    if (hit_edge) {
        enemy_direction *= -1;
        should_drop = true;
        return 0;
    }
    
    // Clear all enemies from current positions
    clear_all_enemies();
    
    // Move all enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            if (should_drop) {
                enemies[i].y += 20; // Drop down
            } else {
                enemies[i].x += enemy_direction * 10; // Move horizontally
            }
        }
    }
    
    if (should_drop) {
        should_drop = false;
    }
    
    return 0;
}

// Count active enemies
int count_active_enemies() {
    int count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) count++;
    }
    return count;
}
