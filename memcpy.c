#include <stdio.h>
#include <stdint.h>

void rte_mov16(uint8_t *dst, const uint8_t *src)
{
	uint16_t * __src_mem;
	uint16_t * __dst_mem;
	
	__src_mem = (uint16_t *)src;
	__dst_mem = (uint16_t *)dst;

	asm volatile (
		"lh %L0, %1\n"
		: "=r" (*__dst_mem)
		: "m" (*__src_mem)
	);	
}

void rte_move32(uint8_t *dst, uint8_t *src)
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

int main()
{
	return 0;
}
