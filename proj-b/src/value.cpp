#include "value.h"

//
//
//
Value::Value()
{

}

//
//
//
Value::Value(const std::string &v )
    : m_value( v )
{

}

//
//
//
std::string Value::ToString() const
{
    return m_value;
}
