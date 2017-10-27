#ifndef BPLUSTREE_RECORD_H
#define BPLUSTREE_RECORD_H

#include "definitions.h"

class Record
{
public:
    explicit Record( Value value );
    ~Record() = default;

    Value value() const;
    std::string toString() const;

private:
    Value m_value;
};

#endif

