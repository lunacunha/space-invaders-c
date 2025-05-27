#include "ship.h"
uint16_t x = 350;
int ship_width = 50; 
int ship_height = 50;
int bullet_width = 50;
int bullet_height = 30;
extern vbe_mode_info_t mode_info;
extern uint8_t scancode;

int draw_ship(uint16_t x) {
    if (print_xpm(ship, x, 600) != 0) return 1;
    return 0;
}

void init_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void ship_action() {
    if (scancode == KB_A) { // 'A' key (make code)
        if (x > ship_width) { 
            x -= 30; 
        }
        printf("Left key pressed, x: %d\n", x);
    } 
    else if (scancode == KB_D) { // 'D' key (make code)
        x += 30;
        if (x > mode_info.XResolution - ship_width) { 
            x = mode_info.XResolution - ship_width;
        }
    } 
    else if (scancode == KB_SPACE) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) { 
                bullets[i].x = x + ship_width/2 - 2; // Center the bullet
                bullets[i].y = 570;   
                bullets[i].active = true;
                break;
            }
        }
    }
    // Remove all drawing code from here - let the main loop handle it
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
            bullets[i].y -= 10;
            
            // Deactivate if off screen
            if (bullets[i].y < 0) { 
                bullets[i].active = false;
            }
        }
    }
}


// Remove the old shoot_bullets function - replace with update_bullets
