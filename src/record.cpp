#include <string>
#include <sstream>
#include "definitions.h"
#include "record.h"

//
//
//
Record::Record( Value value )
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
    std::ostringstream oss;
    oss << m_value.ToString();
    return oss.str();
}
