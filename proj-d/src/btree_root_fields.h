#ifndef ROMZ_BTREE_ROOT_FIELDS_H
#define ROMZ_BTREE_ROOT_FIELDS_H


#include "btree_internal_fields.h"

namespace btree
{


template< typename Params >
class btree_node;

template< typename Params >
class btree_root_fields : public btree_internal_fields< Params >
{
public:
    typedef typename Params::size_type          size_type;

public:
    btree_node< Params >* rightmost() const;
    btree_node< Params >** mutable_rightmost();

    size_type size() const;
    size_type* mutable_size();

    void init( btree_node< Params > *parent );

private:
    btree_node< Params > *m_rightmost;
    size_type m_size;
};

//
//
//
template< typename P >
void btree_root_fields< P >::init( btree_node< P > *parent )
{
    btree_internal_fields< P >::init( parent );
    this->m_rightmost = parent;
    this->m_size = parent->count();
}

//
// Getter for the rightmost root node field. Only valid on the root node.
//
template< typename P >
btree_node< P >* btree_root_fields< P >::rightmost() const
{
    return this->m_rightmost;
}

//
//
//
template< typename P >
btree_node< P >** btree_root_fields< P >::mutable_rightmost()
{
    return &this->m_rightmost;
}

//
// Getter for the size root node field. Only valid on the root node.
//
template< typename P >
typename btree_root_fields< P >::size_type
btree_root_fields< P >::size() const
{
    return this->m_size;
}

//
//
//
template< typename P >
typename btree_root_fields< P >::size_type*
btree_root_fields< P >::mutable_size()
{
    return &this->m_size;
}

}

#endif
