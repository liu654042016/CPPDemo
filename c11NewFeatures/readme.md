#! https://zhuanlan.zhihu.com/p/483072475
<!--
 * @Author: LIU KANG
 * @Date: 2022-03-18 11:10:18
 * @LastEditors: LIU KANG
 * @LastEditTime: 2022-03-31 20:31:36
 * @FilePath: \CPPDemo\c11NewFeatures\readme.md
 * @Description: c++11 new features
 * 
 * Copyright (c) 2022 by 用户/公司名, All Rights Reserved. 
-->
#c++11 new features
##std::function
参考网站：https://yangqi.xyz/2021/04/28/210428_cpp_std_function/
###对函数的封装
```
double add(double a, double b)
{
    return a + b;
}
```

```
std::function<double(double, double)> f_add = add;
auto result = f_add(4, 5);//结果返回9
```
###Lambda表达式的封装
Lambda 表达式完整声明
```
[capture list](params list) mutable exception - > return type {function body}
```
capture list：捕获外部变量列表
params list：形参列表
mutable指示符：用来说用是否可以修改捕获的变量
exception：异常设定
return type：返回类型
function body：函数体
```
bool cmp(int a, int b)
{
    return  a < b;
}
sort(myvec.begin(), myvec.end(), cmp);// myvec是一个int类型的无序变量
```
Lambda表达式写法
```
sort(myvec.begin(), myvec.end(),[](int a, int b)->bool {return a < b;});

```
封装Lambda表达式
```
std::function<double(double, double)> f_mul = [](double x, double y) { return x * y; };

```
###using和typedef
参考网站：https://www.cnblogs.com/lifexy/p/14098103.html
https://www.runoob.com/w3cnote/cpp-func-pointer.html
####类函数指针
typedef写法`typedef void (A::*PFUN)(int num);`
using写法`using PFUN = void (A::*)(int num);`
类成员函数别名调用`this->*PFUN(int num)`

####enum使用方法
```
typedef enum tag_DateTime {
    DATETIME_YEAR,
    DATETIME_MONTH,
    DATETIME_DAY,
} Typedf_DATE;

using Using_DATE = enum tag_Date {
    DATE_YEAR,
    DATE_MONTH,
    DATE_DAY,
};
using Using_DATE = enum {
    DATE_YEAR,
    DATE_MONTH,
    DATE_DAY,
};
```
###可变参数宏
参考网站：http://wengsht.github.io/wiki/cs/C/define.html
https://codeantenna.com/a/f84j0u6jQd
1.#
假如希望在字符串中包含宏参数，ANSI C允许这样作，在类函数宏的替换部分，#符号用作一个预处理运算符，它可以把语言符号转化程字符串。例如，如果x是一个宏参量，那么#x可以把参数名转化成相应的字符串。该过程称为字符串化（stringizing）.
```
#incldue <stdio.h>
#define PSQR(x) printf("the square of" #x "is %d.\n",(x)*(x))
int main(void)
{
    int y =4;
    PSQR(y);
    PSQR(2+4);
    return 0;
}
输出结果：
the square of y is 16.
the square of 2+4 is 36.
第一次调用宏时使用“y”代替#x；第二次调用时用“2+4"代#x。
```
2.##
。##运算符可以用于类函数宏的替换部分。另外，##还可以用于类对象宏的替换部分。这个运算符把两个语言符号组合成单个语言符号。
```
#define XNAME(n) x##n
这样宏调用：
XNAME(4)
展开后：
x4
```
3.可变参数宏 ...和_ _VA_ARGS_ _
q__VA_ARGS__ 是一个可变参数的宏，很少人知道这个宏，这个可变参数的宏是新的C99规范中新增的，目前似乎只有gcc支持（VC6.0的编译器不支持）。
实现思想就是宏定义中参数列表的最后一个参数为省略号（也就是三个点）。这样预定义宏_ _VA_ARGS_ _就可以被用在替换部分中，替换省略号所代表的字符串。比如：
```
#define PR(...) printf(__VA_ARGS__)
int main()
{
    int wt=1,sp=2;
    PR("hello\n");
    PR("weight = %d, shipping = %d",wt,sp);
    return 0;
}
输出结果：
hello
weight = 1, shipping = 2
省略号只能代替最后面的宏参数。
```
