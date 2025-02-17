#include <stdlib.h>
#include <stdio.h>

#define BIT(n) (1 << (n)) // () prevent precedence issues

int main() {
    int n = 0;
    printf("n=%d\n", n);
    n = n | 1;
    printf("n|1=%d\n", n);

    printf("0b101^0b10=%d\n", 0b101^0b10);
    printf("0xF+1=%d\n", 0xF+1);

    int k = 0xE;
    printf("k=%d\n", k);
    printf("Bit 3 of k is %d\n", (k >> 3) & 1);
    printf("Bit 3 of k is %d\n", (k & 0b1000) >> 3);

    k |= (1 << 3);  // k = k | (1 << 3)
    printf("k=%d\n", k);

    k |= BIT(3); // set bit 3
    k &= ~BIT(3); // clear bit 3

    return 0;
}