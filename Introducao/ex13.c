#include <stdio.h>
#include <stdlib.h>

int main(void) {

	int *array;

	int ln;
	scanf("%d", &ln);

	// array = (int*) malloc(ln * sizeof(int));
	array = (int*) calloc(ln, sizeof(int));

	for (int i = 0; i < ln; i++){
		array[i] = i;
		printf("array[%d] = %d\n", i, array[i]);
	}
	free (array);

	return 0;
}