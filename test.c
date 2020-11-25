#include <stdio.h>
#include <stdint.h>
#include <string.h>

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


void mov16(uint8_t *dst, const uint8_t *src)
{
	uint16_t * __src_mem;
	uint16_t * __dst_mem;
	
	__src_mem = (uint16_t *)src;
	__dst_mem = (uint16_t *)dst;

	asm volatile (
		"lh %0, %1\n"
		: "=r" (*__dst_mem)
		: "m" (*__src_mem)
	);	
}

void mov32(uint8_t *dst, const uint8_t *src)
{
	uint32_t * __src_mem;
	uint32_t * __dst_mem;
	
	__src_mem = (uint32_t *)src;
	__dst_mem = (uint32_t *)dst;

	asm volatile (
		"lw %0, %1\n"
		: "=r" (*__dst_mem)
		: "m" (*__src_mem)
	);	
}


void mov64(uint8_t *dst, const uint8_t *src)
{
	uint64_t * __src_mem;
	uint64_t * __dst_mem;
	
	__src_mem = (uint64_t *)src;
	__dst_mem = (uint64_t *)dst;

	asm volatile (
		"ld %0, %1\n"
		: "=r" (*__dst_mem)
		: "m" (*__src_mem)
	);	
}

void mov128(uint8_t *dst, const uint8_t *src)
{
	mov64(dst + 0 * 64, src + 0 * 64);
	mov64(dst + 1 * 64, src + 1 * 64);
}


void rte_mov256(uint8_t *dst, const uint8_t *src)
{
	mov128(dst, src);
	mov128(dst + 128, src + 128);
}

int main()
{
#if 0
	uint64_t dst = 12111111111111111111123;
	uint64_t src = 321;

	mov64(&dst, &src);
	printf("dst %lu\n", dst);
	printf("src %lu\n", src);

	int cmp = memcmp(&src, &dst, 4);
	if (cmp < 0)
		printf("src < dst\n");
	else if (cmp > 0)
		printf("src > dst\n");
	else
		printf("src == dst\n");
#endif
	return 0;
}
