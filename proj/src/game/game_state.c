#include "game_state.h"

extern vbe_mode_info_t mode_info;
extern uint16_t x;
uint16_t y = 600;
extern uint8_t scancode;
extern uint8_t irq_set_keyboard, irq_set_timer;
extern int enemy_move_timer;
extern int ship_width;
extern int ship_height;
extern int timer_counter;

bool player_win = false;
bool player_lost = false;

// Check collision between bullet and enemies
int check_bullet_enemy_collision() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) continue;
            
            // Proper bounding box collision with correct bullet dimensions
            if (bullets[i].x < enemies[j].x + ENEMY_WIDTH &&
                bullets[i].x + BULLET_WIDTH > enemies[j].x &&     // Use actual bullet width
                bullets[i].y < enemies[j].y + ENEMY_HEIGHT &&
                bullets[i].y + BULLET_HEIGHT > enemies[j].y) {    // Use actual bullet height
                
                // Hit! Deactivate both bullet and enemy
                bullets[i].active = false;
                enemies[j].active = false;
                
                printf("Enemy hit! Bullet (%d,%d) hit enemy (%d,%d)\n", 
                       bullets[i].x, bullets[i].y, enemies[j].x, enemies[j].y);
                return 1; // Return 1 to indicate a hit occurred
            }
        }
    }
    return 0;
}

void check_enemy_bullet_collision() {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!enemy_bullets[i].active) continue; // Skip inactive bullets

        // Check collision with the player's ship
        if (enemy_bullets[i].x < x + ship_width &&  
            enemy_bullets[i].x + BULLET_WIDTH > x && 
            enemy_bullets[i].y < y + ship_height && 
            enemy_bullets[i].y + BULLET_HEIGHT > y) { 
            
            // Collision detected
            enemy_bullets[i].active = false; // Deactivate the bullet
            player_lost = true; // Mark the player as lost
            printf("Player hit by enemy bullet at (%d, %d)!\n", enemy_bullets[i].x, enemy_bullets[i].y);
        }
    }
}

void enemies_moving() {
    enemy_move_timer++;
    if (enemy_move_timer >= 30) {
        move_enemies();
        enemy_move_timer = 0;
    }
}

int game_state() {
    int ipc_st;
    message msg;
    
    while (scancode != KB_BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_st)) {   
            printf("Error: driver_receive failed with: %d", ipc_st);
            continue;
        }

        if (is_ipc_notify(ipc_st)) {  
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:  
                    // keyboard
                    if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {  
                        kbc_ih();   

                        bool is_break = scancode & KB_BREAK_CODE; 

                        if (!is_break) { 
                            ship_action(); // This now only updates position/shoots, no drawing
                        }
                    }
                    // timer
                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) {   
                        timer_int_handler();  
                        
                        // Clear the entire back buffer first
                        clear_back_buf(0x000000);
                        
                        // Update game logic (no drawing in these functions)
                        update_bullets(); // Use the new function name
                        enemies_moving();

                        if (timer_counter % 60 == 0) { // Fire every 120 frames
                            printf("fire\n");
                            fire_enemy_bullet();
                        }
                        update_enemy_bullets();

                        check_enemy_bullet_collision();
                        check_bullet_enemy_collision();
                        
                        // Draw everything to back buffer in correct order
                        if (draw_ship(x) != 0) return 1;
                        if (draw_all_enemies() != 0) {
                            printf("Error drawing enemies\n");
                            return 1;
                        }
                        if (draw_all_bullets() != 0) return 1;
                        draw_enemy_bullets();
                        
                        // Swap buffers once per frame
                        swap_buffers();

                        // Check if the player has lost
                        if (player_lost) {
                            menu_set_state(MENU_SCORES);
                            return 0; // Exit game loop on player loss
                        }
                        
                        // Check win condition
                        if (count_active_enemies() == 0) {
                            player_win = true;
                            menu_set_state(MENU_SCORES);
                            return 0;
                        }

                        
                    }
                    break;

                default:
                    break;
            }
        }
    }
    menu_set_state(MENU_MAIN);
    return 0;
}
