
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : SpinLockQueue.h
*   Last Modified : 2019-08-22 20:21
*   Describe      :
*
*******************************************************/

#ifndef  _SPINLOCKQUEUE_H
#define  _SPINLOCKQUEUE_H

#include <atomic>

template <typename T>
class SpinLockQueue {
public:
    explicit SpinLockQueue(size_t capacity) {
        _capacityMask = capacity - 1;
        for(size_t i = 1; i <= sizeof(void*) * 4; i <<= 1) {
            _capacityMask |= _capacityMask >> i;
        }
        _capacity = _capacityMask + 1;

        _queue = (Node*)malloc(sizeof(Node) * _capacity);

        _lock = 0;
        _head = 0;
        _tail = 0;
    }

    ~SpinLockQueue() {
        for(size_t i = _head; i != _tail; ++i) {
            (&_queue[i & _capacityMask].data)->~T();
        }
        free(_queue);
    }
  
    size_t capacity() const { return _capacity; }
  
    size_t size() const {
        size_t result;
        while(__sync_val_compare_and_swap(&_lock, 0, 1) != 0);
            result = _tail - _head;
        __sync_val_compare_and_swap(&_lock, 1, 0);
        return result;
    }
  
    bool push(const T& data) {
        while(__sync_val_compare_and_swap(&_lock, 0, 1) != 0);
        if(_tail - _head == _capacity) {
            __sync_val_compare_and_swap(&_lock, 1, 0);
            return false; // queue is full
        }
        Node& node = _queue[(_tail++) & _capacityMask];
        new (&node.data)T(data);
        __sync_val_compare_and_swap(&_lock, 1, 0);
        return true;
    }
  
    bool pop(T& result) {
        while(__sync_val_compare_and_swap(&_lock, 0, 1) != 0);
        if(_head == _tail) {
            _lock = 0;
            return false; // queue is empty
        }
        Node& node = _queue[(_head++) & _capacityMask];
        result = node.data;
        (&node.data)->~T();
        __sync_val_compare_and_swap(&_lock, 1, 0);
        return true;
    } 

private:
    struct Node {
        T data;
    };

private:
	size_t _capacityMask;
	Node* _queue;
 	size_t _capacity;
  	size_t _head;
  	size_t _tail;
  	mutable volatile int _lock;
};



#endif // _SPINLOCKQUEUE_H


