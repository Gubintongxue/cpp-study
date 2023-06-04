## Any

仿boost::any，通过定义一个Any类型的变量来存放任意类型的数据



#### 接口

```c++
Any()
Any(const Any& other)
Any& operator=(const Any& other)
Any(Any&& other)

empty()
isType()
any_cast()
```



#### 测试

使用gtest

```c++
TEST(Any, testAny) {
    Any a = 31415;
    Any b = std::string("hello world");
    Any c;

    EXPECT_FALSE(a.empty());
    EXPECT_FALSE(b.empty());
    EXPECT_TRUE(c.empty());

    EXPECT_TRUE(a.isType<int>());
    EXPECT_FALSE(a.isType<std::string>());
    EXPECT_EQ(a.any_cast<int>(), 31415);

    c = b;
    EXPECT_FALSE(c.isType<int>());
    EXPECT_TRUE(c.isType<std::string>());
    EXPECT_TRUE(c.isType<const std::string&>());
    EXPECT_TRUE(c.isType<std::string&&>());
    EXPECT_EQ(c.any_cast<std::string>(), std::string("hello world"));
    // std::cout << c.any_cast<int>() << std::endl;
}
```

