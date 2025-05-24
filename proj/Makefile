# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/src/controllers/timer
.PATH: ${.CURDIR}/src/controllers/keyboard
.PATH: ${.CURDIR}/src/controllers/graphics
.PATH: ${.CURDIR}/src/game/ship


# source code files to be compiled
SRCS = utils.c timer.c kb_controller.c kb_interrupts.c graphic.c main.c ship.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
