#include <lcom/lcf.h>
#include "graphic.h"

int (set_graphical_mode)(uint16_t mode) {
    reg86_t r86;
    memset(&r86, 0, sizeof(r86)); /* zero the structure */
    r86.ax = 0x4F02; /* r86.ah = 0x4F, r86.al = 0x02 */
    r86.bx = BIT(14) | mode; /* set the linear frame buffer model */
    r86.intno = VBE; /* VBE = 0x10 */
    if(sys_int86(&r86) != 0) {
        printf("\n Set graaphic mode failed \n");
        return 1;
    }
    return 0;
}

uint8_t *video_mem; /* frame-buffer VM address (static global variable) */ 
vbe_mode_info_t mode_info;
unsigned int vram_size;

int set_frame_buffer(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));
    if(vbe_get_mode_info(mode, &mode_info) != 0) return 1;

    /* bytes per pixel */
    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    vram_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

    /* Allow memory mapping */
    struct minix_mem_range mr;
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)!=0){
        return 1;
    }

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
        return 1;
    }
    return 0;
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    // limits
    if (x >= mode_info.XResolution || y >= mode_info.YResolution || x < 0 || y < 0) return 1;
    
    // pixel index
    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel+7)/8;
    unsigned int idx = (mode_info.XResolution * y + x) * bytes_per_pixel;
    if (memcpy(&video_mem[idx], &color, bytes_per_pixel) == NULL) return 1;
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (unsigned i = 0; i < len; i++) {
        if (vg_draw_pixel(x+i, y, color) != 0) return 1;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (unsigned i = 0; i < height; i++) {
        if (vg_draw_hline(x, y+i, width, color) != 0) return 1;
    }
    return 0;
}

int norm_color(uint32_t color, uint32_t *n_color) {
    if (mode_info.BitsPerPixel == 32) {*n_color = color;}
    else {*n_color = color & (BIT(mode_info.BitsPerPixel) - 1);}
    return 0;
}

// direct mode 
uint32_t (dir_mode) (unsigned col, unsigned row, uint8_t step, uint32_t first) {
    uint32_t r = ((((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition)) + col * step) % (1 << mode_info.RedMaskSize);
    uint32_t g = ((((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition)) + row * step) % (1 << mode_info.GreenMaskSize);
    uint32_t b = ((((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition)) + (col + row) * step) % (1 << mode_info.BlueMaskSize);
    return (r << mode_info.RedFieldPosition) | (g << mode_info.GreenFieldPosition) | (b << mode_info.BlueFieldPosition);
}

// indexed mode
uint32_t (idx_mode) (unsigned col, unsigned row, uint8_t step, uint32_t first, uint8_t no_rectangles) {
    return ((first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel));
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t image;
    uint32_t *colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);
    for (int h = 0; h < image.height; h++) {
        for (int w = 0; w < image.width; w++) {
            if (vg_draw_pixel(w+x, h+y, colors[h*image.width + w]) != 0) return 1;
        }
    }
    return 0;
}





