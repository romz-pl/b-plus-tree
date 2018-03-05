#ifndef ROMZ_BTREE_INNER_NODE_H
#define ROMZ_BTREE_INNER_NODE_H


#include "node.h"


/// Extended structure of a inner node in-memory. Contains only keys and no
/// data items.
///



template< typename _Key,
          typename _Traits,
          typename _Alloc >
struct btree_inner_node : public node
{
    typedef _Key key_type;

    typedef _Traits traits;

    /// Define an related allocator for the inner_node structs.
    typedef typename _Alloc::template rebind< btree_inner_node >::other alloc_type;

    static const size_t innerslotmax = traits::innerslots;

    static const size_t mininnerslots = (innerslotmax / 2);

    /// Keys of children or data pointers
    key_type slotkey[ innerslotmax ];

    /// Pointers to children
    node     * childid[ innerslotmax + 1 ];

    /// Set variables to initial values
    inline btree_inner_node(const size_type l)
        : node(l)
    { }

    /// Construction during restore from node top
    inline btree_inner_node(const node& top)
        : node(top.level, top.slotuse)
    { }

    /// True if the node's slots are full
    inline bool isfull() const
    {
        return (node::slotuse == innerslotmax);
    }

    /// True if few used entries, less than half full
    inline bool isfew() const
    {
        return (node::slotuse <= mininnerslots);
    }

    /// True if node has too few entries
    inline bool isunderflow() const
    {
        return (node::slotuse < mininnerslots);
    }
};


#endif
