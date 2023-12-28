#include <stdio.h>

void test(int *a_p) {
	printf("address of a = %p\n", a_p);
}

int main(void) {
	int a = 10;
	int *a_p = &a;
	printf("address of a = %p\n", a_p);

	test(a_p);

	return 0;
}
