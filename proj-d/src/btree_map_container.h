#ifndef ROMZ_BTREE_MAP_CONTAINER_H
#define ROMZ_BTREE_MAP_CONTAINER_H


#include "btree_unique_container.h"

namespace btree
{

//
// A common base class for btree_map and safe_btree_map.
//
template< typename Tree >
class btree_map_container : public btree_unique_container< Tree >
{
    typedef btree_map_container< Tree >     self_type;
    typedef btree_unique_container< Tree >  super_type;

public:
    typedef typename Tree::key_type       key_type;
    typedef typename Tree::data_type      data_type;
    typedef typename Tree::value_type     value_type;
    typedef typename Tree::mapped_type    mapped_type;
    typedef typename Tree::key_compare    key_compare;
    typedef typename Tree::allocator_type allocator_type;

private:

    // A pointer-like object which only generates its value when
    // dereferenced. Used by operator[] to avoid constructing an empty data_type
    // if the key already exists in the map.
    struct generate_value
    {
        generate_value( const key_type &k )
            : key( k )
        {
        }

        value_type operator*() const
        {
            return std::make_pair( key, data_type() );
        }
        const key_type &key;
    };

public:
    btree_map_container( const key_compare &comp = key_compare(),
                         const allocator_type &alloc = allocator_type() );

    btree_map_container( const self_type &x );

    template< class InputIterator >
    btree_map_container( InputIterator b,
                         InputIterator e,
                         const key_compare &comp = key_compare(),
                         const allocator_type &alloc = allocator_type() );

    data_type& operator[]( const key_type &key );
};

//
// Default constructor.
//
template< typename Tree >
btree_map_container< Tree >::btree_map_container( const key_compare &comp,
                                                  const allocator_type &alloc )
    : super_type( comp, alloc )
{
}

//
// Copy constructor.
//
template< typename Tree >
btree_map_container< Tree >::btree_map_container( const self_type &x )
    : super_type( x )
{
}

//
// Range constructor.
//
template< typename Tree >
template< class InputIterator >
btree_map_container< Tree >::btree_map_container( InputIterator b,
                                                  InputIterator e,
                                                  const key_compare &comp,
                                                  const allocator_type &alloc )
    : super_type( b, e, comp, alloc )
{
}

//
// Insertion routines.
//
template< typename Tree >
typename btree_map_container< Tree >::data_type&
btree_map_container< Tree >::operator[]( const key_type &key )
{
    return this->tree_.insert_unique( key, generate_value( key ) ).first->second;
}

}


#endif

