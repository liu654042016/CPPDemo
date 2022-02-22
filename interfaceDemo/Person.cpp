#include "Person.h"
#include "PersonImpl.h"

Person::Person(std::string& name){
    m_personImpl = new PersonImpl(name);
}
Person::~Person(){
    if(m_personImpl){
        delete m_personImpl;
    }
}
std::string& Person::getName(){
    return m_personImpl->getName();//真正调用的是实现类的功能接口
}