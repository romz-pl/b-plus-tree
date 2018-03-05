#ifndef ROMZ_BTREE_LEAF_NODE_H
#define ROMZ_BTREE_LEAF_NODE_H

#include <cassert>
#include "node.h"


/// Extended structure of a leaf node in memory. Contains pairs of keys and
/// data items. Key and data slots are kept in separate arrays, because the
/// key array is traversed very often compared to accessing the data items.

template< typename _Key,
          typename _Data,
          typename _Value,
          typename _Traits,
          typename _Alloc,
          bool _UsedAsSet >

struct btree_leaf_node : public node
{
    typedef _Key key_type;

    typedef _Data data_type;

    typedef _Traits traits;

    /// Define an related allocator for the leaf_node structs.
    typedef typename _Alloc::template rebind< btree_leaf_node >::other alloc_type;

    typedef std::pair<key_type, data_type> pair_type;


    static const size_type leafslotmax = traits::leafslots;

    static const size_type minleafslots = (leafslotmax / 2);

    static const bool used_as_set = _UsedAsSet;



    /// Double linked list pointers to traverse the leaves
    btree_leaf_node * prevleaf;

    /// Double linked list pointers to traverse the leaves
    btree_leaf_node * nextleaf;

    /// Keys of children or data pointers
    key_type  slotkey[leafslotmax];

    /// Array of data
    data_type slotdata[used_as_set ? 1 : leafslotmax];

    /// Set variables to initial values
    inline btree_leaf_node()
        : node(0), prevleaf(nullptr), nextleaf(nullptr)
    { }

    /// Construction during restore from node top
    inline btree_leaf_node(const node& top)
        : node(top.level, top.slotuse), prevleaf(nullptr), nextleaf(nullptr)
    { }

    /// True if the node's slots are full
    inline bool isfull() const
    {
        return (node::slotuse == leafslotmax);
    }

    /// True if few used entries, less than half full
    inline bool isfew() const
    {
        return (node::slotuse <= minleafslots);
    }

    /// True if node has too few entries
    inline bool isunderflow() const
    {
        return (node::slotuse < minleafslots);
    }

    /// Set the (key,data) pair in slot. Overloaded function used by
    /// bulk_load().
    inline void set_slot(size_type slot, const pair_type& value)
    {
        assert(used_as_set == false);
        assert(slot < node::slotuse);
        slotkey[slot] = value.first;
        slotdata[slot] = value.second;
    }

    /// Set the key pair in slot. Overloaded function used by
    /// bulk_load().
    inline void set_slot(size_type slot, const key_type& key)
    {
        assert(used_as_set == true);
        assert(slot < node::slotuse);
        slotkey[slot] = key;
    }
};


#endif
