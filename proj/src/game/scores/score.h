#ifndef SCORE_H
#define SCORE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../../controllers/keyboard/kb_interrupts.h"
#include "../../controllers/graphics/graphic.h"
#include "../menu/menu.h"

typedef struct {
    int bullets_fired;
    int game_time_seconds;
    int final_score;
} GameScore;

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

void draw_digit(int digit, int x, int y);
void draw_number(int number, int x, int y, int digit_spacing);
void draw_live_score();
void draw_final_score_display();

int score_state(); 

#endif
