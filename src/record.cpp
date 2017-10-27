#include <string>
#include <sstream>
#include "definitions.h"
#include "record.h"

Record::Record( ValueType value )
    : m_value( value )
{

}

ValueType Record::value() const
{
    return m_value;
}

std::string Record::toString() const
{
    std::ostringstream oss;
    oss << m_value;
    return oss.str();
}
