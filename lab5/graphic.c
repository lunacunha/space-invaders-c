#include <lcom/lcf.h>
#include "VBE.h"

int (set_graphical_mode)(uint16_t mode) {
    reg86_t r86;
    memset(&r86, 0, sizeof(r86)); /* zero the structure */
    r86.ax = 0x4F02; /* r86.ah = 0x4F, r86.al = 0x02 */
    r86.bx = BIT(14) | mode; /* set the linear frame buffer model */
    r86.intno = VBE; /* VBE = 0x10 */
    if(sys_int86(&r86) != 0 ) {
        printf("\n Set graaphic mode failed \n");
        return 1;
    }
    return 0;
}

vbe_mode_info_t mode_info;

int set_frame_buffer(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));
    vbe_get_mode_info(mode, &mode_info);
    return 0;
}

