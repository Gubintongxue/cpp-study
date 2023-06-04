
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : MyString.h
*   Last Modified : 2019-10-08 13:33
*   Describe      :
*
*******************************************************/

#ifndef  _MYSTRING_H
#define  _MYSTRING_H

#include <iostream>
#include <cstring>

class MyString {
public:
    explicit MyString(const char* str = nullptr);
    MyString(const MyString& other);
    MyString(MyString&& other);
    ~MyString();

    MyString& operator=(MyString& other);
    MyString& operator=(MyString&& other);

private:
    char* _data;
};

MyString::MyString(const char* str) {
    if(str == nullptr) {
        std::cout << "Default Constructor" << std::endl;
        _data = new char[1];
        _data[0] = '\0';
    }
    else {
        std::cout << "Pass Argument Constructor" << std::endl;
        int len = strlen(str);
        _data = new char[len+1];
        strcpy(_data, str);
    }

}

MyString::MyString(const MyString& other) {
    std::cout << "Copy Constructor" << std::endl;
    int len = strlen(other._data);
    _data = new char[len+1];
    strcpy(_data, other._data);
}

MyString::MyString(MyString&& other) {
    std::cout << "Move Constructor" << std::endl;
    _data = other._data;
    other._data = nullptr;
}

MyString::~MyString() {
    std::cout << "Destructor" << std::endl;
    delete[] _data;
    _data = nullptr;
}

MyString& MyString::operator=(MyString& other) {
    std::cout << "Copy assignment" << std::endl;
    if(this != &other) {
        delete[] _data;
        int len = strlen(other._data);
        _data = new char[len+1];
        strcpy(_data, other._data);
    }
    return *this;
}

MyString& MyString::operator=(MyString&& other) {
    std::cout << "Move assignment" << std::endl;
    if(this != &other) {
        delete[] _data;
        _data = other._data;
        other._data = nullptr;
    }
    return *this;
}

#endif // _MYSTRING_H

