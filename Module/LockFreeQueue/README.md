## LockFreeQueue

#### 原子操作

- 保证多个线程不在同一个时间内访问相同的资源
- 类似互斥量对共享资源访问的保护，但是原子操作更接近底层，效率更高

数值操作比如自加（++）、自减（--）、赋值（=）**都不是原子操作**， 如果使用锁或者互斥量保证操作的安全性，对于较高并发的程序会造成一定的性能瓶颈 



#### GCC提供的原子操作

GCC从4.1.2开始提供了`__sync_`系列的built-in函数，用于提供加减和逻辑运算的原子操作 

```c
// 第一组返回更新前的值，第二组返回更新后的值
type __sync_fetch_and_add (type *ptr, type value)
type __sync_fetch_and_sub (type *ptr, type value)
type __sync_fetch_and_or (type *ptr, type value)
type __sync_fetch_and_and (type *ptr, type value)
type __sync_fetch_and_xor (type *ptr, type value)
type __sync_fetch_and_nand (type *ptr, type value)

type __sync_add_and_fetch (type *ptr, type value)
type __sync_sub_and_fetch (type *ptr, type value)
type __sync_or_and_fetch (type *ptr, type value)
type __sync_and_and_fetch (type *ptr, type value)
type __sync_xor_and_fetch (type *ptr, type value)
type __sync_nand_and_fetch (type *ptr, type value)

// 原子比较和交换操作
// 如果*ptr == oldval，就将newval写入*ptr，第一个函数返回操作之前的值，第二个函数在相等并写入的情况下返回true
type __sync_val_compare_and_swap (type *ptr, type oldval, type newval)
bool __sync_bool_compare_and_swap (type *ptr, type oldval, type newval)

// 原子赋值操作，第一个函数将*ptr设为value并返回*ptr操作之前的值，第二个函数将*ptr置零
type __sync_lock_test_and_set (type *ptr, type value)
void __sync_lock_release (type *ptr)
```

```c++
// muduo Atomic.h
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include "muduo/base/noncopyable.h"

#include <stdint.h>

namespace muduo
{

namespace detail
{
template<typename T>
class AtomicIntegerT : noncopyable
{
 public:
  AtomicIntegerT()
    : value_(0)
  {
  }

  // uncomment if you need copying and assignment
  //
  // AtomicIntegerT(const AtomicIntegerT& that)
  //   : value_(that.get())
  // {}
  //
  // AtomicIntegerT& operator=(const AtomicIntegerT& that)
  // {
  //   getAndSet(that.get());
  //   return *this;
  // }

  T get()
  {
    // in gcc >= 4.7: __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
    return __sync_val_compare_and_swap(&value_, 0, 0);
  }

  T getAndAdd(T x)
  {
    // in gcc >= 4.7: __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST)
    return __sync_fetch_and_add(&value_, x);
  }

  T addAndGet(T x)
  {
    return getAndAdd(x) + x;
  }

  T incrementAndGet()
  {
    return addAndGet(1);
  }

  T decrementAndGet()
  {
    return addAndGet(-1);
  }

  void add(T x)
  {
    getAndAdd(x);
  }

  void increment()
  {
    incrementAndGet();
  }

  void decrement()
  {
    decrementAndGet();
  }

  T getAndSet(T newValue)
  {
    // in gcc >= 4.7: __atomic_exchange_n(&value, newValue, __ATOMIC_SEQ_CST)
    return __sync_lock_test_and_set(&value_, newValue);
  }

 private:
  volatile T value_;
};
}  // namespace detail

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

}  // namespace muduo

#endif  // MUDUO_BASE_ATOMIC_H
```



#### 自旋锁（spinlock）

当一个线程在获取锁时，如果锁已经被其它线程获取，那么该线程将循环等待，不断地判断该锁是否能够被成功获取，直到获取到锁时才会退出循环，尝试获取锁的线程一直处于活跃状态，但是并没有执行任何有效的任务，使用这种锁会造成**busy-waiting**

自旋锁的优点

自旋锁不会使线程状态发生切换，一直处于用户态，即线程一直都是活跃的不会使线程进入阻塞状态，减少了不必要的上下文切换，执行速度快 

非自旋锁在获取不到锁的时候会进入阻塞状态，从而进入内核态，当获取到锁的时候需要从内核态恢复，需要线程上下文切换（线程被阻塞后便进入内核（Linux）调度状态，这个会导致系统在用户态与内核态之间来回切换，严重影响锁的性能） 



#### 无锁队列的实现

**CAS操作： Compare & Swap** 

- GCC的CAS

  ```c
  bool __sync_bool_compare_and_swap (type *ptr, type oldval, type newval, ...)
  type __sync_val_compare_and_swap (type *ptr, type oldval, type newval, ...)
  ```

- C++11的CAS

  ```c++
  template< class T >
  bool atomic_compare_exchange_weak( std::atomic* obj,
                                     T* expected, T desired );
  template< class T >
  bool atomic_compare_exchange_weak( volatile std::atomic* obj,
                                     T* expected, T desired );
  ```

无锁队列的链表实现

无锁队列的数组实现



#### 参考

1. [GCC 提供的原子操作](https://www.cnblogs.com/FrankTan/archive/2010/12/11/1903377.html)

2. [【muduo】base篇---Atomic](https://blog.csdn.net/daaikuaichuan/article/details/86179766)

3. [无锁队列的实现](https://coolshell.cn/articles/8239.html)

4. [无锁队列是否不适用于大容量应用场景？](https://www.zhihu.com/question/25960605/answer/71689105)

5. [Cplusplus-Concurrency-In-Practice](https://github.com/forhappy/Cplusplus-Concurrency-In-Practice)

6. [C++11的原子量与内存序浅析](https://www.cnblogs.com/FateTHarlaown/p/8919235.html)

7. [C++11实现自旋锁](https://blog.csdn.net/jeffasd/article/details/80661804)

8. [自旋锁](http://blog.leanote.com/post/medusar/%E8%87%AA%E6%97%8B%E9%94%81)