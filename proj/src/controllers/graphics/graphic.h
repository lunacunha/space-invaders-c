#ifndef GRAPHIC_H
#include <lcom/lcf.h>

#define VBE 0x10

int (set_graphical_mode)(uint16_t mode);
int (set_frame_buffer)(uint16_t mode);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (norm_color)(uint32_t color, uint32_t *n_color);

uint32_t (dir_mode)(unsigned row, unsigned col, uint8_t step, uint32_t first);

uint32_t (idx_mode)(unsigned row, unsigned col, uint8_t step, uint32_t first, uint8_t no_rectangles);

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);
#endif //GRAPHIC_H
