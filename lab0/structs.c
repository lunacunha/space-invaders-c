#include <string.h>
#include <stdio.h>

typedef struct {
    int w, h; // width, height
} Plane;

typedef struct {
    Plane base;
    int d; // depth
} Volume;

void print_plane(Plane *p) {
    printf("W %d, H %d\n", p->w, p->h);
}

void print_volume(Volume *v) {
    printf("W %d, H %d, D %d\n", v->base.w, v->base.h, v->d);
}

int main() {
    Plane p = {5, 2};
    Volume v = {p, 3};
    print_plane(&p);
    print_plane((Plane *) &v);
    print_volume(&v);
    return 0;
}