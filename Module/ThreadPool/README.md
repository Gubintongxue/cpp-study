## ThreadPool

#### 虚假唤醒（线程池的惊群）

当一个线程调用notify_all后，多个调用wait的线程返回，这种效应称为**虚假唤醒（spurious wakeup）**

- 当消费者的数量为1时，没有其他线程竞争队列，不会触发虚假唤醒

- 当生产者的数量为1时，

  当使用notify_one通知消费线程时，不会发生虚假唤醒

  当使用notify_all通知消费线程时，会发生虚假唤醒，会有多个消费者线程收到信号被唤醒，当一个线程被唤醒之前，可能其他线程先被唤醒先持有锁，将产品消耗掉

- 当生产者的数量大于1时，都会发生虚假唤醒

```c++
void wait(unique_lock<mutex>& lck);
template <class Predicate>
  void wait(unique_lock<mutex>& lck, Predicate pred);
```

- 当前线程调用wait函数后阻塞并且会解锁互斥量，**如果使用lock_guard则不能调用unlock函数，所以只能使用unique_lock**，直到另外某个线程调用notify_one或者notify_all唤醒当前线程，一旦当前线程获得通知，wait函数将自动调用lock函数，同理不能使用lock_guard对象

- 如果wait没有第二个参数，第一次调用默认条件不成立，直接解锁互斥量并阻塞，直到另外某个线程调用notify_one或notify_all为止，被唤醒后wait重新尝试获取互斥量，如果得不到，线程卡住直到获取到互斥量，然后无条件继续执行后面的操作

- 如果wait包含第二个参数，如果第二个参数不满足，那么wait将解锁互斥量并堵塞，直到另外某个线程调用notify_one或notify_all为止，被唤醒后wait重新尝试获取互斥量，如果得不到，线程卡住直到获取到互斥量，然后继续判断第二个参数，如果表达式为false，wait对互斥量解锁，然后阻塞，如果为true，则执行后面的操作，**因此带第二个参数的wait可以解决虚假唤醒**



**CountDownLatch**通过一个计数器来实现，在new一个CountDownLatch对象的时候需要传入一个计数器值，该值表示了线程的数量，每当一个线程完成自己的任务后，计数器的值就会减1。当计数器的值变为0时，就表示所有的线程均已经完成了任务，然后就可以恢复等待的线程继续执行了

```c++
CountDownLatch latch(1);  // 初始化计数
tp.run(std::bind(&CountDownLatch::countDown, &latch));  // 计数减1
latch.wait();             // 等待直到计数为0
```

