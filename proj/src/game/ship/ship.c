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
    int prev_x = x; 

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
                bullets[i].x = x;    
                bullets[i].y = 570;   
                bullets[i].active = true;
                break;
            }
        }


    }

    // Limpar apenas a área onde a nave estava antes
    if (prev_x != x) { // Apenas limpe e redesenhe se a posição mudou
        vg_draw_rectangle(prev_x, 600, ship_width, ship_height, 0x000000);
        prev_x = x;
    }

    // Redesenhar a nave na nova posição
    draw_ship(prev_x);

    // Desenhar balas
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            print_xpm(bullet, bullets[i].x, bullets[i].y);
        }
    }
}


void shoot_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // Limpar a posição anterior da bala
            vg_draw_rectangle(bullets[i].x, bullets[i].y, bullet_width, bullet_height, 0x000000);
        // Atualizar a posição da bala
            bullets[i].y -= 10;
        // Desativar a bala se sair da tela
            if (bullets[i].y < 0) { 
                bullets[i].active = false;
            } else {
                // Redesenhar a bala na nova posição
                print_xpm(bullet, bullets[i].x, bullets[i].y);
            }
        }
    }

}
