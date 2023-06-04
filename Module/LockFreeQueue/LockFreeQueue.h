
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : LockFreeQueue.h
*   Last Modified : 2019-08-23 13:15
*   Describe      :
*
*******************************************************/

#ifndef  _LOCKFREEQUEUE_H
#define  _LOCKFREEQUEUE_H

template <typename T>
class LockFreeQueue {
public:
  	explicit LockFreeQueue(size_t capacity) {
    	_capacityMask = capacity - 1;
    	for(size_t i = 1; i <= sizeof(void*) * 4; i <<= 1) {
      		_capacityMask |= _capacityMask >> i;
        }
        _capacity = _capacityMask + 1;

        _queue = (Node*)malloc(sizeof(Node) * _capacity);
        for(size_t i = 0; i < _capacity; ++i) {
            _queue[i].tail = i;
            _queue[i].head = -1;
        }

        _tail = 0;
        _head = 0;
    }

    ~LockFreeQueue() {
        for(size_t i = _head; i != _tail; ++i) {
            (&_queue[i & _capacityMask].data)->~T();
        }

        free(_queue);
    }
  
    size_t capacity() const { return _capacity; }
  
    size_t size() const {
        size_t head = __sync_fetch_and_add(&_head, 0);
        return _tail - head;
    }
  
    bool push(const T& data) {
        Node* node;
        size_t next, tail = _tail;
        for(;; tail = next) {
            node = &_queue[tail & _capacityMask];
            if(__sync_fetch_and_add(&(node->tail), 0) != tail)
                return false;
            if((next = __sync_val_compare_and_swap(&_tail, tail, tail + 1)) == tail)
                break;
        }
        new (&node->data)T(data);
        __sync_lock_test_and_set(&(node->head), tail);
        return true;
    }

    bool pop(T& result) {
        Node* node;
        size_t next, head = _head;
        for(;; head = next) {
            node = &_queue[head & _capacityMask];
            if(__sync_fetch_and_add(&(node->head), 0) != head)
                return false;
            if((next = __sync_val_compare_and_swap(&_head, head, head + 1)) == head)
                break;
        }
        result = node->data;
        (&node->data)->~T();
        __sync_lock_test_and_set(&(node->tail), head + _capacity);
        return true;
    }

private:
  	struct Node {
    	T data;
    	size_t tail;
    	size_t head;
  	};

private:
	size_t _capacityMask;
  	Node* _queue;
  	size_t _capacity;
  	volatile size_t _tail;
  	mutable volatile size_t _head;
};


#endif // _LOCKFREEQUEUE_H


