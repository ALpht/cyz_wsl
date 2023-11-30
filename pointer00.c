#include<stdio.h>
#include<stdlib.h>

int number(int a, int b)
{
	int result;
	result = a + b;
	return result;
}

int main()
{
	int *ptr, x, i;
	x = 100;
	ptr = &x;
	printf("ptr = %d\n", *ptr);
	printf("&ptr = %p\n", ptr);
	for(i = 1; i <= 10; i++)
	{
		x *= i;
		printf("x = %d\n", x);
	}
	
	printf("result : %d\n", number(x, i));

	return 0;
}
