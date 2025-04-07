#include <lcom/lcf.h>
#include "VBE.h"

int (set_graphic_mode)(uint16_t mode) {
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));
    r86.ax = 0x4F02;
    r86.bx = BIT(14) | mode;
    r86.intno = VBE;
    if(sys_int86(&r86) != 0 ) {
        printf("\n Set graaphic mode failed \n");
        return 1;
    }
}
