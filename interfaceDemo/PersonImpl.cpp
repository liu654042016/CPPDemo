#include "PersonImpl.h"

#include <string>

PersonImpl::PersonImpl(std::string& name)
{
    m_name = name;
}

PersonImpl::~PersonImpl()
{
}

std::string& PersonImpl::getName()
{
    return m_name;
}


