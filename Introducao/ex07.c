#include <stdio.h>
#include "ex07.h"

int main(void){
    int a = 6;
    printf("a = %d\n", plus_one(a));

    return 0;
}

int plus_one(int x) {
    return x + 1;
}