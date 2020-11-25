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

void rte_mov32(uint8_t *dst, uint8_t *src)
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

void rte_mov64(uint8_t *dst, const uint8_t *src)
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
	rte_mov64(dst + 0 * 64, src + 0 * 64);
	rte_mov64(dst + 1 * 64, src + 1 * 64);
}


void rte_mov256(uint8_t *dst, const uint8_t *src)
{
	rte_mov128(dst, src);
	rte_mov128(dst + 128, src + 128);
}
int main()
{
	return 0;
}
