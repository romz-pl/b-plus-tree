#ifndef BPLUSTREE_RECORD_H
#define BPLUSTREE_RECORD_H

#include "value.h"

class Record
{
public:
    explicit Record( const Value& value );
    ~Record() = default;

    Value value() const;
    std::string toString() const;

private:
    Value m_value;
};

#endif

