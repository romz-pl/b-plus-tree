#include "record.h"

//
//
//
Record::Record( const Value &value )
    : m_value( value )
{

}

//
//
//
Value Record::value() const
{
    return m_value;
}

//
//
//
std::string Record::toString() const
{
    return m_value.ToString();
}
