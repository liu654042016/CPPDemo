#! https://zhuanlan.zhihu.com/p/483072475
<!--
 * @Author: LIU KANG
 * @Date: 2022-03-18 11:10:18
 * @LastEditors: LIU KANG
 * @LastEditTime: 2022-03-28 18:48:32
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