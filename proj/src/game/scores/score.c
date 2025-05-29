#include "score.h"

extern uint8_t scancode;
extern uint8_t irq_set_keyboard;
extern int timer_counter;
extern vbe_mode_info_t mode_info;

#include "../../xpm/characters/number_char.h"

static xpm_map_t numbers[] = {
    n0, n1, n2, n3, n4, n5, n6, n7, n8, n9
};

// Global score tracking
GameScore current_score = {0, 0, 0};
int game_start_time = 0;
bool game_active = false;

void score_init() {
    current_score.bullets_fired = 0;
    current_score.game_time_seconds = 0;
    current_score.final_score = 0;
    game_start_time = timer_counter;
    game_active = true;
    printf("Score system initialized. Start time: %d\n", game_start_time);
}

void score_add_bullet() {
    if (game_active) {
        current_score.bullets_fired++;
        printf("Bullet fired! Total: %d\n", current_score.bullets_fired);
    }
}

void score_update_time(int timer_ticks) {
    if (game_active) {
        // Convert timer ticks to seconds (assuming 60 FPS)
        current_score.game_time_seconds = (timer_ticks - game_start_time) / 60;
    }
}

int score_calculate_final() {
    if (!game_active) return current_score.final_score;
    
    // Score calculation:
    // Base score: 10000 points
    // Penalty: -50 points per bullet fired
    // Penalty: -10 points per second elapsed
    // Minimum score: 100 points
    
    int base_score = 10000;
    int bullet_penalty = current_score.bullets_fired * 50;
    int time_penalty = current_score.game_time_seconds * 10;
    
    current_score.final_score = base_score - bullet_penalty - time_penalty;
    
    // Ensure minimum score
    if (current_score.final_score < 100) {
        current_score.final_score = 100;
    }
    
    printf("Final Score Calculation:\n");
    printf("Base: %d, Bullet penalty: %d, Time penalty: %d\n", 
           base_score, bullet_penalty, time_penalty);
    printf("Final Score: %d\n", current_score.final_score);
    
    game_active = false;
    return current_score.final_score;
}

void score_reset() {
    current_score.bullets_fired = 0;
    current_score.game_time_seconds = 0;
    current_score.final_score = 0;
    game_start_time = 0;
    game_active = false;
}


void draw_digit(int digit, int x, int y) {
    if (digit >= 0 && digit <= 9) {
        print_xpm(numbers[digit], x, y);
    }
}


void draw_number(int number, int x, int y, int digit_spacing) {
    if (number == 0) {
        draw_digit(0, x, y);
        return;
    }
    
    // Convert number to string to get digits
    char num_str[12]; // Enough for large integers
    sprintf(num_str, "%d", number);
    
    int len = strlen(num_str);
    for (int i = 0; i < len; i++) {
        int digit = num_str[i] - '0';
        draw_digit(digit, x + (i * digit_spacing), y);
    }
}


void draw_live_score() {
    // Define positions for score elements
    int score_x = 50;           // Left side of screen
    int score_y = 30;           // Top of screen
    int digit_spacing = 25;     // Space between digits
    
    // Calculate current live score (same formula as final score)
    int base_score = 10000;
    int bullet_penalty = current_score.bullets_fired * 50;
    int time_penalty = current_score.game_time_seconds * 10;
    int live_score = base_score - bullet_penalty - time_penalty;
    
    if (live_score < 100) {
        live_score = 100;
    }
    
    // Draw "SCORE:"
    // For now, just draw the numbers
    draw_number(live_score, score_x, score_y, digit_spacing);
    
    // Optionally draw other stats
    // Bullets fired at top right
    int bullets_x = mode_info.XResolution - 200;
    draw_number(current_score.bullets_fired, bullets_x, score_y, digit_spacing);
    
    // Time at top center
    int time_x = (mode_info.XResolution / 2) - 50;
    draw_number(current_score.game_time_seconds, time_x, score_y, digit_spacing);
}

void draw_final_score_display() {
    int center_x = mode_info.XResolution / 2;
    int center_y = mode_info.YResolution / 2;
    int digit_spacing = 30;
    
    // Draw final score (centered)
    int score_width = 6 * digit_spacing; // Estimate width for centering
    int final_score_x = center_x - (score_width / 2);
    draw_number(current_score.final_score, final_score_x, center_y - 50, digit_spacing);
    
    // Draw bullets fired below
    draw_number(current_score.bullets_fired, final_score_x, center_y, digit_spacing);
    
    // Draw time below that
    draw_number(current_score.game_time_seconds, final_score_x, center_y + 50, digit_spacing);
}

int score_state() {
    int ipc_status;
    message msg;

    while (scancode != KB_BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_status)) {
            printf("Error: driver_receive failed with: %d", ipc_status);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // keyboard
                    if (msg.m_notify.interrupts & BIT(irq_set_keyboard)) {
                        kbc_ih();
                        
                        if (scancode == KB_ENTER || scancode == KB_BREAK_ESC) {
                            return 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}
