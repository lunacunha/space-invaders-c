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
#define CSV_FILENAME "./highscores.csv"
#define MAX_HIGHSCORES 100
#define MAX_DISPLAY_SCORES 10

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

extern GameScore current_score;
extern int game_start_time;
extern bool game_active;

extern HighScoreEntry highscores[MAX_HIGHSCORES];
extern int highscore_count;

/**
 * @brief Initialize the scoring system
 */
void score_init();

/**
 * @brief Increment the bullet counter
 */
void score_add_bullet();

/**
 * @brief Update game time based on timer ticks
 * @param timer_ticks Number of timer interrupts elapsed
 */
void score_update_time(int timer_ticks);

/**
 * @brief Calculate the final game score
 * @return Final calculated score value
 */
int score_calculate_final();

/**
 * @brief Reset all score values to initial state
 */
void score_reset();

/**
 * @brief Generate a random anime character name
 * @return Pointer to randomly selected name string
 */
char* get_random_anime_name();

/**
 * @brief Save a high score entry to CSV file
 * @param name Player name
 * @param score Final score achieved
 * @param bullets Number of bullets fired
 * @param time Game duration in seconds
 * @return 0 on success, non-zero on failure
 */
int save_highscore_to_csv(const char* name, int score, int bullets, int time);

/**
 * @brief Load high scores from CSV file into memory
 * @return 0 on success, non-zero on failure
 */
int load_highscores_from_csv();

/**
 * @brief Save current game as winning high score
 */
void save_win_highscore();

/**
 * @brief Sort high scores in descending order
 */
void sort_highscores();

/**
 * @brief Draw the high scores menu display
 */
void draw_scoreboard_menu();

/**
 * @brief Draw a single letter at specified coordinates
 * @param letter Character to draw
 * @param x X coordinate
 * @param y Y coordinate
 */
void draw_letter(char letter, int x, int y);

/**
 * @brief Draw text string at specified coordinates
 * @param text String to draw
 * @param x X coordinate
 * @param y Y coordinate
 */
void draw_text(const char* text, int x, int y);

/**
 * @brief Calculate pixel width of text string
 * @param text String to measure
 * @return Width in pixels
 */
int calculate_text_width(const char* text);

/**
 * @brief Draw text centered around specified X coordinate
 * @param text String to draw
 * @param center_x Center X coordinate
 * @param y Y coordinate
 */
void draw_text_centered(const char* text, int center_x, int y);

/**
 * @brief Draw a single digit at specified coordinates
 * @param digit Digit (0-9) to draw
 * @param x X coordinate
 * @param y Y coordinate
 */
void draw_digit(int digit, int x, int y);

/**
 * @brief Draw a number at specified coordinates
 * @param number Integer to draw
 * @param x X coordinate
 * @param y Y coordinate
 * @return Width of drawn number in pixels
 */
int draw_number(int number, int x, int y);

/**
 * @brief Draw label followed by number
 * @param label Text label to display
 * @param number Number to display
 * @param x X coordinate
 * @param y Y coordinate
 */
void draw_label_and_number(const char* label, int number, int x, int y);

/**
 * @brief Draw centered label and number
 * @param label Text label to display
 * @param number Number to display
 * @param center_x Center X coordinate
 * @param y Y coordinate
 */
void draw_label_and_number_centered(const char* label, int number, int center_x, int y);

/**
 * @brief Draw live score display during gameplay
 */
void draw_live_score();

/**
 * @brief Draw final score screen
 */
void draw_final_score_display();

/**
 * @brief Get current score system state
 * @return Current state identifier
 */
int score_state();

#endif
