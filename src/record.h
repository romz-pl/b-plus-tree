#ifndef BPLUSTREE_RECORD_H
#define BPLUSTREE_RECORD_H

#include "definitions.h"

class Record
{
public:
    explicit Record( ValueType value );
    ~Record() = default;

    ValueType value() const;
    std::string toString() const;

private:
    ValueType m_value;
};

#endif

