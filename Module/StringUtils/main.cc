#include <iostream>
#include <cassert>

#include "StringUtils.h"

int main() {
    std::string testStr("Ou Wei Bin 欧炜滨123 ");
    std::vector<std::string> testStr1{"Ou", "Wei", "Bin", "欧炜滨123"};


    assert(owb::toUpper(testStr) == "OU WEI BIN 欧炜滨123 ");
    assert(owb::toLower(testStr) == "ou wei bin 欧炜滨123 ");

    assert(owb::RepeatN(testStr, 2) == "Ou Wei Bin 欧炜滨123 Ou Wei Bin 欧炜滨123 ");

    assert(owb::Compact(testStr1) == std::vector<std::string>({"Ou", "Wei", "Bin", "欧炜滨123"}));

    assert(owb::trimLeft(testStr) == "Ou Wei Bin 欧炜滨123 ");

    assert(owb::isStartWith(testStr, std::string("Ou")) == true);
    assert(owb::isEndWith(testStr, "123 ") == true);

    assert(owb::Join(testStr1, "+") == "Ou+Wei+Bin+欧炜滨123");
    assert(owb::SplitByStr(testStr, " ") == testStr1);
    assert(owb::Replace(testStr, " ", "+") == "Ou+Wei+Bin+欧炜滨123");

    assert(owb::SplitByC(testStr, " ") == testStr1);

    auto ret = owb::CovFrStr<bool>("true");
    std::cout << ret << std::endl;

    return 0;
}

