#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <lcom/lcf.h>

/// Interrupção da BIOS para operações VBE (modo gráfico)
#define VBE 0x10

/**
 * @brief Define o modo gráfico de vídeo.
 * 
 * @param mode Modo gráfico a ser definido (ex: 0x14C).
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (set_graphical_mode)(uint16_t mode);

/**
 * @brief Inicializa o frame buffer para desenho gráfico.
 * 
 * @param mode Modo gráfico utilizado.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (set_frame_buffer)(uint16_t mode);

/**
 * @brief Limpa o buffer secundário (back buffer) com uma cor específica.
 * 
 * @param color Cor usada para limpar.
 */
void (clear_back_buf)(uint32_t color);

/**
 * @brief Troca o buffer secundário com o principal (double buffering).
 */
void (swap_buffers)();

/**
 * @brief Limpa uma área retangular do ecrã com uma cor.
 * 
 * @param x Coordenada X inicial.
 * @param y Coordenada Y inicial.
 * @param width Largura da área.
 * @param height Altura da área.
 * @param color Cor usada para preencher.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (clear_area)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Desenha um píxel no ecrã.
 * 
 * @param x Coordenada X do píxel.
 * @param y Coordenada Y do píxel.
 * @param color Cor do píxel.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Desenha uma linha horizontal.
 * 
 * @param x Coordenada X inicial.
 * @param y Coordenada Y.
 * @param len Comprimento da linha.
 * @param color Cor da linha.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Desenha um retângulo.
 * 
 * @param x Coordenada X inicial.
 * @param y Coordenada Y inicial.
 * @param width Largura do retângulo.
 * @param height Altura do retângulo.
 * @param color Cor do retângulo.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Normaliza uma cor para o formato utilizado pelo modo gráfico.
 * 
 * @param color Cor original.
 * @param n_color Ponteiro onde será armazenada a cor normalizada.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (norm_color)(uint32_t color, uint32_t *n_color);

/**
 * @brief Calcula uma cor para o modo direto com base na posição.
 * 
 * @param row Linha da grelha.
 * @param col Coluna da grelha.
 * @param step Incremento de cor por posição.
 * @param first Cor base.
 * @return Cor resultante.
 */
uint32_t (dir_mode)(unsigned row, unsigned col, uint8_t step, uint32_t first);

/**
 * @brief Calcula uma cor para o modo indexado com base na posição.
 * 
 * @param row Linha da grelha.
 * @param col Coluna da grelha.
 * @param step Incremento de cor por posição.
 * @param first Cor base.
 * @param no_rectangles Número total de retângulos.
 * @return Cor resultante.
 */
uint32_t (idx_mode)(unsigned row, unsigned col, uint8_t step, uint32_t first, uint8_t no_rectangles);

/**
 * @brief Desenha uma imagem no formato XPM no ecrã.
 * 
 * @param xpm Imagem no formato XPM.
 * @param x Coordenada X onde a imagem será desenhada.
 * @param y Coordenada Y onde a imagem será desenhada.
 * @return 0 em caso de sucesso, valor diferente de 0 em caso de erro.
 */
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif // GRAPHIC_H
