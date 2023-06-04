
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : Any.h
*   Last Modified : 2019-11-16 11:17
*   Describe      :
*
*******************************************************/

#ifndef  _ANY_H
#define  _ANY_H

#include <memory>
#include <typeinfo>
#include <typeindex>

namespace ideal {

class Any {
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;

public:
    Any() : _basePtr(nullptr), _typeIndex(typeid(void)) { }
    Any(const Any& other) : _basePtr(other.clone()), _typeIndex(other._typeIndex) { }
    Any& operator=(const Any& other) {
        if(this != &other) {
            _basePtr = other.clone();
            _typeIndex = other._typeIndex;
        }
        return *this;
    }

    Any(Any&& other) : _basePtr(std::move(other._basePtr)), _typeIndex(std::move(other._typeIndex)) { }
    template <typename T>
    Any(T&& value) : _basePtr(new Derived<typename std::decay<T>::type>(std::forward<T>(value))),
                     _typeIndex(std::type_index(typeid(typename std::decay<T>::type))) { }

    bool empty() const { return _basePtr == nullptr; }
    
    template <class T>
    bool isType() const { return _typeIndex == std::type_index(typeid(T)); }

    template <class T>
    T& any_cast() const {
        if(!isType<T>()) {
            throw std::bad_cast(); // 抛出异常
        }
        auto derived = dynamic_cast<Derived<T>*>(_basePtr.get());
        return derived->_value;
    }

private:
    BasePtr clone() const {
        if(_basePtr != nullptr)
            return _basePtr->clone();
        return nullptr;
    }

private:
    struct Base {
        virtual ~Base() { }
        virtual BasePtr clone() const = 0;
    };

    template <typename T>
    struct Derived : public Base {
        template <typename U>
        Derived(U&& value) : _value(std::forward<U>(value)) { }

        BasePtr clone() const {
            return BasePtr(new Derived<T>(_value));
        }

        T _value;
    };

    BasePtr _basePtr;
    std::type_index _typeIndex;
};

}

#endif // _ANY_H


