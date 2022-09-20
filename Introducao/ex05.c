#include <stdio.h>

int main() {

	int x = 10, y = 20;

	for (int i = 0; i < x; i++)
		printf("y vale %d\n", y++);


	for(;;){
		printf("x vale %d\n", x++);
		if (x > 30)
			break;
	}
	
	return 0;
}
