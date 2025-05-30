#ifndef SCORE_H
#define SCORE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../../controllers/keyboard/kb_interrupts.h"
#include "../../controllers/graphics/graphic.h"
#include "../menu/menu.h"

#define MAX_NAME_LENGTH 20
#define CSV_FILENAME "highscores.csv"
#define MAX_HIGHSCORES 100

typedef struct {
    int bullets_fired;
    int game_time_seconds;
    int final_score;
} GameScore;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
    int bullets_fired;
    int game_time_seconds;
} HighScoreEntry;

// Global score variables
extern GameScore current_score;
extern int game_start_time;
extern bool game_active;

// Score functions
void score_init();
void score_add_bullet();
void score_update_time(int timer_ticks);
int score_calculate_final();
void score_reset();

// High score functions
char* get_random_anime_name();
int save_highscore_to_csv(const char* name, int score, int bullets, int time);
int load_highscores_from_csv();
void save_win_highscore(); // NEW: Function to call when player wins

void draw_letter(char letter, int x, int y);
void draw_text(const char* text, int x, int y);
int calculate_text_width(const char* text);
void draw_text_centered(const char* text, int center_x, int y);

void draw_digit(int digit, int x, int y);
int draw_number(int number, int x, int y);
void draw_label_and_number(const char* label, int number, int x, int y);
void draw_label_and_number_centered(const char* label, int number, int center_x, int y);

void draw_live_score();
void draw_final_score_display();

int score_state(); 

#endif
