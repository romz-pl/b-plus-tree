#ifndef ROMZ_BTREE_NODE_H
#define ROMZ_BTREE_NODE_H


/// The header structure of each node in-memory. This structure is extended
/// by inner_node or leaf_node.
struct node
{
    /// Size type used to count keys
    typedef size_t size_type;


    /// Level in the b-tree, if level == 0 -> leaf node
    size_type level;

    /// Number of key slotuse use, so number of valid children or data
    /// pointers
    size_type slotuse;

    /// Delayed initialisation of constructed node
    inline node(const size_type l, const size_type s = 0)
        : level(l), slotuse(s)
    { }

    /// True if this is a leaf node
    inline bool    isleafnode() const
    {
        return (level == 0);
    }
};


#endif

