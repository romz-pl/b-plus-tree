#include "leaf.h"
#include <cassert>
#include <algorithm>
#include <cmath>


//
//
//
Leaf::Leaf( Internal* parent )
    : Node( parent )
    , m_next( nullptr )
{

    const size_t order = Node::GetOrder();

    m_key.reserve( order - 1 );
    m_value.reserve( order - 1 );
}

//
//
//
Leaf* Leaf::GetLeaf()
{
    return this;
}

//
//
//
bool Leaf::IsLeaf() const
{
    return true;
}

//
//
//
Leaf* Leaf::GetNext()
{
    return m_next;
}

//
//
//
void Leaf::SetNext( Leaf* next )
{
    m_next = next;
}

//
//
//
bool Leaf::IsFull() const
{
    const size_t order = Node::GetOrder();
    assert( m_key.size() == m_value.size() );
    return ( m_key.size() < ( order - 1 ) );
}

//
//
//
Value* Leaf::Find( const Key& key )
{
    assert( std::is_sorted( m_key.begin(), m_key.end() ) );
    const auto it = std::lower_bound( m_key.begin(), m_key.end(), key );
    if( it == m_key.end() )
    {
        return nullptr;
    }

    if( *it != key )
    {
        return nullptr;
    }

    const auto dist = std::distance( it, m_key.begin() );
    assert( dist >= 0 );
    return &( m_value[ dist ] );
}

//
//
//
void Leaf::Append( const Key& key, const Value& value )
{
    m_key.push_back( key );
    m_value.push_back( value ) ;
}

//
//
//
void Leaf::Insert( const Key& key, const Value& value )
{
    if( key < m_key.front() )
    {
        m_key.insert( m_key.begin(), key );
        m_value.insert( m_value.begin(), value );
        return;
    }

    auto it = std::lower_bound( m_key.begin(), m_key.end(), key );
    assert( it != m_key.end() );
    it++;

    m_key.insert( it, key );

    const auto dist = std::distance( it, m_key.begin() );
    assert( dist >= 0 );

    const auto itV = m_value.begin() + dist;
    m_value.insert( itV, value );
}

//
//
//
std::pair< Leaf*, Key > Leaf::SplitInsert( const Key& key, const Value& value )
{
    assert( IsFull() );

    Leaf * newL = new Leaf( nullptr );

    Insert( key, value );

    newL->SetNext( GetNext() );
    SetNext( newL );

    const size_t mid = ceil( Node::GetOrder() / 2 );

    for( size_t i = mid + 1; i < m_key.size(); i++ )
    {
        newL->Append( key, m_value[ i ] );
    }

    const Key smallest = m_key[ mid + 1 ];
    m_key.erase( m_key.begin() + mid, m_key.end() );
    m_value.erase( m_value.begin() + mid, m_value.end() );


    return std::make_pair( newL, smallest );
}
