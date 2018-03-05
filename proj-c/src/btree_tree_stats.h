#ifndef ROMZ_BTREE_TREE_STATS_H
#define ROMZ_BTREE_TREE_STATS_H


// *** Small Statistics Structure

/** A small struct containing basic statistics about the B+ tree. It can be
 * fetched using get_stats(). */


template< typename _Traits >
struct btree_tree_stats
{
    typedef size_t size_type;


    /// Number of items in the B+ tree
    size_type                   itemcount;

    /// Number of leaves in the B+ tree
    size_type                   leaves;

    /// Number of inner nodes in the B+ tree
    size_type                   innernodes;

    /// Base B+ tree parameter: The number of key/data slots in each leaf
    static const size_type leafslots = _Traits::leafslotmax;

    /// Base B+ tree parameter: The number of key slots in each inner node.
    static const size_type innerslots = _Traits::innerslotmax;

    /// Zero initialized
    inline btree_tree_stats()
        : itemcount(0),
          leaves(0), innernodes(0)
    { }

    /// Return the total number of nodes
    inline size_type            nodes() const
    {
        return innernodes + leaves;
    }

    /// Return the average fill of leaves
    inline double               avgfill_leaves() const
    {
        return static_cast<double>(itemcount) / (leaves * leafslots);
    }
};


#endif
