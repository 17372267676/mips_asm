#include <stdio.h>


static inline void rte_prefetch0(const volatile void *p)
{
	asm volatile ("pref 1, 0 (%[rbase])\n" : : [rbase] "d" (p));
}


int main()
{
	int a = 1;
	int b = 2;
	rte_prefetch0(&b);

	printf("a :%d, b :%d\n", a, b);
}
