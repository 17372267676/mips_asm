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

static inline void *
rte_memcpy_func(void *dst, const void *src, size_t n)
{
	void *ret = dst;

	/* We can't copy < 16 bytes using XMM registers so do it manually. */
	if (n < 16) {
		if (n & 0x01) {
			*(uint8_t *)dst = *(const uint8_t *)src;
			dst = (uint8_t *)dst + 1;
			src = (const uint8_t *)src + 1;
		}
		if (n & 0x02) {
			*(uint16_t *)dst = *(const uint16_t *)src;
			dst = (uint16_t *)dst + 1;
			src = (const uint16_t *)src + 1;
		}
		if (n & 0x04) {
			*(uint32_t *)dst = *(const uint32_t *)src;
			dst = (uint32_t *)dst + 1;
			src = (const uint32_t *)src + 1;
		}
		if (n & 0x08)
			*(uint64_t *)dst = *(const uint64_t *)src;
		return ret;
	}

	/* Special fast cases for <= 128 bytes */
	if (n <= 32) {
		rte_mov16((uint8_t *)dst, (const uint8_t *)src);
		rte_mov16((uint8_t *)dst - 16 + n,
			(const uint8_t *)src - 16 + n);
		return ret;
	}

	if (n <= 64) {
		rte_mov32((uint8_t *)dst, (const uint8_t *)src);
		rte_mov32((uint8_t *)dst - 32 + n,
			(const uint8_t *)src - 32 + n);
		return ret;
	}

	if (n <= 128) {
		rte_mov64((uint8_t *)dst, (const uint8_t *)src);
		rte_mov64((uint8_t *)dst - 64 + n,
			(const uint8_t *)src - 64 + n);
		return ret;
	}

	/*
	 * For large copies > 128 bytes. This combination of 256, 64 and 16 byte
	 * copies was found to be faster than doing 128 and 32 byte copies as
	 * well.
	 */
	for ( ; n >= 256; n -= 256) {
		rte_mov256((uint8_t *)dst, (const uint8_t *)src);
		dst = (uint8_t *)dst + 256;
		src = (const uint8_t *)src + 256;
	}

	/*
	 * We split the remaining bytes (which will be less than 256) into
	 * 64byte (2^6) chunks.
	 * Using incrementing integers in the case labels of a switch statement
	 * encourages the compiler to use a jump table. To get incrementing
	 * integers, we shift the 2 relevant bits to the LSB position to first
	 * get decrementing integers, and then subtract.
	 */
	switch (3 - (n >> 6)) {
	case 0x00:
		rte_mov64((uint8_t *)dst, (const uint8_t *)src);
		n -= 64;
		dst = (uint8_t *)dst + 64;
		src = (const uint8_t *)src + 64;      /* fallthrough */
	case 0x01:
		rte_mov64((uint8_t *)dst, (const uint8_t *)src);
		n -= 64;
		dst = (uint8_t *)dst + 64;
		src = (const uint8_t *)src + 64;      /* fallthrough */
	case 0x02:
		rte_mov64((uint8_t *)dst, (const uint8_t *)src);
		n -= 64;
		dst = (uint8_t *)dst + 64;
		src = (const uint8_t *)src + 64;      /* fallthrough */
	default:
		;
	}

	/*
	 * We split the remaining bytes (which will be less than 64) into
	 * 16byte (2^4) chunks, using the same switch structure as above.
	 */
	switch (3 - (n >> 4)) {
	case 0x00:
		rte_mov16((uint8_t *)dst, (const uint8_t *)src);
		n -= 16;
		dst = (uint8_t *)dst + 16;
		src = (const uint8_t *)src + 16;      /* fallthrough */
	case 0x01:
		rte_mov16((uint8_t *)dst, (const uint8_t *)src);
		n -= 16;
		dst = (uint8_t *)dst + 16;
		src = (const uint8_t *)src + 16;      /* fallthrough */
	case 0x02:
		rte_mov16((uint8_t *)dst, (const uint8_t *)src);
		n -= 16;
		dst = (uint8_t *)dst + 16;
		src = (const uint8_t *)src + 16;      /* fallthrough */
	default:
		;
	}

	/* Copy any remaining bytes, without going beyond end of buffers */
	if (n != 0)
		rte_mov16((uint8_t *)dst - 16 + n,
			(const uint8_t *)src - 16 + n);
	return ret;
}

int main()
{
	return 0;
}
