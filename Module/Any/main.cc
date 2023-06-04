
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : main.cc
*   Last Modified : 2019-11-16 13:02
*   Describe      :
*
*******************************************************/

#include "Any.h"

#include <iostream>
#include <string>

using namespace ideal;

int main() {
    Any a = 31415;
    Any b = std::string("hello world");
    Any c;

    std::cout << std::boolalpha;
    std::cout << a.empty() << std::endl;     // false
    std::cout << b.empty() << std::endl;     // false
    std::cout << c.empty() << std::endl;;    // true

    std::cout << a.isType<int>() << std::endl;;            // true
    std::cout << a.isType<std::string>() << std::endl;     // false
    std::cout << a.any_cast<int>() << std::endl;           // 31415
    
    c = b;
    std::cout << c.isType<int>() << std::endl;;                // false
    std::cout << c.isType<std::string>() << std::endl;;        // true
    std::cout << c.isType<const std::string&>() << std::endl;  // true
    std::cout << c.isType<std::string&&>() << std::endl;       // true
    std::cout << c.any_cast<std::string>() << std::endl;       // "hello world"
//    std::cout << c.any_cast<int>() << std::endl;
    
    return 0;
}

