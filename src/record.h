#ifndef BPLUSTREE_RECORD_H
#define BPLUSTREE_RECORD_H

#include "definitions.h"

class Record
{
public:
    explicit Record(ValueType aValue);
    ValueType value() const;
    std::string toString() const;
private:
    Record() : fValue(0) {}
    ValueType fValue;
};

#endif

