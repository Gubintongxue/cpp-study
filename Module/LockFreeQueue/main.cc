
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : main.cc
*   Last Modified : 2019-10-14 16:37
*   Describe      :
*
*******************************************************/

#include <thread>
#include <vector>
#include <chrono>
#include <random>

#include <stdio.h>
#include <assert.h>

#include "LockFreeQueue.h"
#include "LockFreeQueueCpp11.h"
#include "MutexLockQueue.h"
#include "SpinLockQueue.h"
#include "mpmc_bounded_queue.h"

static const int testItems = 2500000;
static const int testThreadConsumerThreads = 8;
static const int testThreadProducerThreads = 8;
static const int testItemsPerConsumerThread = testItems / testThreadConsumerThreads;
static const int testItemsPerProducerThread = testItems / testThreadProducerThreads;

template<typename T>
class IQueue {
public:
    virtual size_t size() const = 0;
    virtual size_t capacity() const = 0;
    virtual bool push(const T& data) = 0;
    virtual bool pop(T& result) = 0;
};

template<typename T, class Q>
class TestQueue : public IQueue<T> {
public:
    TestQueue(size_t size) : queue(size) {}
    size_t size() const {return queue.size();}
    size_t capacity() const {return queue.capacity();}
    bool push(const T& data) {return queue.push(data);}
    bool pop(T& result) {return queue.pop(result);}

private:
    Q queue;
};

volatile size_t producerSum;
volatile size_t consumerSum;
volatile long maxPushDuration;
volatile long maxPopDuration;

void producerThread(IQueue<int>* queue)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    int val;
    for(int i = 0; i < testItemsPerProducerThread; ++i) {
        for(;;) {
            val = dis(gen);
            auto start = std::chrono::system_clock::now();
            while(!queue->push(val)) {
                std::this_thread::yield();
                start = std::chrono::system_clock::now();
            }
            {
                auto end = std::chrono::system_clock::now();
                long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                for(;;) {
                    long lmaxPushDuration = maxPushDuration;
                    if(duration <= lmaxPushDuration || __sync_val_compare_and_swap(&maxPushDuration, lmaxPushDuration, duration) == lmaxPushDuration)
                    break;
                }
                break;
            }
        }
        __sync_fetch_and_add(&producerSum, val);
    }
}

void consumerThread(IQueue<int>* queue)
{
    int val;
    for(int i = 0; i < testItemsPerConsumerThread; ++i) {
        for(;;) {
            auto start = std::chrono::system_clock::now();
            while(!queue->pop(val)) {
                std::this_thread::yield();
                start = std::chrono::system_clock::now();
            }
            {
                auto end = std::chrono::system_clock::now();
                long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                for(;;) {
                  long lmaxPopDuration = maxPopDuration;
                  if(duration <= lmaxPopDuration || __sync_val_compare_and_swap(&maxPopDuration, lmaxPopDuration, duration) == lmaxPopDuration)
                    break;
                }
                break;
            }
        }
        __sync_fetch_and_add(&consumerSum, val);
    }
}

template<class Q>
void testQueue(const std::string& name, bool fifo = false) {
	printf("Testing %s... \n", name.c_str());

	volatile int int32 = 0;
	volatile unsigned int uint32 = 0;
	assert(__sync_val_compare_and_swap(&int32, 0, 1) == 0);
	assert(int32 == 1);
	assert(__sync_val_compare_and_swap(&uint32, 0, 1) == 0);
	assert(uint32 == 1);

	volatile long int64 = 0;
	volatile unsigned long uint64 = 0;
	assert(__sync_val_compare_and_swap(&int64, 0, 1) == 0);
	assert(int64 == 1);
	assert(__sync_val_compare_and_swap(&uint64, 0, 1) == 0);
	assert(uint64 == 1);

    {
        TestQueue<int, Q> queue(10000);
        int result;
        assert(queue.capacity() == 16384);
        assert(!queue.pop(result));
        assert(queue.push(42));
        assert(queue.pop(result));
        assert(result == 42);
        assert(!queue.pop(result));
    }

    {
        TestQueue<int, Q> queue(2);
        int result;
        assert(queue.capacity() == 2);
        assert(!queue.pop(result));
        assert(queue.push(42));
        assert(queue.push(43));
        assert(queue.pop(result));
        assert(result == (fifo ? 43 : 42));
        assert(queue.pop(result));
        assert(result == (fifo ? 42 : 43));
        assert(!queue.pop(result));
        assert(queue.push(44));
        assert(queue.push(45));
        assert(queue.pop(result));
        assert(result == (fifo ? 45 : 44));
        assert(queue.push(47));
    }

    auto start = std::chrono::system_clock::now();
    {
        TestQueue<int, Q> queue(100);
        std::vector<std::thread> threads;
        for(int i = 0; i < testThreadProducerThreads; ++i) {
            threads.emplace_back(producerThread, &queue);
        }

        for(int i = 0; i < testThreadConsumerThreads; ++i) {
            threads.emplace_back(consumerThread, &queue);
        }

        for(auto iter = threads.begin(); iter != threads.end(); ++iter) {
            if(iter->joinable())
                iter->join();
        }

        assert(queue.size() == 0);
        assert(producerSum == consumerSum);
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    printf("%ld microseconds, maxPush: %ld microseconds, maxPop: %ld microseconds\n", 
        duration.count(), 
        maxPushDuration, 
        maxPopDuration);
}

int main(int argc, char* argv[]) {
	for(int i = 0; i < 3; ++i) {
        producerSum = 0;
        consumerSum = 0;
        maxPushDuration = 0;
        maxPopDuration = 0;

		printf("--- Run %d ---\n", i);
		testQueue<LockFreeQueue<int>>("LockFreeQueue");
		testQueue<mpmc_bounded_queue<int> >("mpmc_bounded_queue");
		testQueue<LockFreeQueueCpp11<int> >("LockFreeQueueCpp11");
		testQueue<MutexLockQueue<int>>("MutexLockQueue");
		testQueue<SpinLockQueue<int>>("SpinLockQueue");
	}
	return 0;
}


