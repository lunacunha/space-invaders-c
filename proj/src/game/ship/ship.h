#ifndef SHIP_H
#define SHIP_H

#include <lcom/lcf.h>
#include "src/controllers/graphics/graphic.h"
#include "src/controllers/keyboard/kb_controller.h"
#include "src/xpm/game/player.h"
#include "src/xpm/messages.h"
#include "src/game/enemies/enemies.h"
#include "src/xpm/game/shields.h"
#include "src/game/scores/score.h"

/// Frequência de atualização do jogo (frames por segundo)
#define FREQUENCY 60

/// Modo gráfico utilizado
#define MODE 0x14c

/// Número máximo de balas disparadas pelo jogador
#define MAX_BULLETS 10

/// Número máximo de balas disparadas pelos inimigos
#define MAX_ENEMY_BULLETS 10

/// Largura da bala do jogador
#define BULLET_WIDTH 8

/// Altura da bala do jogador
#define BULLET_HEIGHT 30

/// Largura da bala do inimigo
#define ENEMY_BULLET_WIDTH 5

/// Altura da bala do inimigo
#define ENEMY_BULLET_HEIGHT 17

/// Número total de escudos no jogo
#define NUM_SHIELDS 4

/// Largura de cada escudo
#define SHIELD_WIDTH 47

/// Altura de cada escudo
#define SHIELD_HEIGHT 34

/**
 * @brief Representa uma bala disparada pelo jogador.
 */
typedef struct {
    int x;         /**< Posição horizontal da bala */
    int y;         /**< Posição vertical da bala */
    bool active;   /**< Indica se a bala está ativa */
} Bullet;

/**
 * @brief Representa uma bala disparada por um inimigo.
 */
typedef struct {
    int x;         /**< Posição horizontal da bala do inimigo */
    int y;         /**< Posição vertical da bala do inimigo */
    bool active;   /**< Indica se a bala está ativa */
} EnemyBullet;

/**
 * @brief Estados possíveis de um escudo.
 */
typedef enum {
    SHIELD_FULL,   /**< Escudo intacto */
    SHIELD_BROKEN, /**< Escudo danificado */
    SHIELD_NONE    /**< Escudo destruído */
} ShieldState;

/**
 * @brief Representa um escudo protetor no jogo.
 */
typedef struct {
    int x, y;             /**< Posição do escudo */
    ShieldState state;    /**< Estado atual do escudo */
} Shield;

/// Array de balas do jogador
Bullet bullets[MAX_BULLETS];

/// Array de balas dos inimigos
EnemyBullet enemy_bullets[MAX_ENEMY_BULLETS];

/// Array de escudos
Shield shields[NUM_SHIELDS];

/**
 * @brief Desenha a nave do jogador na posição especificada.
 * 
 * @param x Posição horizontal da nave.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (draw_ship)(uint16_t x);

/**
 * @brief Inicializa todas as balas do jogador como inativas.
 */
void (init_bullets)();

/**
 * @brief Inicializa todas as balas dos inimigos como inativas.
 */
void (init_enemy_bullets)();

/**
 * @brief Inicializa os escudos na posição inicial e estado completo.
 */
void (init_shields)();

/**
 * @brief Trata das ações do jogador, como movimentação e disparo.
 */
void (ship_action)();

/**
 * @brief Faz um inimigo disparar uma bala.
 */
void (fire_enemy_bullet)();

/**
 * @brief Desenha todos os escudos no ecrã.
 */
void (draw_shields)();

/**
 * @brief Desenha todas as balas do jogador ativas.
 * 
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (draw_all_bullets)();

/**
 * @brief Desenha uma bala específica do jogador.
 * 
 * @param bullet_obj Ponteiro para a bala a desenhar.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (draw_bullet)(Bullet* bullet_obj);

/**
 * @brief Desenha todas as balas disparadas pelos inimigos.
 */
void (draw_enemy_bullets)();

/**
 * @brief Atualiza a posição das balas do jogador.
 */
void (update_bullets)();

/**
 * @brief Atualiza a posição das balas dos inimigos.
 */
void (update_enemy_bullets)();

#endif
