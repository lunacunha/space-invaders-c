
#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#define FREQUENCY 60
#define MODE 0x14c

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "src/controllers/keyboard/kb_interrupts.h"
#include "src/confs.h"
#include "ship/ship.h"
#include "enemies/enemies.h"

int (check_bullet_enemy_collision)();

int (game_state)(); 

#endif
