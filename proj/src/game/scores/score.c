#include "score.h"

extern uint8_t scancode;
extern uint8_t irq_set_keyboard;
extern int timer_counter;
extern vbe_mode_info_t mode_info;

// Include the XPMs
#include "../../xpm/characters/number_char.h"
#include "../../xpm/characters/letter_char.h"

// Import XPM arrays
static xpm_map_t numbers[] = {
    n0, n1, n2, n3, n4, n5, n6, n7, n8, n9
};

static xpm_map_t letters[] = {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

#define CHAR_WIDTH 20    
#define CHAR_HEIGHT 25   
#define CHAR_SPACING 22  

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
    // Base: 10000 points
    // -50 per bullet, -10 per second
    // Minimum: 100 points
    
    int base_score = 10000;
    int bullet_penalty = current_score.bullets_fired * 50;
    int time_penalty = current_score.game_time_seconds * 10;
    
    current_score.final_score = base_score - bullet_penalty - time_penalty;
    
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

// Draw single letter at position
void draw_letter(char letter, int x, int y) {
    if (letter >= 'A' && letter <= 'Z') {
        print_xpm(letters[letter - 'A'], x, y);
    } else if (letter >= 'a' && letter <= 'z') {
        print_xpm(letters[letter - 'a'], x, y);
    }
    // Ignore spaces and other characters
}

// Draw text string
void draw_text(const char* text, int x, int y) {
    int current_x = x;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == ' ') {
            current_x += CHAR_SPACING;
        } else {
            draw_letter(text[i], current_x, y);
            current_x += CHAR_SPACING;
        }
    }
}

// Get text width for centering
int calculate_text_width(const char* text) {
    return strlen(text) * CHAR_SPACING;
}

// Draw text centered at x position
void draw_text_centered(const char* text, int center_x, int y) {
    int text_width = calculate_text_width(text);
    int start_x = center_x - (text_width / 2);
    draw_text(text, start_x, y);
}

void draw_digit(int digit, int x, int y) {
    if (digit >= 0 && digit <= 9) {
        print_xpm(numbers[digit], x, y);
    }
}

// Draw number, returns end x position
int draw_number(int number, int x, int y) {
    if (number == 0) {
        draw_digit(0, x, y);
        return x + CHAR_SPACING;
    }
    
    char num_str[12];
    sprintf(num_str, "%d", number);
    
    int len = strlen(num_str);
    int current_x = x;
    for (int i = 0; i < len; i++) {
        int digit = num_str[i] - '0';
        draw_digit(digit, current_x, y);
        current_x += CHAR_SPACING;
    }
    return current_x;
}

// Draw label and number together
void draw_label_and_number(const char* label, int number, int x, int y) {
    draw_text(label, x, y);
    int label_width = calculate_text_width(label);
    draw_number(number, x + label_width, y);
}

// Draw label and number centered
void draw_label_and_number_centered(const char* label, int number, int center_x, int y) {
    char num_str[12];
    sprintf(num_str, "%d", number);
    int num_width = strlen(num_str) * CHAR_SPACING;
    int label_width = calculate_text_width(label);
    int total_width = label_width + num_width;
    
    int start_x = center_x - (total_width / 2);
    draw_text(label, start_x, y);
    draw_number(number, start_x + label_width, y);
}

// Draw current score during gameplay
void draw_live_score() {
    // Calculate live score using same formula
    int base_score = 10000;
    int bullet_penalty = current_score.bullets_fired * 50;
    int time_penalty = current_score.game_time_seconds * 10;
    int live_score = base_score - bullet_penalty - time_penalty;
    
    if (live_score < 100) {
        live_score = 100;
    }
    
    int margin = 20;
    int top_y = margin;
    
    // Left: Score
    draw_label_and_number("SCORE ", live_score, margin, top_y);
    
    // Center: Time
    int center_x = mode_info.XResolution / 2;
    draw_label_and_number_centered("TIME ", current_score.game_time_seconds, center_x, top_y);
    
    // Right: Shots
    char shots_text[] = "SHOTS ";
    char shots_num[12];
    sprintf(shots_num, "%d", current_score.bullets_fired);
    int shots_total_width = calculate_text_width(shots_text) + (strlen(shots_num) * CHAR_SPACING);
    int shots_x = mode_info.XResolution - shots_total_width - margin;
    draw_label_and_number(shots_text, current_score.bullets_fired, shots_x, top_y);
}

// Draw single letter at 2x scale
void draw_letter_2x(char letter, int x, int y) {
    if (letter >= 'A' && letter <= 'Z') {
        // Draw the letter 4 times in a 2x2 pattern for 2x scale
        print_xpm(letters[letter - 'A'], x, y);
        print_xpm(letters[letter - 'A'], x + CHAR_WIDTH, y);
        print_xpm(letters[letter - 'A'], x, y + CHAR_HEIGHT);
        print_xpm(letters[letter - 'A'], x + CHAR_WIDTH, y + CHAR_HEIGHT);
    } else if (letter >= 'a' && letter <= 'z') {
        print_xpm(letters[letter - 'a'], x, y);
        print_xpm(letters[letter - 'a'], x + CHAR_WIDTH, y);
        print_xpm(letters[letter - 'a'], x, y + CHAR_HEIGHT);
        print_xpm(letters[letter - 'a'], x + CHAR_WIDTH, y + CHAR_HEIGHT);
    }
}

// Draw final score screen
void draw_final_score_display() {
    int center_x = mode_info.XResolution / 2;
    int center_y = mode_info.YResolution / 2;
    int line_spacing = CHAR_HEIGHT + 15;
    
    // Start higher to accommodate "YOU WON" message
    int start_y = center_y - (line_spacing * 2);
    
    // Display "YOU WON" at the top
    draw_text_centered("YOU WON", center_x, start_y);
    
    // Display the scores below
    draw_label_and_number_centered("FINAL SCORE ", current_score.final_score, center_x, start_y + line_spacing);
    draw_label_and_number_centered("TIME ", current_score.game_time_seconds, center_x, start_y + (line_spacing * 2));
    draw_label_and_number_centered("SHOTS FIRED ", current_score.bullets_fired, center_x, start_y + (line_spacing * 3));
    
    // Show efficiency percentage
    if (current_score.bullets_fired > 0) {
        int efficiency = (current_score.final_score * 100) / 10000;
        if (efficiency > 100) efficiency = 100;
        draw_label_and_number_centered("EFFICIENCY ", efficiency, center_x, start_y + (line_spacing * 4));
        draw_text_centered("PERCENT", center_x, start_y + (line_spacing * 4) + CHAR_HEIGHT + 5);
    }
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
