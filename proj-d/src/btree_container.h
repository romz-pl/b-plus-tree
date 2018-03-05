// Copyright 2013 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROMZ_BTREE_BTREE_CONTAINER_H
#define ROMZ_BTREE_BTREE_CONTAINER_H

#include <iosfwd>
#include <utility>

namespace btree {

//
// A common base class for btree_set, btree_map, btree_multiset and btree_multimap.
//
template< typename Tree >
class btree_container
{
    typedef btree_container< Tree > self_type;

public:
    typedef typename Tree::params_type              params_type;
    typedef typename Tree::key_type                 key_type;
    typedef typename Tree::value_type               value_type;
    typedef typename Tree::key_compare              key_compare;
    typedef typename Tree::allocator_type           allocator_type;
    typedef typename Tree::pointer                  pointer;
    typedef typename Tree::const_pointer            const_pointer;
    typedef typename Tree::reference                reference;
    typedef typename Tree::const_reference          const_reference;
    typedef typename Tree::size_type                size_type;
    typedef typename Tree::difference_type          difference_type;
    typedef typename Tree::iterator                 iterator;
    typedef typename Tree::const_iterator           const_iterator;
    typedef typename Tree::reverse_iterator         reverse_iterator;
    typedef typename Tree::const_reverse_iterator   const_reverse_iterator;

public:
    btree_container( const key_compare &comp, const allocator_type &alloc );
    btree_container( const self_type &x );

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;


    iterator lower_bound( const key_type &key );
    const_iterator lower_bound( const key_type &key ) const;

    iterator upper_bound( const key_type &key );
    const_iterator upper_bound( const key_type &key ) const;

    std::pair<iterator,iterator> equal_range( const key_type &key );
    std::pair< const_iterator, const_iterator > equal_range( const key_type &key ) const;

    void clear();
    void swap( self_type &x );
    void dump( std::ostream &os ) const;
    void verify() const;


    size_type size() const;
    size_type max_size() const;
    bool empty() const;
    size_type height() const;
    size_type internal_nodes() const;
    size_type leaf_nodes() const;
    size_type nodes() const;
    size_type bytes_used() const;

    static double average_bytes_per_value();

    double fullness() const;
    double overhead() const;

    bool operator==( const self_type& x ) const;
    bool operator!=( const self_type& other ) const;


protected:
    Tree tree_;
};

//
// Default constructor.
//
template< typename Tree >
btree_container< Tree >::btree_container( const key_compare &comp, const allocator_type &alloc )
    : tree_( comp, alloc )
{
}

//
// Copy constructor.
//
template< typename Tree >
btree_container< Tree >::btree_container( const self_type &x )
    : tree_( x.tree_ )
{
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::iterator
btree_container< Tree >::begin()
{
    return tree_.begin();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::const_iterator
btree_container< Tree >::begin() const
{
    return tree_.begin();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::iterator
btree_container< Tree >::end()
{
    return tree_.end();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::const_iterator
btree_container< Tree >::end() const
{
    return tree_.end();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::reverse_iterator
btree_container< Tree >::rbegin()
{
    return tree_.rbegin();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::const_reverse_iterator
btree_container< Tree >::rbegin() const
{
    return tree_.rbegin();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::reverse_iterator
btree_container< Tree >::rend()
{
    return tree_.rend();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::const_reverse_iterator
btree_container< Tree >::rend() const
{
    return tree_.rend();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::iterator
btree_container< Tree >::lower_bound( const key_type &key )
{
    return tree_.lower_bound( key );
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::const_iterator
btree_container< Tree >::lower_bound( const key_type &key ) const
{
    return tree_.lower_bound( key );
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::iterator
btree_container< Tree >::upper_bound( const key_type &key )
{
    return tree_.upper_bound( key );
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::const_iterator
btree_container< Tree >::upper_bound( const key_type &key ) const
{
    return tree_.upper_bound( key );
}

//
//
//
template< typename Tree >
std::pair< typename btree_container< Tree >::iterator, typename btree_container< Tree >::iterator >
btree_container< Tree >::equal_range( const key_type &key )
{
    return tree_.equal_range( key );
}

//
//
//
template< typename Tree >
std::pair< typename btree_container< Tree >::const_iterator, typename btree_container< Tree >::const_iterator >
btree_container< Tree >::equal_range( const key_type &key ) const
{
    return tree_.equal_range( key );
}

//
//
//
template< typename Tree >
void btree_container< Tree >::clear()
{
    tree_.clear();
}

//
//
//
template< typename Tree >
void btree_container< Tree >::swap( self_type &x )
{
    tree_.swap( x.tree_ );
}

//
//
//
template< typename Tree >
void btree_container< Tree >::dump( std::ostream &os ) const
{
    tree_.dump( os );
}

//
//
//
template< typename Tree >
void btree_container< Tree >::verify() const
{
    tree_.verify();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::size_type
btree_container< Tree >::size() const
{
    return tree_.size();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::size_type
btree_container< Tree >::max_size() const
{
    return tree_.max_size();
}

//
//
//
template< typename Tree >
bool btree_container< Tree >::empty() const
{
    return tree_.empty();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::size_type
btree_container< Tree >::height() const
{
    return tree_.height();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::size_type
btree_container< Tree >::internal_nodes() const
{
    return tree_.internal_nodes();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::size_type
btree_container< Tree >::leaf_nodes() const
{
    return tree_.leaf_nodes();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::size_type
btree_container< Tree >::nodes() const
{
    return tree_.nodes();
}

//
//
//
template< typename Tree >
typename btree_container< Tree >::size_type
btree_container< Tree >::bytes_used() const
{
    return tree_.bytes_used();
}

//
//
//
template< typename Tree >
double btree_container< Tree >::average_bytes_per_value()
{
    return Tree::average_bytes_per_value();
}

//
//
//
template< typename Tree >
double btree_container< Tree >::fullness() const
{
    return tree_.fullness();
}

//
//
//
template< typename Tree >
double btree_container< Tree >::overhead() const
{
    return tree_.overhead();
}

//
//
//
template< typename Tree >
bool btree_container< Tree >::operator==( const self_type& x ) const
{
    if ( size() != x.size() )
    {
        return false;
    }

    for( const_iterator i = begin(), xi = x.begin(); i != end(); ++i, ++xi )
    {
        if( *i != *xi )
        {
            return false;
        }
    }
    return true;
}

//
//
//
template< typename Tree >
bool btree_container< Tree >::operator!=( const self_type& other ) const
{
    return !operator==( other );
}


//
//
//
template< typename T >
inline std::ostream& operator<<( std::ostream &os, const btree_container<T> &b )
{
    b.dump( os );
    return os;
}




} // namespace btree

#endif  // UTIL_BTREE_BTREE_CONTAINER_H__
