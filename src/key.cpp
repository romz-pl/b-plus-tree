#include "key.h"
#include <limits>
#include <stdexcept>


//
//
//
Key::Key( int64_t value )
    : m_value( value )
{
    if( value < MinValue() )
    {
        throw std::runtime_error( "Key has not allowed value" );
    }
}

//
//
//
bool Key::operator==( const Key& key ) const
{
    return m_value == key.m_value;
}

//
//
//
bool Key::operator!=( const Key& key ) const
{
    return !( m_value == key.m_value );
}


//
//
//
bool Key::operator<( const Key& key ) const
{
    return m_value < key.m_value;
}

//
//
//
bool Key::operator<=( const Key& key ) const
{
 return !( key.m_value < m_value );
}

//
//
//
bool Key::operator>( const Key& key ) const
{
    return key.m_value < m_value;
}

//
//
//
bool Key::operator>=( const Key& key ) const
{
    return !( m_value < key.m_value );
}

//
//
//
std::string Key::ToString() const
{
    return std::to_string( m_value );
}

//
// Key used where only the entry's pointer has the meaning.
// It is assumed that this value is lower then all keys in the B+tree!!!
// See function LeafNode::insert and condition in "while loop"
//
Key Key::Dummy()
{
    return Key( MinValue() );
}

//
//
//
int64_t Key::MinValue()
{
    const int64_t v = std::numeric_limits< int64_t >::min();
    return v;
}
