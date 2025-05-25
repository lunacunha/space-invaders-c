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

/**
 * @brief Estados possíveis do menu
 */
typedef enum {
    MENU_MAIN,
    MENU_START,
    MENU_SCORES,
    MENU_QUIT,
    MENU_EXIT
} MenuState;

/**
 * @brief Opções do menu principal
 */
typedef enum {
    MENU_OPTION_START,
    MENU_OPTION_SCORES,
    MENU_OPTION_QUIT,
    MENU_OPTION_NONE
} MenuOption;


void (menu_init)();
void (menu_handle_input)();
void (menu_render)();
MenuState (menu_get_state)();



#endif
