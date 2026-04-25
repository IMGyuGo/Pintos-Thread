#ifndef THREADS_SYNCH_H
#define THREADS_SYNCH_H

#include <list.h>
#include <stdbool.h>

/* A counting semaphore. */
struct semaphore
{
	unsigned value;		 /* Current value. */
	struct list waiters; /* List of waiting threads. */
};

void sema_init(struct semaphore *, unsigned value);
void sema_down(struct semaphore *);
bool sema_try_down(struct semaphore *);
void sema_up(struct semaphore *);
void sema_self_test(void);

/* Lock. */
struct lock
{
	struct thread *holder;		/* Thread holding lock (for debugging). */
	struct semaphore semaphore; /* Binary semaphore controlling access. */
};

void lock_init(struct lock *);
void lock_acquire(struct lock *);
bool lock_try_acquire(struct lock *);
void lock_release(struct lock *);
bool lock_held_by_current_thread(const struct lock *);

/* Condition variable. */
struct condition
{
	struct list waiters; /* List of waiting threads. */
};

void cond_init(struct condition *);
void cond_wait(struct condition *, struct lock *);
void cond_signal(struct condition *, struct lock *);
void cond_broadcast(struct condition *, struct lock *);

/* Optimization barrier.
 *
 * The compiler will not reorder operations across an
 * optimization barrier.  See "Optimization Barriers" in the
 * reference guide for more information.*/
/**
 * 함수는 호출 오버헤드가 존재 => 함수가 아니라 매크로
 * 함수는 인라인 보장 안됨
 * 함수는 컴파일러가 다시 최적화할 수도 있음
 *
 * asm : C 코드 안에 직접 어셈블리 코드 삽입
 * asm volatile ("") -> 아무 CPU 명령도 실행 안 함
 * 근데 volatile을 붙이면 컴파일러는 절대 지우지 않게 됨
 * : : : "memory" -> 이 코드가 메모리를 건드릴 수도 있으니 앞뒤 메모리 순서 절대 바꾸지 않도록 강제
 *
 * 만약 이게 없으면
 * intr_disable();
 * t = ticks;
 * intr_set_level(old_level);
 * 		↓
 * t - ticks;
 * intr_disable();
 * intr_set_level(old_level);
 * 로 컴파일러가 최적화해버려 내부적인 인터럽트 보호 역할이 완전 깨질 수 있음
 */
#define barrier() asm volatile("" : : : "memory")

#endif /* threads/synch.h */
