
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : UniquePtr.h
*   Last Modified : 2019-10-08 18:52
*   Describe      :
*
*******************************************************/

#ifndef  _UNIQUEPTR_H
#define  _UNIQUEPTR_H

template <typename T>
class UniquePtr {
public:
    explicit UniquePtr(T* ptr = nullptr);
    ~UniquePtr();

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&&);
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr& operator=(UniquePtr&&);

    T* release();
    void reset();
    void reset(T*);
    void swap(UniquePtr&);

    T* get();
    T& operator*();
    T* operator->();

private:
    T* _ptr = nullptr;
};

template <typename T>
UniquePtr<T>::UniquePtr(T* ptr) {
    if(ptr) {
        _ptr = ptr;
    }
}
    
template <typename T>
UniquePtr<T>::~UniquePtr() {
    delete _ptr;
    _ptr = nullptr;
}

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) {
    _ptr = other._ptr;
    other._ptr = nullptr;
}

template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) {
    if(this != &other) {
        delete _ptr;
        _ptr = other._ptr;
        other._ptr = nullptr;
    }
    return *this;
}

template <typename T>
T* UniquePtr<T>::release() {
    T* tmp = _ptr;
    _ptr = nullptr;
    return tmp;
}

template <typename T>
void UniquePtr<T>::reset() {
    delete _ptr;
    _ptr = nullptr;
}

template <typename T>
void UniquePtr<T>::reset(T* ptr) {
    delete _ptr;
    if(ptr) {
        _ptr = ptr;
    }
    else {
        _ptr = nullptr;
    }
}

template <typename T>
void UniquePtr<T>::swap(UniquePtr& other) {
    std::swap(_ptr, other._ptr);
}

template <typename T>
T* UniquePtr<T>::get() {
    return _ptr;
}

template <typename T>
T& UniquePtr<T>::operator*() {
    return *_ptr;
}

template <typename T>
T* UniquePtr<T>::operator->() {
    return _ptr;
}

#endif // _UNIQUEPTR_H

