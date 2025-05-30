#ifndef GAME_STATE_H
#define GAME_STATE_H

/**
 * @def FREQUENCY
 * @brief Defines the game loop frequency in Hz.
 */
#define FREQUENCY 60

/**
 * @def MODE
 * @brief Defines the graphics mode (0x14C).
 */
#define MODE 0x14c

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "src/controllers/keyboard/kb_interrupts.h"
#include "src/confs.h"
#include "ship/ship.h"
#include "enemies/enemies.h"
#include "menu/menu.h"
#include "scores/score.h"

/**
 * @brief Checks for collisions between bullets and enemies.
 * 
 * @return int Returns 1 if a collision occurred, 0 otherwise.
 */
int (check_bullet_enemy_collision)();

/**
 * @brief Checks for collisions between enemy bullets and other entities.
 */
void (check_enemy_bullet_collision)();

/**
 * @brief Checks for collisions between the player's ship and aliens.
 */
void (check_ship_alien_collision)();

/**
 * @brief Checks for collisions involving shields.
 */
void (check_shield_collision)();

/**
 * @brief Updates enemy movement based on game logic.
 */
void (enemies_moving)();

/**
 * @brief Main game state loop.
 * 
 * @return int Status code: 0 on success, non-zero on error.
 */
int (game_state)();

#endif // GAME_STATE_H
