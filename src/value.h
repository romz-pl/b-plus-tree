#ifndef BPLUSTREE_VALUE_H
#define BPLUSTREE_VALUE_H

//
// 1. This represent value in B+tree.
//
// 2. The current implementation stores int64_t.
//
// 3. If you need other type, just change int64_t to your type D,
//    and make sure comparision operators are available for type D.
//

#include <string>

class Value
{
public:
    Value();
    explicit Value( const int64_t& v );
    ~Value() = default;

//    bool operator == ( const Value& v ) const;
//    bool operator != ( const Value& v ) const;

    std::string ToString() const;

private:
    int64_t m_value;
};


#endif

