## StringUtils

拓展了std::string的字符串处理函数



#### 函数

```c++
// 字符串str转大写
std::string toUpper(const std::string &str)
// 字符串str转小写
std::string toLower(const std::string &str)
// 字符串str拷贝N次
std::string RepeatN(const std::string &str, unsigned int N)
// 去除字符串数组tokens里的空字符串
std::vector<std::string> Compact(const std::vector<std::string> &tokens)
// 去除字符串str首部的空白字符
std::string trimLeft(const std::string &str)
// 去除字符串str尾部的空白字符
std::string trimRight(const std::string &str)
// 去除字符串str首部尾部的空白字符
std::string trim(const std::string &str)
// 字符串str是否以字符串start为起始
bool isStartWith(const std::string &str, const std::string &start)
// 字符串str是否以字符串end为结尾
bool isEndWith(const std::string &str, const std::string &end)
// 字符串str转化为其他类型
template<class T> T CovFrStr(const std::string &str)
// 其他类型T转换为字符串
template<class T> std::string CovToStr(const T& t)
// 用字符串deli拼凑字符串数组tokens
std::string Join(const std::vector<std::string> &tokens, const std::string &deli)
// 用字符串deli分割字符串str
std::vector<std::string> SplitByStr(const std::string &str, const std::string &deli)
// 用字符串deli里的字符分割字符串str
std::vector<std::string> SplitByC(const std::string &str, const std::string &deli)
// 字符串的字符代替
std::string Replace(const std::string &source, const std::string &target, const std::string &replacement)
```



#### 参考

1. https://github.com/HondaDai/StringUtils

2. https://github.com/sw897/string_utils




