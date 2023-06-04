#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <string>
#include <algorithm>
#include <sstream>

namespace owb {

    // 字符串str转大写
    std::string toUpper(const std::string &str) {
        std::string tmp(str);
        transform(str.begin(), str.end(), tmp.begin(), ::toupper);
        return tmp;
    }

    // 字符串str转小写
    std::string toLower(const std::string &str) {
        std::string tmp(str);
        transform(str.begin(), str.end(), tmp.begin(), ::tolower);
        return tmp;
    }

    // 字符串str拷贝N次
    std::string RepeatN(const std::string &str, unsigned int N) {
        std::string tmp;
        for(unsigned int i=0; i<N; ++i) {
            tmp += str;
        }
        return tmp;
    }

    // 去除字符串数组tokens里的空字符串
    std::vector<std::string> Compact(const std::vector<std::string> &tokens) {
        std::vector<std::string> tmp;
        for(auto& s:tokens) {
            if(!s.empty()) {
                tmp.push_back(s);
            }
        }
        return tmp;
    }

    // 去除字符串str首部的空白字符
    std::string trimLeft(const std::string &str) {
        std::string blank = " \t\n\v\f\r"; // 空白字符
        std::string tmp(str);
        auto iter = tmp.begin();
        for(; iter!=tmp.end(); iter++) {
            if(blank.find(*iter) == std::string::npos)
                break;
        }
        if(iter==tmp.end())
            tmp.clear();
        else
            tmp.erase(tmp.begin(), iter);

        return tmp;
    }

    // 去除字符串尾部的空白字符
    std::string trimRight(const std::string &str) {
        std::string blank = " \t\n\v\f\r";
        std::string tmp(str);
        auto iter = tmp.end()-1;
        for(; ;iter--) {
            if(blank.find(*iter) == std::string::npos) {
                tmp.erase(iter+1, tmp.end());
                break;
            }
            if(iter==tmp.begin()) {
                tmp.clear();
                break;
            }
        }

        return tmp;
    }

    // 去除字符串str首部尾部的空白字符
    std::string trim(const std::string &str) {
        return trimRight(trimLeft(str));
    }

    // 字符串str是否以字符串start为起始
    bool isStartWith(const std::string &str, const std::string &start) {
        if(start.length() > str.size())
            return false;
        return std::equal(start.begin(), start.end(), str.begin());
    }

    // 字符串str是否以字符串end为结尾
    bool isEndWith(const std::string &str, const std::string &end) {
        if(end.length() > str.size())
            return false;
        return std::equal(end.rbegin(), end.rend(), str.rbegin());
    }

    // 字符串str转化为其他类型
    template<class T>
    T CovFrStr(const std::string &str) {
        T tmp;
        std::basic_istringstream<char> ss(str);
        ss >> tmp;
        return tmp;
    }

    // 其他类型T转换为字符串
    template<class T>
    std::string CovToStr(const T& t) {
        std::basic_ostringstream<char> os;
        os << t;
        return os.str();
    }

    // 用字符串deli拼凑字符串数组tokens
    std::string Join(const std::vector<std::string> &tokens, const std::string &deli) {
        auto tmp = Compact(tokens);
        std::stringstream ss;
        for(size_t i=0; i<tmp.size()-1; i++)
            ss << tmp[i] << deli;
        ss << tmp[tmp.size()-1];
        return ss.str();
    }

    // 用字符串deli分割字符串str
    std::vector<std::string> SplitByStr(const std::string &str, const std::string &deli) {
        size_t pos, lastPos = 0;
        int len;
        std::vector<std::string> tokens;
        while(true)
        {
            pos = str.find(deli, lastPos);
            if(pos == std::string::npos)
                pos = str.size();

            len = pos - lastPos;
            if(len != 0)
                tokens.push_back(str.substr(lastPos, len));

            if(pos == str.size())
                break;
            else
                lastPos = pos+deli.size();
        }
        return tokens;
    }

    // 用字符串deli里的字符分割字符串str
    std::vector<std::string> SplitByC(const std::string &str, const std::string &deli) {
        unsigned int pos = 0, lastPos = 0;
        unsigned int len;
        std::vector<std::string> tokens;
        for(unsigned int i=0; i<str.length(); i++)
        {
            if(deli.find(str[i]) != std::string::npos)
                lastPos = i;
            else
                continue;

            len = lastPos - pos;
            if(len != 0)
                tokens.push_back(str.substr(pos, len));

            pos = lastPos+1;
        }

        if(pos != str.length())
            tokens.push_back(str.substr(pos, str.length()-pos));

        return tokens;
    }

    // 字符串的字符代替
    std::string Replace(const std::string &source, const std::string &target, const std::string &replacement) {
        return Join(SplitByStr(source, target), replacement);
    }

};
#endif
