
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : main.cc
*   Last Modified : 2019-11-04 13:41
*   Describe      :
*
*******************************************************/

#include "Logger.h"
#include <unistd.h>

int main() {
    LOG_INIT("./test.log");
    LOG(INFO, "ouweibin");
    LOG(WARN, "123456");
    LOG(ERROR, "abcdefg");

    const char* s = "logger";
    LOG(DEBUG, "%s", s);

    LOG_LEVEL(DEBUG);
    LOG(DEBUG, "%s", s);

    LOG_UNINIT();
    return 0;
}


