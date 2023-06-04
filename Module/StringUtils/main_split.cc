
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : main_split.cc
*   Last Modified : 2019-10-09 10:41
*   Describe      :
*
*******************************************************/

#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> ret;
    int start = 0;
    for(size_t i = 0; i < str.length(); ++i) {
        if(str[i] != ' ')
            continue;
        else {
            if(i - start != 0) {     // 剔除首部空格和重复的空格
                ret.push_back(str.substr(start, i-start));
            }
            start = i+1;
        }
    }
    if(str.length() - start != 0) {  // 剔除尾部空格
        ret.push_back(str.substr(start, str.length() - start));
    }
    return ret;
}

int main() {
    std::vector<std::string> tokens = split("  ou  wei bin ");
    for(auto& s : tokens) {
        std::cout << s << std::endl;
    }
    return 0;
}


