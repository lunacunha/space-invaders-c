#ifndef ENEMIES_H
#define ENEMIES_H

#include <lcom/lcf.h>
#include "src/controllers/graphics/graphic.h"
#include "src/xpm/game/aliens.h"
#include "src/xpm/messages.h"

/// Frequência de atualização do jogo (frames por segundo)
#define FREQUENCY 60

/// Modo gráfico utilizado
#define MODE 0x14c

/// Número máximo de inimigos
#define MAX_ENEMIES 18

/// Número de linhas de inimigos
#define ENEMY_ROWS 3

/// Número de colunas de inimigos
#define ENEMY_COLS 6

/// Largura de cada inimigo (em píxeis)
#define ENEMY_WIDTH 40

/// Altura de cada inimigo (em píxeis)
#define ENEMY_HEIGHT 40

/// Espaço horizontal entre inimigos
#define ENEMY_SPACING_X 100

/// Espaço vertical entre inimigos
#define ENEMY_SPACING_Y 80

/// Posição inicial no eixo X para os inimigos
#define ENEMY_START_X 100

/// Posição inicial no eixo Y para os inimigos
#define ENEMY_START_Y 50

/**
 * @brief Estrutura que representa um inimigo no ecrã.
 */
typedef struct {
    int x;          /**< Posição horizontal do inimigo */
    int y;          /**< Posição vertical do inimigo */
    bool active;    /**< Indica se o inimigo está ativo */
    int type;       /**< Tipo de inimigo (0, 1 ou 2, dependendo da linha) */
} Enemy;

/// Array que contém todos os inimigos do jogo
Enemy enemies[MAX_ENEMIES];

/**
 * @brief Inicializa os inimigos com posições e tipos predefinidos.
 * 
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (init_enemies)();

/**
 * @brief Desenha um inimigo no ecrã.
 * 
 * @param enemy Ponteiro para a estrutura do inimigo a desenhar.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (draw_enemy)(Enemy* enemy);

/**
 * @brief Desenha todos os inimigos ativos.
 * 
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (draw_all_enemies)();

/**
 * @brief Limpa (apaga) um inimigo do ecrã.
 * 
 * @param enemy Ponteiro para a estrutura do inimigo a apagar.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (clear_enemy)(Enemy* enemy);

/**
 * @brief Limpa (apaga) todos os inimigos do ecrã.
 * 
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (clear_all_enemies)();

/**
 * @brief Move todos os inimigos em formação no ecrã.
 * 
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (move_enemies)();

/**
 * @brief Conta o número de inimigos ainda ativos.
 * 
 * @return Número de inimigos ativos.
 */
int (count_active_enemies)();

#endif
