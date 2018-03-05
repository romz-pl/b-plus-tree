#include "key.h"

//
//
//
Key::Key( int64_t value )
    : m_value( value )
{

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

