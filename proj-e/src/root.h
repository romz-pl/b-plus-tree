#ifndef ROMZ_BTREE_ROOT_FIELDS_H
#define ROMZ_BTREE_ROOT_FIELDS_H


#include "internal.h"

namespace btree
{

template< typename Key, typename Data >
class Root : public Internal< Key, Data >
{
public:
    typedef typename Node< Key, Data >::size_type          size_type;

public:
    Root( );
    virtual ~Root() = default;

    void init( Internal< Key, Data >* parent );

    Node< Key, Data >* rightmost() const;
    Node< Key, Data >** mutable_rightmost();

    size_type size() const;
    size_type* mutable_size();

private:
    Node< Key, Data > *m_rightmost;
    size_type m_size;
};

//
//
//
template< typename K, typename D >
Root< K, D >::Root(  )
    : m_rightmost( nullptr )
    , m_size( 0 )
{

}

//
//
//
template< typename K, typename D >
void Root< K, D >::init( Internal< K, D >* parent )
{
    Internal< K, D >::init( parent );

    this->m_rightmost = parent;
    assert( parent );
    this->m_size = parent->count();
}

//
// Getter for the rightmost root node field. Only valid on the root node.
//
template< typename K, typename D >
Node< K, D >* Root< K, D >::rightmost() const
{
    return this->m_rightmost;
}

//
//
//
template< typename K, typename D >
Node< K, D >** Root< K, D >::mutable_rightmost()
{
    return &this->m_rightmost;
}

//
// Getter for the size root node field. Only valid on the root node.
//
template< typename K, typename D >
typename Root< K, D >::size_type
Root< K, D >::size() const
{
    return this->m_size;
}

//
//
//
template< typename K, typename D >
typename Root< K, D >::size_type*
Root< K, D >::mutable_size()
{
    return &this->m_size;
}

}

#endif
