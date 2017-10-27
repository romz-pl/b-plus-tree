#include <string>
#include <sstream>
#include "definitions.h"
#include "record.h"

Record::Record(ValueType aValue)
: fValue(aValue)
{}

ValueType Record::value() const
{
    return fValue;
}

std::string Record::toString() const
{
    std::ostringstream oss;
    oss << fValue;
    return oss.str();
}
