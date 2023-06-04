
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : SharedPtr.h
*   Last Modified : 2019-10-08 20:17
*   Describe      :
*
*******************************************************/

#ifndef  _SHAREDPTR_H
#define  _SHAREDPTR_H

#include <cstddef>
#include <mutex>

template <typename T>
class SharedPtr {
public:
    explicit SharedPtr(T* ptr = nullptr);
    ~SharedPtr();

    SharedPtr(const SharedPtr<T>&);
    SharedPtr<T>& operator=(const SharedPtr<T>&);

    T& operator*();
    T* operator->();

    size_t use_count();

private:
    size_t getCnt();
    void increase();
    void decrease();

private:
    T* _ptr = nullptr;
    size_t* _cntptr = nullptr;
    std::mutex _mtx;
};

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) {
    if(ptr) {
        _ptr = ptr;
        _cntptr = new size_t(0);
        increase();
    }
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    if(_ptr) {
        if(getCnt() > 0) {
            decrease();
        }
        else {
            delete _ptr;
            _ptr = nullptr;
            delete _cntptr;
            _cntptr = nullptr;
        }
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) {
    _ptr = other._ptr;
    _cntptr = other._cntptr;
    increase();
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
    if(this != &other) {
        if(_ptr != other._ptr) {
            decrease();
            if(getCnt() == 0) {
                this->~SharedPtr();
            }
        }
        _ptr = other._ptr;
        _cntptr = other._cntptr;
        increase();
    }
    return *this;
}

template <typename T>
T& SharedPtr<T>::operator*() {
    return *_ptr;
}

template <typename T>
T* SharedPtr<T>::operator->() {
    return _ptr;
}

template <typename T>
size_t SharedPtr<T>::use_count() {
    return getCnt();
}

template <typename T>
size_t SharedPtr<T>::getCnt() {
    return *_cntptr;
}

template <typename T>
void SharedPtr<T>::increase() {
    std::lock_guard<std::mutex> lg(_mtx);
    (*_cntptr)++;
}

template <typename T>
void SharedPtr<T>::decrease() {
    std::lock_guard<std::mutex> lg(_mtx);
    (*_cntptr)--;
}

#endif // _SHAREDPTR_H

