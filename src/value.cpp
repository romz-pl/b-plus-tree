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
Value::Value( const int64_t& v )
    : m_value( v )
{

}

//
//
//
bool Value::operator == ( const Value& v ) const
{
    return m_value == v.m_value;
}

//
//
//
bool Value::operator != ( const Value& v ) const
{
    return !( m_value == v.m_value );
}

//
//
//
std::string Value::ToString() const
{
    return std::to_string( m_value );
}
