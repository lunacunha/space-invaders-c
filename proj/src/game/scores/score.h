#ifndef SCORE_H
#define SCORE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../controllers/keyboard/kb_interrupts.h"
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

int score_state(); 

#endif
