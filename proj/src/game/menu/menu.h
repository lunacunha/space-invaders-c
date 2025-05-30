#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "src/controllers/keyboard/kb_controller.h"
#include "src/xpm/menu/options.h"
#include "src/xpm/menu/title.h"
#include "src/game/enemies/enemies.h"
#include "src/game/ship/ship.h"
#include "src/controllers/graphics/graphic.h"
#include "src/game/scores/score.h"

typedef enum {
    MENU_MAIN,
    MENU_START,
    MENU_SCORES,
    MENU_GAME_OVER,
    MENU_QUIT,
    MENU_EXIT
} MenuState;

typedef enum {
    MENU_OPTION_START,
    MENU_OPTION_SCORES,
    MENU_OPTION_QUIT,
    MENU_OPTION_NONE
} MenuOption;

/**
 * @brief Initialize the menu system
 */
void (menu_init)();

/**
 * @brief Process keyboard input for menu navigation
 */
void (menu_handle_input)();

/**
 * @brief Render the current menu screen
 */
void (menu_render)();

/**
 * @brief Get the current menu state
 * @return Current MenuState value
 */
MenuState (menu_get_state)();

/**
 * @brief Set the menu to a specific state
 * @param state MenuState to transition to
 */
void (menu_set_state)(MenuState state);

#endif
