
extern uint8_t scancode;
extern int timer_counter;
extern vbe_mode_info_t mode_info;
uint8_t irq_set_keyboard, irq_set_timer;

// Initialize enemies in formation





int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}


int player_game_loop(uint16_t x) {
    int prev_x = x; 
    int ship_width = 50; 
    int ship_height = 50;
    int bullet_width = 50;
    int bullet_height = 30;

    int ipc_st;
    message msg;

    // Initialize bullets and enemies
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
    
    if (init_enemies() != 0) {
        printf("Error initializing enemies\n");
        return 1;
    }

    while (scancode != KB_BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_st)) {   
            printf("Error: driver_receive failed with: %d", ipc_st);
            continue;
        }

        if (is_ipc_notify(ipc_st)) {  
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:  
                    if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {  
                        kbc_ih();   

                        bool is_break = scancode & KB_BREAK_CODE; 

                        if (!is_break) { 
                            if (scancode == 0x1e) { // 'A' key (make code)
                                if (x > ship_width) { 
                                    x -= 30; 
                                }
                                printf("Left key pressed, x: %d\n", x);

                            } 
                            else if (scancode == 0x20) { // 'D' key (make code)
                                x += 30;
                                if (x > mode_info.XResolution - ship_width) { 
                                    x = mode_info.XResolution - ship_width;
                                }

                            } 
                            else if (scancode == 0x39) { // Space bar - shoot
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
                    }

                    if (msg.m_notify.interrupts & BIT(irq_set_timer)) {   
                        timer_int_handler();  

                        // Move bullets
                        for (int i = 0; i < MAX_BULLETS; i++) {
                            if (bullets[i].active) {
                                // Clear the previous position of the bullet
                                vg_draw_rectangle(bullets[i].x, bullets[i].y, bullet_width, bullet_height, 0x000000);

                                // Update bullet position
                                bullets[i].y -= 10;

                                // Deactivate bullet if it goes off screen
                                if (bullets[i].y < 0) { 
                                    bullets[i].active = false;
                                } else {
                                    // Draw bullet at new position
                                    if (print_xpm(bullet, bullets[i].x, bullets[i].y) != 0) {
                                        printf("Error drawing bullet\n");
                                        return 1;
                                    }
                                }
                            }
                        }
                        
                        // Move enemies every 30 timer interrupts (0.5 seconds at 60Hz)
                        enemy_move_timer++;
                        if (enemy_move_timer >= 30) {
                            if (move_enemies() != 0) {
                                printf("Error moving enemies\n");
                                return 1;
                            }
                            enemy_move_timer = 0;
                        }
                        
                        // Check for collisions
                        check_bullet_enemy_collision();
                        
                        // Check if all enemies are destroyed
                        if (count_active_enemies() == 0) {
                            printf("All enemies destroyed! You win!\n");
                            // You could reinitialize enemies here for next wave
                            // init_enemies();
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        // Clear and redraw ship only if position changed
        if (prev_x != x) {
            vg_draw_rectangle(prev_x, 600, ship_width, ship_height, 0x000000);
            prev_x = x;
        }

        // Draw ship
        if(draw_ship(prev_x)) return 1;

        // Draw all active enemies
        if (draw_all_enemies() != 0) {
            printf("Error drawing enemies\n");
            return 1;
        }

        // Draw bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                if (print_xpm(bullet, bullets[i].x, bullets[i].y) != 0) {
                    printf("Error drawing bullet\n");
                    return 1;
                }
            }
        }
    }
    return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
    uint16_t x = 350;

    if (init_game() != 0) return close_game();
    if (draw_ship(x) != 0) return 1;
    if (player_game_loop(x) != 0) return 1;
    return close_game();
}