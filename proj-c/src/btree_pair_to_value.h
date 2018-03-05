#ifndef ROMZ_BTREE_PAIR_TO_VALUE_H
#define ROMZ_BTREE_PAIR_TO_VALUE_H

#include <utility>



/// For sets the second pair_type is an empty struct, so the value_type
/// should only be the first.
template< typename _Key,
          typename _Data,
          typename value_type,
          typename pair_type>
struct btree_pair_to_value
{
    /// Convert a fake pair type to just the first component
    inline value_type operator () (pair_type& p) const
    {
        return p.first;
    }
    /// Convert a fake pair type to just the first component
    inline value_type operator () (const pair_type& p) const
    {
        return p.first;
    }
};

/// For maps value_type is the same as the pair_type
template <typename _Key,
          typename _Data,
          typename value_type>
struct btree_pair_to_value<_Key,_Data,value_type, value_type>
{
    typedef std::pair< _Key, _Data > pair_type;


    /// Identity "convert" a real pair type to just the first component
    inline value_type operator () (pair_type& p) const
    {
        return p;
    }
    /// Identity "convert" a real pair type to just the first component
    inline value_type operator () (const pair_type& p) const
    {
        return p;
    }
};



#endif

