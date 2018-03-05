#ifndef ROMZ_BTREE_NODE_STATS_H
#define ROMZ_BTREE_NODE_STATS_H

// #include <cstddef>

//
//
//
struct node_stats
{
    node_stats( ssize_t l, ssize_t i )
    : leaf_nodes( l ),
      internal_nodes( i )
    {
    }

    node_stats& operator+=( const node_stats &x )
    {
        leaf_nodes += x.leaf_nodes;
        internal_nodes += x.internal_nodes;
        return *this;
    }

    ssize_t leaf_nodes;
    ssize_t internal_nodes;
};

#endif
