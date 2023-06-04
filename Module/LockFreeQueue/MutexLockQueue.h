
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : MutexLockQueue.h
*   Last Modified : 2019-08-22 19:54
*   Describe      :
*
*******************************************************/

#ifndef  _MUTEXLOCKQUEUE_H
#define  _MUTEXLOCKQUEUE_H

#include <mutex>

template <typename T> 
class MutexLockQueue {
public:
    explicit MutexLockQueue(size_t capacity) {
        _capacityMask = capacity - 1;
        for(size_t i = 1; i <= sizeof(void*) * 4; i <<= 1) {
            _capacityMask |= _capacityMask >> i;
        }
        _capacity = _capacityMask + 1;

        _queue = (Node*)malloc(sizeof(Node) * _capacity);

        _head = 0;
        _tail = 0;
    }

    ~MutexLockQueue() {
        for(size_t i = _head; i != _tail; ++i) {
            (&_queue[i & _capacityMask].data)->~T();
        }
        free(_queue);
    }

    size_t capacity() const { return _capacity; }

    size_t size() const {
        size_t result;
        _mutex.lock();
        result = _tail - _head;
        _mutex.unlock();
        return result;
    }

    bool push(const T& data) {
        _mutex.lock();
        if(_tail - _head == _capacity) {
            _mutex.unlock();
            return false; // queue is full
        }
        Node& node = _queue[(_tail++) & _capacityMask];
        new (&node.data)T(data);
        _mutex.unlock();
        return true;
    }

    bool pop(T& result) {
        _mutex.lock();
        if(_head == _tail) {
            _mutex.unlock();
            return false; // queue is empty
        }
        Node& node = _queue[(_head++) & _capacityMask];
        result = node.data;
        (&node.data)->~T();
        _mutex.unlock();
        return true;
    }

private:
    struct Node {
        T data;
    };

private:
	Node* _queue;
    size_t _capacityMask;
	size_t _capacity;
	size_t _head;
	size_t _tail;
	mutable std::mutex _mutex;
};



#endif // _MUTEXLOCKQUEUE_H


