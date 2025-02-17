#include <stdlib.h>
#include <stdio.h>

void print_array(int *p, int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", p[i]);
    printf("\n");
}

// fill here
int main() {
    int n = 5;
    int *p = &n;
    printf("n=%d\n", n);    // %d - format to a decimal
    printf("*p=%d\n", *p);
    printf("p=%p\n",  (void *)p);    // %u - format to an unsigned int

    int **pp = &p;
    printf("p=%p\n", (void *)p);
    printf("*pp=%p\n", (void *)*pp);


    // Arrays
    int s[100] = { 0 }; // all zeroes by the compiler
    int *d = malloc(100 * sizeof(int)); // new int[100] in C++
    memset(d, 0, 100 * sizeof(int)); // all zeroes in run time

    s[1] = 1;
    printf("s[0]=%d\n", s[0]);
    printf("*s=%d\n", *s);
    printf("s[1]=%d\n", s[1]);
    printf("*(s+1)=%d\n", *(s+1));

    printf("*p=%d\n", *p);
    printf("p[0]=%d\n", p[0]);

    printf("sizeof(s)=%u\n", sizeof(s));
    printf("sizeof(d)=%u\n", sizeof(d));

    *p = 0;
    char *cp = (char *)p;
    cp[0] = 'A';
    printf("*cp=%c\n", *cp); // %c - format for a character
    printf("*p=%d\n", *p);

    print_array(s, 100);

    free(d); // del[] d in C++

    return 0;
}