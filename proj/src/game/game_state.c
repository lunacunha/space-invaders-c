#include "game_state.h"

extern vbe_mode_info_t mode_info;
extern uint16_t x;
extern uint8_t scancode;
extern uint8_t irq_set_keyboard, irq_set_timer;
extern int enemy_move_timer;

bool player_win = false;
bool player_lost = false;

// Check collision between bullet and enemies
int check_bullet_enemy_collision() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) continue;
            
            // Simple bounding box collision
            if (bullets[i].x < enemies[j].x + ENEMY_WIDTH &&
                bullets[i].x + 10 > enemies[j].x &&
                bullets[i].y < enemies[j].y + ENEMY_HEIGHT &&
                bullets[i].y + 10 > enemies[j].y) {
                
                // Hit! Deactivate both bullet and enemy
                bullets[i].active = false;
                enemies[j].active = false;
                
                printf("Enemy hit!\n");
                return 1; // Return 1 to indicate a hit occurred
            }
        }
    }
    return 0;
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
                            ship_action();
                        }
                    }
                    // timer
                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) {   
                        timer_int_handler();  
                        
                        // Clear the entire back buffer first
                        clear_back_buf(0x000000);
                        
                        // Update game logic
                        shoot_bullets();
                        enemies_moving();
                        check_bullet_enemy_collision();
                        
                        // Draw everything to back buffer
                        if (draw_ship(x) != 0) return 1;
                        if (draw_all_enemies() != 0) {
                            printf("Error drawing enemies\n");
                            return 1;
                        }
                        if (draw_all_bullets() != 0) return 1; // Make sure you have this function
                        
                        // Swap buffers once per frame
                        swap_buffers();
                        
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
