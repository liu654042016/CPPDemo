#ifndef PERSON_H
#define PERSON_H
#include <string>
class PersonImpl;
class Person{
public:
    Person(std::string& name);
    virtual ~Person();
    std::string getName();//要暴露的功能接口
private:
    PersonImpl *m_personImpl;//指向实现类的对象
}；

#endif