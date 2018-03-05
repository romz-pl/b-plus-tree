#ifndef BPLUSTREE_VALUE_H
#define BPLUSTREE_VALUE_H

//
// 1. This represent value in B+tree.
//
// 2. The current implementation stores "std::string".
//
// 3. If you need other type, just change "std::string" to your type D.
//

#include <string>

class Value
{
public:
    Value();
    explicit Value( const std::string& v );
    ~Value() = default;

    std::string ToString() const;

private:
    std::string m_value;
};


#endif

