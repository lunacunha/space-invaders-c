#include "./score.h"

extern uint8_t scancode;
extern uint8_t irq_set_keyboard;
extern int timer_counter;

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

