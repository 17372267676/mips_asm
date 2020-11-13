#include <stdio.h>

#if 0
void fun() {
	int a = 10, b;
	asm ( "movl %1, %%eax\n"
	      "movl %%eax, %0\n"
		:"=r"(b)
		:"r"(a)
		:"%eax"
	);
	printf("b is %d\n", b);

}
#endif


void move16(unsigned char *dst, unsigned char *src)
{
	asm volatile (
		"ld %0, %1\n"
		: "=r" (*dst)
		: "m" (*src)
	);	
}

int main()
{
	unsigned short int aa = 0;
	unsigned short int bb = 123;

	move16(&aa, &bb);
	printf("%u\n", aa);
	return 0;
}
