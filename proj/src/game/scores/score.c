#include "score.h"

extern uint8_t scancode;
extern uint8_t irq_set_keyboard;
extern int timer_counter;
extern vbe_mode_info_t mode_info;

#include "../../xpm/characters/number_char.h"
#include "../../xpm/characters/letter_char.h"

static xpm_map_t numbers[] = {
    n0, n1, n2, n3, n4, n5, n6, n7, n8, n9
};

static xpm_map_t letters[] = {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

static const char* anime_names[] = {
    "MUGEN",
    "JIN", 
    "FUU",
    "SARA",
    "SHIREN",
    "YUKIMARU",
    "KOZA",
    "UMANOSUKE",
    "KARIYA",
    "OKURU",
    "SPIKE",
    "JET",
    "FAYE",
    "EDWARD",
    "VICIOUS",
    "JULIA",
    "GREN",
    "PIERCE",
    "RHINT",
    "ASIMOV",
    "KATERINA",
    "ROCO",
    "MILES",
    "ZEBRA",
    "RASHID",
    "UDAI"
};

#define NUM_ANIME_NAMES (sizeof(anime_names) / sizeof(anime_names[0]))
#define CHAR_WIDTH 20    
#define CHAR_HEIGHT 25   
#define CHAR_SPACING 22  

GameScore current_score = {0, 0, 0};
int game_start_time = 0;
bool game_active = false;
static bool highscore_saved_this_game = false;
HighScoreEntry highscores[MAX_HIGHSCORES];
int highscore_count = 0;

void score_init() {
    current_score.bullets_fired = 0;
    current_score.game_time_seconds = 0;
    current_score.final_score = 0;
    game_start_time = timer_counter;
    game_active = true;
    highscore_saved_this_game = false;
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
        current_score.game_time_seconds = (timer_ticks - game_start_time) / 60;
    }
}

int score_calculate_final() {
    if (!game_active) return current_score.final_score;
    
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
    highscore_saved_this_game = false;
}

char* get_random_anime_name() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    
    int index = rand() % NUM_ANIME_NAMES;
    printf("Selected character: %s (index %d of %d)\n", anime_names[index], index, NUM_ANIME_NAMES);
    return (char*)anime_names[index];
}

int save_highscore_to_csv(const char* name, int score, int bullets, int time) {
    printf("Attempting to save highscore to CSV...\n");
    printf("Name: %s, Score: %d, Bullets: %d, Time: %d\n", name, score, bullets, time);
    
    FILE* test_file = fopen(CSV_FILENAME, "r");
    bool file_exists = (test_file != NULL);
    if (test_file) {
        fclose(test_file);
        printf("CSV file already exists\n");
    } else {
        printf("CSV file doesn't exist, will create it\n");
    }
    
    FILE* file = fopen(CSV_FILENAME, "a");
    if (file == NULL) {
        printf("Failed to open file in append mode, trying write mode...\n");
        file = fopen(CSV_FILENAME, "w");
        if (file == NULL) {
            printf("Error: Could not create highscores file at all!\n");
            printf("Current working directory issue or permission problem\n");
            return -1;
        }
        file_exists = false;
    }
    
    if (!file_exists) {
        printf("Writing CSV header...\n");
        fprintf(file, "Name,Score,Bullets_Fired,Game_Time_Seconds\n");
        fflush(file);
    }
    
    printf("Writing highscore entry...\n");
    int result = fprintf(file, "%s,%d,%d,%d\n", name, score, bullets, time);
    if (result < 0) {
        printf("Error writing to file!\n");
        fclose(file);
        return -1;
    }
    
    fflush(file);
    fclose(file);
    
    printf("Highscore successfully saved: %s - %d points\n", name, score);
    
    if (highscore_count < MAX_HIGHSCORES) {
        strcpy(highscores[highscore_count].name, name);
        highscores[highscore_count].score = score;
        highscores[highscore_count].bullets_fired = bullets;
        highscores[highscore_count].game_time_seconds = time;
        highscore_count++;
        
        sort_highscores();
        printf("Score added to in-memory array and re-sorted\n");
    }
    
    return 0;
}

int load_highscores_from_csv() {
    printf("Attempting to load highscores from: %s\n", CSV_FILENAME);
    FILE* file = fopen(CSV_FILENAME, "r");
    if (file == NULL) {
        printf("No highscores file found, will create on first save\n");
        highscore_count = 0;
        return 0;
    }
    
    char line[256];
    highscore_count = 0;
    
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Empty file or read error\n");
        fclose(file);
        return 0;
    }
    
    printf("=== LOADING HIGH SCORES ===\n");
    while (fgets(line, sizeof(line), file) && highscore_count < MAX_HIGHSCORES) {
        char name[MAX_NAME_LENGTH];
        int score, bullets, time;
        
        if (sscanf(line, "%19[^,],%d,%d,%d", name, &score, &bullets, &time) == 4) {
            strcpy(highscores[highscore_count].name, name);
            highscores[highscore_count].score = score;
            highscores[highscore_count].bullets_fired = bullets;
            highscores[highscore_count].game_time_seconds = time;
            
            printf("Loaded: %s - %d points (%d bullets, %d seconds)\n", 
                   name, score, bullets, time);
            highscore_count++;
        }
    }
    
    fclose(file);
    
    sort_highscores();
    
    printf("Loaded %d high scores\n", highscore_count);
    return highscore_count;
}

void save_win_highscore() {
    if (!highscore_saved_this_game) {
        char* winner_name = get_random_anime_name();
        printf("Player won! Saving highscore for %s\n", winner_name);
        save_highscore_to_csv(winner_name, current_score.final_score, 
                             current_score.bullets_fired, current_score.game_time_seconds);
        highscore_saved_this_game = true;
    } else {
        printf("Highscore already saved for this game\n");
    }
}

void draw_letter(char letter, int x, int y) {
    if (letter >= 'A' && letter <= 'Z') {
        print_xpm(letters[letter - 'A'], x, y);
    } else if (letter >= 'a' && letter <= 'z') {
        print_xpm(letters[letter - 'a'], x, y);
    }
}

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

int calculate_text_width(const char* text) {
    return strlen(text) * CHAR_SPACING;
}

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

void draw_label_and_number(const char* label, int number, int x, int y) {
    draw_text(label, x, y);
    int label_width = calculate_text_width(label);
    draw_number(number, x + label_width, y);
}

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

void draw_live_score() {
    int base_score = 10000;
    int bullet_penalty = current_score.bullets_fired * 50;
    int time_penalty = current_score.game_time_seconds * 10;
    int live_score = base_score - bullet_penalty - time_penalty;
    
    if (live_score < 100) {
        live_score = 100;
    }
    
    int margin = 20;
    int top_y = margin;
    
    draw_label_and_number("SCORE ", live_score, margin, top_y);
    
    int center_x = mode_info.XResolution / 2;
    draw_label_and_number_centered("TIME ", current_score.game_time_seconds, center_x, top_y);
    
    char shots_text[] = "SHOTS ";
    char shots_num[12];
    sprintf(shots_num, "%d", current_score.bullets_fired);
    int shots_total_width = calculate_text_width(shots_text) + (strlen(shots_num) * CHAR_SPACING);
    int shots_x = mode_info.XResolution - shots_total_width - margin;
    draw_label_and_number(shots_text, current_score.bullets_fired, shots_x, top_y);
}

void draw_final_score_display() {
    int center_x = mode_info.XResolution / 2;
    int center_y = mode_info.YResolution / 2;
    int line_spacing = CHAR_HEIGHT + 15;
    
    int start_y = center_y - (line_spacing * 3);
    
    draw_text_centered("YOU WON", center_x, start_y);
    
    static char assigned_name[MAX_NAME_LENGTH] = "";
    if (assigned_name[0] == '\0') {
        strcpy(assigned_name, "UNKNOWN");
    }
    
    draw_text_centered("PILOT", center_x - 60, start_y + line_spacing);
    draw_text_centered(assigned_name, center_x + 60, start_y + line_spacing);
    
    draw_label_and_number_centered("FINAL SCORE ", current_score.final_score, center_x, start_y + (line_spacing * 2));
    draw_label_and_number_centered("TIME ", current_score.game_time_seconds, center_x, start_y + (line_spacing * 3));
    draw_label_and_number_centered("SHOTS FIRED ", current_score.bullets_fired, center_x, start_y + (line_spacing * 4));
    
    if (current_score.bullets_fired > 0) {
        int efficiency = (current_score.final_score * 100) / 10000;
        if (efficiency > 100) efficiency = 100;
        draw_label_and_number_centered("EFFICIENCY ", efficiency, center_x, start_y + (line_spacing * 5));
    }
    
    draw_text_centered("PRESS ENTER", center_x, start_y + (line_spacing * 6));
}

void sort_highscores() {
    if (highscore_count <= 1) return;
    
    for (int i = 0; i < highscore_count - 1; i++) {
        for (int j = 0; j < highscore_count - i - 1; j++) {
            if (highscores[j].score < highscores[j + 1].score) {
                HighScoreEntry temp = highscores[j];
                highscores[j] = highscores[j + 1];
                highscores[j + 1] = temp;
            }
        }
    }
    
    printf("High scores sorted by highest score first\n");
}

void draw_scoreboard_menu() {
    extern vbe_mode_info_t mode_info;
    
    int center_x = mode_info.XResolution / 2;
    int start_y = 80;
    int line_height = 40;
    
    draw_text_centered("HIGH SCORES", center_x, start_y);
    
    int title_y = start_y + 50;
    
    int display_count = (highscore_count < 8) ? highscore_count : 8;
    
    if (display_count == 0) {
        draw_text_centered("NO HIGH SCORES YET", center_x, title_y + line_height * 2);
        draw_text_centered("PLAY A GAME TO SET YOUR FIRST SCORE", center_x, title_y + line_height * 3);
    } else {
        draw_text("RANK", 80, title_y);
        draw_text("PILOT", 200, title_y);
        draw_text("SCORE", 400, title_y);
        draw_text("SHOTS", 550, title_y);
        draw_text("TIME", 700, title_y);
        
        for (int i = 0; i < display_count; i++) {
            int row_y = title_y + ((i + 1) * line_height);
            
            char rank_str[8];
            if (i == 0) sprintf(rank_str, "1ST");
            else if (i == 1) sprintf(rank_str, "2ND");
            else if (i == 2) sprintf(rank_str, "3RD");
            else sprintf(rank_str, "%dTH", i + 1);
            draw_text(rank_str, 80, row_y);
            
            draw_text(highscores[i].name, 200, row_y);
            
            draw_number(highscores[i].score, 400, row_y);
            
            draw_number(highscores[i].bullets_fired, 550, row_y);
            
            draw_number(highscores[i].game_time_seconds, 700, row_y);
            draw_text("S", 700 + (calculate_text_width("999")), row_y);
        }
    }
    
    int instruction_y = mode_info.YResolution - 80;
    draw_text_centered("PRESS ESC TO RETURN TO MENU", center_x, instruction_y);
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
