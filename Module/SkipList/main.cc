
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : main.cc
*   Last Modified : 2019-10-12 15:48
*   Describe      :
*
*******************************************************/

#include <iostream>
#include "SkipList.h"

int main() {
    std::cout << "Test SkipList: " << std::endl;
    SkipList sl;

    for(int i = 0; i < 100; ++i) {
        sl.insert(i, i+10);
    }
    
    std::cout << "The number of elements in SkipList: " << sl.size() << std::endl;

    for(int i = 0; i < 100; ++i) {
        int value;
        if(sl.search(i, value) && value == i+10)
            continue;
        else {
            std::cout << "Search failed" << std::endl;
            exit(1);
        }
    }

    for(int i = 0; i < 100; ++i) {
        sl.remove(i);
        if(sl.size() == (size_t)(99 - i)) {
            printf("remove %d yes\n", i);
            continue;
        }
        else {
            std::cout << "Remove failed" << std::endl;
            exit(1);
        }
    }

    std::cout << "Test Finished ..." << std::endl;
    
    return 0;
}


