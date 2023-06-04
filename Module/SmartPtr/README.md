## SmartPtr

#### unique_ptr的使用

##### 分配内存

与shared_ptr不同，unique_ptr没有定义类似make_shared的操作，因此只能使用new来分配内存，并且由于unique_ptr不可拷贝和赋值，初始化unique_ptr必须使用直接初始化的方式

```c++
unique_ptr<int> up1(new int());    // okay, 直接初始化
unique_ptr<int> up2 = new int();   // error! 构造函数是explicit
unique_ptr<int> up3(up1);          // error! 不允许拷贝
```

##### 不可拷贝和赋值

与shared_ptr不同，在某一时刻，只能有一个unique_ptr指向特定的对象，不允许多个unique_ptr指向同一个对象，所以不允许拷贝与赋值

##### unique_ptr的操作

```c++
unique_ptr<T> up 
// 空的unique_ptr，可以指向类型为T的对象，默认使用delete来释放内存

unique_ptr<T,D> up(d) 
// 空的unique_ptr，同上，接受一个D类型的删除器d，使用删除器d来释放内存

up = nullptr 
// 释放up指向的对象，将up置为空

up.release() 
// up放弃对它所指对象的控制权，并返回保存的指针，将up置为空，不会释放内存

up.reset(…) 
// 参数可以为空或者指针，先将up所指对象释放，然后重置up的值
```

##### 传递unique_ptr参数和返回unique_ptr

一般情况下unique_ptr不可拷贝和赋值，事实上不能拷贝unique_ptr的规则有一个例外：我们可以拷贝或赋值一个将要被销毁的unique_ptr（C++ Primer 5th p418）

```c++
// 从函数返回一个unique_ptr
unique_ptr func1(int a) {
    return unique_ptr<int>(new int(a));
}
 
// 返回一个局部对象的拷贝
unique_ptr func2(int a) {
    unique_ptr<int> up(new int(a));
    return up;
}
```

传unique_ptr参数可以使用引用避免所有权的转移，或者暂时移交所有权

```c++
void func1(unique_ptr<int>& up) {
    cout << *up << endl;
}
unique_ptr<int> func2(unique_ptr<int> up) {
    cout << *up << endl;
    return up;
}
 
// 使用up作为参数
unique_ptr<int> up(new int(10));
 
// 传引用，不拷贝，不涉及所有权的转移
func1(up);
// 暂时转移所有权，函数结束时返回拷贝，重新收回所有权
up = func2(unique_ptr<int>(up.release()));
// 如果不用up重新接受func2的返回值，这块内存就泄漏了
```

##### 向unique_ptr传递删除器

类似shared_ptr，用unique_ptr管理非new对象、没有析构函数的类时，需要向unique_ptr传递一个删除器。不同的是，unique_ptr管理删除器的方式，我们必须在尖括号中unique_ptr指向类型后面提供删除器的类型，在创建或reset一个这种unique_ptr对象时，必须提供一个相同类型的可调用对象（删除器），这个删除器接受一个T*参数

##### 不要与裸指针混用

unique_ptr不允许两个独占指针指向同一个对象，在没有裸指针的情况下，我们只能用release获取内存的地址，同时放弃对对象的所有权，这样就有效避免了多个独占指针同时指向一个对象，而使用裸指针就很容器打破这一点

```c++
int* x(new int());
unique_ptr<int> up1, up2;
// 会使up1 up2指向同一个内存
up1.reset(x);
up2.reset(x);
```

##### 记得使用u.release()的返回值

在调用`u.release()`时不会释放u所指的内存，这时返回值就是对这块内存的唯一索引，如果没有使用这个返回值释放内存或是保存起来，这块内存就泄漏了



#### shared_ptr的使用

##### 分配内存

- make_shared

  ```c++
  // make_shared<int>分配一块int类型大小的内存，并值初始化为100
  // 返回值是shared_ptr类型，因此可以直接赋值给sp
  shared_ptr<int> sp = make_shared<int>(100);
  ```

- new

  接受指针参数的智能指针构造函数是explicit的，因此，我们不能将一个内置指针隐式转化为一个智能指针，而是必须使用直接初始化形式

  ```c++
  // 错误！不会进行隐式转换，类型不符合
  shared_ptr<int> sp1 = new int(100);
  // 正确，直接初始化调用构造函数
  shared_ptr<int> sp2(new int(100000));
  ```

##### shared_ptr的操作

```c++
p.get() 
// 返回p保存的指针

swap(p,q) 
// 交换p、q中保存的指针

shared_ptr<T> p(q) 
// p是q的拷贝，它们指向同一块内存，互相关联

p = q 
// 用q为p赋值，之后p、q指向同一块内存，q引用计数+1，p（原来内存空间的）引用计数-1

p.use_count() 
// 返回与p共享对象的智能指针数量

shared_ptr<T> p(q, d) 
// q是一个可以转换为T*的指针，d是一个可调用对象（作为删除器），p接管q所指对象的所有权，用删除器d代替delete释放内存

p.reset() 
// 将p重置为空指针

p.reset(p) 
// 将p重置为p（的值）

p.reset(p,d) 
// 将p重置为p（的值）并使用d作为删除器
```

##### shared_ptr的关联与独立

多个共享指针指向同一个空间，它们的关系可能是关联（我们所期望的正常关系）或是独立的（一种错误状态）

```c++
shared_ptr<int> sp1(new int(10));
shared_ptr<int> sp2(sp1), sp3;
sp3 = sp1;
// 一个典型的错误用法
shared_ptr<int> sp4(sp1.get()); 
cout << sp1.use_count() << " " << sp2.use_count() << " " 
    << sp3.use_count() << " " << sp4.use_count() << endl;
// 输出 3 3 3 1
```

sp1，sp2，sp3是相互关联的共享指针，共同控制所指内存的生存期，sp4虽然指向同样的内存，却是与sp1，sp2，sp3独立的，sp4按自己的引用计数来关联内存的释放

**只有用一个shared_ptr为另一个shared_ptr赋值时，才将这连个共享指针关联起来，直接使用地址值会导致各个shared_ptr独立**

##### 向shared_ptr传递删除器

有时候我们需要用智能指针管理非new的对象，或者是没有析构函数的类，由于shared_ptr默认使用delete来释放内存并执行析构函数，对于以上的两种情况是不适用的，所以我们要传递特别的删除器

```c++
// 没有析构函数的类
struct MyStruct {
    int* p;
    MyStruct():p(new int(10)) { }   // 用裸指针管理，不用时需要手动释放
};
 
void main() {
    // st是局部的对象，存放在栈区
    // 并非由new申请，不可用delete释放内存
    MyStruct st;
    {
        shared_ptr<MyStruct> sp(&st, [](MyStruct* ptr) {
            delete(ptr->p);
            ptr->p = nullptr;
            cout << "destructed." << endl;
        });
    }
    // 离开作用域，调用传递的删除器释放sp所指的内存空间
}
```

##### 不要写出独立的shared_ptr

 不要与裸指针混用 

```c++
// 错误场景1
int* x(new int(10));
shared_ptr<int> sp1(x);
shared_ptr<int> sp2(x);
// 虽然sp1、sp2都指向x所指的内存，但他们是独立的
// 会在其他shared_ptr还在使用内存的情况下就释放掉内存
// 失去了设计共享指针的意义
// 同时，使用裸指针x本身也是很危险的，x随时可能变成空悬指针而无从知晓
```

```c++
// 错误场景2
void func(shared_ptr<int> sp);
 
int* x(new int(10));
// 创建了一个指向x指针所指内存的共享指针，引用计数为1，是引用这块内存的唯一共享指针
func(shared_ptr<int>(x));
// 离开函数即离开共享指针的作用域，这块内存即被删除
```

 不要用get()的返回值为shared_ptr赋值 

```c++
shared_ptr<int> sp1(new int(10));
// sp2与sp1独立
shared_ptr<int> sp2(sp1.get()), sp3;
// sp3与sp1独立
sp.reset(sp1.get());
```

##### 谨慎使用p.get()的返回值

- 不要保存p.get()的返回值

  无论是保存为裸指针还是shared_ptr都是错误的，保存为裸指针不知什么时候就会变成空悬指针 ，保存为shared_ptr则产生了独立指针

- 不要delete p.get()的返回值

  会导致对一块内存delete两次的错误

##### 记得向shared_ptr传递删除器

如果用shared_ptr管理非new对象或是没有析构函数的类时，应当为其传递合适的删除器

##### 避免形成指针循环引用



#### weak_ptr的使用

- weak_ptr是一种不控制对象生命周期的智能指针, 它指向一个shared_ptr管理的对象. 对该对象进行内存管理的是那个强引用的shared_ptr，weak_ptr只是提供了对管理对象的一个访问手段

- weak_ptr设计的目的是为了协助shared_ptr工作，它只能从一个shared_ptr或者另一个weak_ptr对象构造，它的构造和析构不会引起引用记数的增加或减少 

- weak_ptr支持拷贝和赋值，不会影响对象的引用计数，但weak_ptr不支持`*`和`->`


```c++
std::shared_ptr<int> sp(new int(10));
std::weak_ptr<int> wp(sp);
printf("%d\n", wp.use_count()); // 1
wp.reset();
printf("%d\n", wp.use_count()); // 0

// 检查 weak_ptr 内部对象的合法性.
if (std::shared_ptr<int> sp = wp.lock()) {
    ...
}
```

##### weak_ptr的操作

```c++
p.reset()      // 置空weak_ptr   
p.expired()    // 检测对象是否已经释放，如果已经释放，返回true；否则返回false
p.lock()       // 获取对象的强引用(shared_ptr)，如果对象已经释放，返回空的shared_ptr
    		   // 需要检查合法性
p.use_count()  // 返回对象的引用计数
```

#####  处理循环引用

- 使用shared_ptr时，shared_ptr为强引用，如果存在循环引用，将导致内存泄露。使用weak_ptr解决循环引用问题

- weak_ptr在功能上类似于普通指针，然而一个比较大的区别是，弱引用能检测到所管理的对象是否已经释放，从而避免访问非法内存


```c++
#include <iostream>
#include <memory>

class CB;
class CA;

class CA {
public:
    CA() {}
    ~CA() { printf("~CA()\n"); }

    void Register(const std::shared_ptr<CB>& sp) {
        m_spb = sp;
    }

private:
    std::shared_ptr<CB> m_spb;
};

class CB {
public:
    CB(){};
    ~CB(){ printf("~CB()\n"); };

    void Register(const std::shared_ptr<CA>& sp) {
        m_spa = sp;
    }

private:
    std::shared_ptr<CA> m_spa;
};

int main() {
    std::shared_ptr<CA> spa(new CA);
    std::shared_ptr<CB> spb(new CB);
    spa->Register(spb);
    spb->Register(spa);
    printf("%ld\n", spa.use_count()); // 2
    printf("%ld\n", spb.use_count()); // 1
    return 0;
}
```



#### 参考

1. [shared_ptr的使用和陷阱](https://blog.csdn.net/qq_33266987/article/details/78784852)
2. [unique_ptr的使用和陷阱](https://blog.csdn.net/qq_33266987/article/details/78784286)
3. [C++智能指针 weak_ptr](https://www.cnblogs.com/diysoul/p/5930372.html)