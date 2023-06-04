
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : main.cc
*   Last Modified : 2019-10-08 20:06
*   Describe      :
*
*******************************************************/

#include <iostream>
#include <string>
#include <cassert>

#include "SharedPtr.h"
#include "UniquePtr.h"

void testSharePtr() {
    auto a = new int(1);
    auto b = new int(314);

    SharedPtr<int> sp1(a);
    std::cout << "sp1:use_count:"  << sp1.use_count() << std::endl; // 1

    {
        SharedPtr<int> sp2(sp1);
        SharedPtr<int> sp3(sp1);

        std::cout << "sp1:use_count:"  << sp1.use_count() << std::endl; // 3
        std::cout << "sp2:use_count:"  << sp2.use_count() << std::endl; // 3
        std::cout << "sp3:use_count:"  << sp3.use_count() << std::endl; // 3
    }

    std::cout << "sp1:use_count:"  << sp1.use_count() << std::endl; // 1

    SharedPtr<int> sp4(b);
    std::cout << "sp4:use_count:"  << sp4.use_count() << std::endl; // 1

    SharedPtr<int> sp5(sp4);
    std::cout << "sp4:use_count:"  << sp4.use_count() << std::endl; // 2
    std::cout << "sp5:use_count:"  << sp5.use_count() << std::endl; // 2

    SharedPtr<int> sp6(sp5);
    std::cout << "sp4:use_count:"  << sp4.use_count() << std::endl; // 3
    std::cout << "sp5:use_count:"  << sp5.use_count() << std::endl; // 3
    std::cout << "sp6:use_count:"  << sp6.use_count() << std::endl; // 3

    sp6 = sp1;

    std::cout << "sp4:use_count:"  << sp4.use_count() << std::endl; // 2
    std::cout << "sp5:use_count:"  << sp5.use_count() << std::endl; // 2
    std::cout << "sp1:use_count:"  << sp1.use_count() << std::endl; // 2
    std::cout << "sp6:use_count:"  << sp6.use_count() << std::endl; // 2

    std::cout << "*sp1:"<< *sp1 << std::endl; // 1
    *sp1 = 10;
    std::cout << "*sp1:"<< *sp1 << std::endl; // 10
}

void testUniquePtr() {
    UniquePtr<std::string> p1(new std::string("ouweibin"));
    std::cout << *p1 << std::endl;

    {
        UniquePtr<int> up1;
        UniquePtr<int> up1b(nullptr);
        UniquePtr<int> up2(new int);

        UniquePtr<int> up3(new int);
        UniquePtr<int> up5(std::move(up2));

        UniquePtr<int> up6b(std::move(up3));
        UniquePtr<int> up7 = std::move(up5);

        int* fp = up7.release();
        assert(up7.get() == nullptr);
        delete fp;

        up6b.reset(new int());
        up6b.reset(nullptr);

        UniquePtr<float> up71(new float(1.1));
        UniquePtr<float> up72(new float(2.2));

        up71.swap(up72);

        std::cout << "up71->val:" << *up71 << std::endl;
        std::cout << "up72->val:" << *(up72.get()) << std::endl;
    }
}

int main() {
	testSharePtr();
    testUniquePtr();
	return 0;
}

