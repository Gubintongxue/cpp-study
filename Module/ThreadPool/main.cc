
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : main.cc
*   Last Modified : 2019-10-11 13:01
*   Describe      :
*
*******************************************************/

#include <iostream>
#include <unistd.h>
#include "ThreadPool.h"

int main() {
    ThreadPool tp(4);
    for(int i = 0; i < 20; ++i) {
        tp.addTask([=] {
            printf("%d\n", i);
        });
    }
    sleep(1);
}

