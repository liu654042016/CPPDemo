#ifndef PERSONIMPL_H
#define PERSONIMPL_H
#include <string>
class PersonImpl{
    public:
    PersonImpl(std::string& name);
    std::string& getName();
    private:
    std::string m_name;
};


#endif