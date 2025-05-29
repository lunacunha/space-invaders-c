#include "ship.h"

uint16_t x = 350;
int ship_width = 50; 
int ship_height = 50;
int bullet_width = 50;
int bullet_height = 30;
extern vbe_mode_info_t mode_info;
extern uint8_t scancode;
extern uint8_t data;
int fire_delay_counter = 150;

// Mouse state variables
static uint8_t mouse_packet_buf[3];
static uint8_t mouse_packet_index = 0;
static struct packet mouse_packet;
static int mouse_x = 400; // Current mouse X position (screen coordinates)
static bool left_button_pressed = false;
static bool prev_left_button = false;

int draw_ship(uint16_t x) {
    if (print_xpm(ship, x, 700) != 0) return 1;
    return 0;
}

void init_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void init_enemy_bullets() {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemy_bullets[i].active = false;
    }
}

void init_shields() {
    int spacing = 200; // Spacing between shields
    int start_x = (mode_info.XResolution - (NUM_SHIELDS * SHIELD_WIDTH + (NUM_SHIELDS - 1) * spacing)) / 2;
    int shield_y = mode_info.YResolution - 250; // Position above the player's ship

    printf("shield_y: %d\n", shield_y);

    for (int i = 0; i < NUM_SHIELDS; i++) {
        shields[i].x = start_x + i * (SHIELD_WIDTH + spacing);
        shields[i].y = shield_y;
        shields[i].state = SHIELD_FULL; // Initialize all shields as full
    }
}

void handle_mouse_input() {
    // Store the mouse byte in the packet buffer
    mouse_packet_buf[mouse_packet_index] = data;
    mouse_packet_index++;
    
    // Check if we have a complete packet (3 bytes)
    if (mouse_packet_index == 3) {
        // Validate first byte (bit 3 should always be 1)
        if (!(mouse_packet_buf[0] & PK_BIT3)) {
            // Invalid packet, reset and try again
            mouse_packet_index = 0;
            return;
        }
        
        // Parse the complete packet
        mouse_parse_packet(&mouse_packet, mouse_packet_buf, 3);
        
        // Update mouse position based on movement
        mouse_x += mouse_packet.delta_x;
        
        // Clamp mouse position to screen bounds
        if (mouse_x < ship_width/2) {
            mouse_x = ship_width/2;
        } else if (mouse_x > mode_info.XResolution - ship_width/2) {
            mouse_x = mode_info.XResolution - ship_width/2;
        }
        
        // Update ship position to follow mouse
        x = mouse_x - ship_width/2;
        
        // Handle mouse click for firing
        prev_left_button = left_button_pressed;
        left_button_pressed = mouse_packet.lb;
        
        // Fire on left button press (not while held)
        if (left_button_pressed && !prev_left_button) {
            if (fire_delay_counter >= 10) { // Reduced delay for mouse firing
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].active) { 
                        bullets[i].x = x + ship_width/2 - 2; // Center the bullet
                        bullets[i].y = 700 - BULLET_HEIGHT;   
                        bullets[i].active = true;
                        break;
                    }
                }
                fire_delay_counter = 0;
            }
        }
        
        // Reset packet index for next packet
        mouse_packet_index = 0;
    }
}

void ship_action() {
    // Keep keyboard controls for compatibility
    if (scancode == KB_A) { // 'A' key (make code)
        if (x > ship_width) { 
            x -= 30; 
            mouse_x = x + ship_width/2; // Update mouse position to match
        }
        printf("Left key pressed, x: %d\n", x);
    } 
    else if (scancode == KB_D) { // 'D' key (make code)
        x += 30;
        if (x > mode_info.XResolution - ship_width) { 
            x = mode_info.XResolution - ship_width;
        }
        mouse_x = x + ship_width/2; // Update mouse position to match
    } 
    else if (scancode == KB_SPACE) { 
        if (fire_delay_counter >= 30) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) { 
                    bullets[i].x = x + ship_width/2 - 2; // Center the bullet
                    bullets[i].y = 700 - BULLET_HEIGHT;   
                    bullets[i].active = true;
                    break;
                }
            }
            fire_delay_counter = 0;
        }
    }
}

void fire_enemy_bullet() {
    int active_enemies = count_active_enemies(); // Count active enemies

    if (active_enemies == 0) return; // No active enemies, no bullets to fire

    // Randomly select an active enemy
    int random_enemy_index = rand() % active_enemies; // Pick a random active enemy
    int active_enemy_count = 0;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            if (active_enemy_count == random_enemy_index) { // Found the randomly selected enemy
                for (int j = 0; j < MAX_ENEMY_BULLETS; j++) {
                    if (!enemy_bullets[j].active) { // Find an inactive bullet
                        enemy_bullets[j].x = enemies[i].x + (ENEMY_WIDTH / 2); // Center bullet horizontally
                        enemy_bullets[j].y = enemies[i].y + ENEMY_HEIGHT;      // Start below the enemy
                        enemy_bullets[j].active = true;
                        printf("Enemy %d fired bullet from (%d, %d)\n", i, enemy_bullets[j].x, enemy_bullets[j].y);
                        return; // Fire only one bullet
                    }
                }
            }
            active_enemy_count++;
        }
    }
}

void draw_shields() {
    for (int i = 0; i < NUM_SHIELDS; i++) {
        switch (shields[i].state) {
            case SHIELD_FULL:
                print_xpm(shield, shields[i].x, shields[i].y); // Full shield XPM
                break;
            case SHIELD_BROKEN:
                print_xpm(break_shield, shields[i].x, shields[i].y); // Broken shield XPM
                break;
            case SHIELD_NONE:
                break;
        }
    }
}

// Draw a single bullet - ONLY use this method
int draw_bullet(Bullet* bullet_obj) {
    if (!bullet_obj->active) return 0;

    // Use the same XPM that was working in your original code
    if (print_xpm(bullet, bullet_obj->x, bullet_obj->y) != 0) {
        return 1;
    }
    return 0;
}

void draw_enemy_bullets() {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemy_bullets[i].active) {
            print_xpm(enemy_bullet, enemy_bullets[i].x, enemy_bullets[i].y);
        }
    }
}

// Draw all active bullets
int draw_all_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            if (draw_bullet(&bullets[i]) != 0) {
                printf("Error drawing bullet %d\n", i);
                return 1;
            }
        }
    }
    return 0;
}

// Update bullet positions - NO DRAWING HERE
void update_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // Just update position
            bullets[i].y -= 20;
            
            // Deactivate if off screen
            if (bullets[i].y < 0) { 
                bullets[i].active = false;
            }
        }
    }
}

void update_enemy_bullets() {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemy_bullets[i].active) {
            enemy_bullets[i].y += 20; // Move bullet downward

            // Deactivate bullet if it goes off-screen
            if (enemy_bullets[i].y > mode_info.YResolution) {
                enemy_bullets[i].active = false;
            }
        }
    }
}
