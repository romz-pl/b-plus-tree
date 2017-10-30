#ifndef BPLUSTREE_KEY_H
#define BPLUSTREE_KEY_H

//
// 1. This represents key in B+tree.
//
// 2. Current implemenation is specific for type "int64_t".
//
// 3. If you want other type, just change "int64_t" to your required type T,
//    and make sure the compariosion operator are available for type T.
//

#include <cstdint>
#include <string>

class Key
{
public:
    explicit Key( int64_t value );
    ~Key() = default;

    bool operator==( const Key& key ) const;
    bool operator!=( const Key& key ) const;

    bool operator<( const Key& key ) const;
    bool operator<=( const Key& key ) const;
    bool operator>( const Key& key ) const;
    bool operator>=( const Key& key ) const;

    std::string ToString() const;

    static Key Dummy();


private:
    static int64_t MinValue();

private:
    int64_t m_value;
};

#endif
