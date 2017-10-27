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
std::string Value::ToString() const
{
    return std::to_string( m_value );
}
