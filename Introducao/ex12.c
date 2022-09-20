#include <stdio.h>
#include <string.h>

int main(void) {
    char *s = "TESTE";

    char buffer[20];
    printf("Type smt: ");
    scanf("%s", buffer);
    printf("You typed: %s with len: %d\n", buffer, strlen(buffer));

    return 0;
}