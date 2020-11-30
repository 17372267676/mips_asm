#include <stdio.h>


typedef struct {
	volatile int locked; /**< lock status 0 = unlocked, 1 = locked */
} rte_spinlock_t;

static inline void
rte_spinlock_lock(rte_spinlock_t *sl)
{

	int lock_val;
	asm volatile (
		"1:\n"
		"ll %[lock_val], %[locked]\n"
		"bnez %[lock_val], 1b\n" /*如果lock_val不为0，表示锁已经被其他core占用，跳转到1*/
		"addiu %[lock_val], %[lock_val], 1\n" /*此时lock_val为0，再加1，变为1*/
		"	sc	%[lock_val], %[locked]		\n"
		"beqz %[lock_val], 1b\n"
		"sync\n"
		//: [locked] "=m" (sl->locked), [lv] "=q" (lock_val),
		: [locked] "=m" (sl->locked),
		  [lock_val] "=&r" (lock_val)
		:
		: "memory");
}

rte_spinlock_unlock(rte_spinlock_t *sl)
{

	int *locked = &(sl->locked);
	asm volatile (
		"sync\n"
		"sw $0, %[locked]\n"
		: [locked] "=m" (*locked)
		:
		: "memory");
}

rte_spinlock_t lock;


int main()
{
#if 0
rte_spinlock_t lock1 = {0};
//lock1.locked = 1;

	rte_spinlock_lock(&lock1);
	printf("the value of lock1 %d\n ", lock1.locked);
rte_spinlock_unlock(&lock1);
	printf("the value of lock1 %d\n ", lock1.locked);

	rte_spinlock_lock(&lock1);
	printf("the value of lock1 %d\n ", lock1.locked);
rte_spinlock_unlock(&lock1);
	printf("the value of lock1 %d\n ", lock1.locked);
#else
	printf("the value of lock %d\n ", lock.locked);
	rte_spinlock_lock(&lock);
	printf("lock too\n");
	rte_spinlock_unlock(&lock);
	rte_spinlock_lock(&lock);
	printf("the value of lock %d\n ", lock.locked);
	rte_spinlock_unlock(&lock);
	printf("the value of lock %d\n ", lock.locked);
	rte_spinlock_lock(&lock);
	printf("the value of lock %d\n ", lock.locked);
	rte_spinlock_unlock(&lock);
	printf("the value of lock %d\n ", lock.locked);
	rte_spinlock_lock(&lock);
#endif
	return 0;	
}
