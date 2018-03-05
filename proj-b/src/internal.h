#ifndef BPLUSTREE_INTERNAL_H
#define BPLUSTREE_INTERNAL_H

//
// 1. The internal (non-leaf) nodes of the B+-tree form a multilevel
//    index on the leaf nodes.
//
// 2. Each internal node contains:
//       a) search-key values $K_1, K_2, ..., K_{w-1}$,
//       b) pointers $P_1, P_2, ..., P_w$ to other nodes (leaf or internal).
//    where $w$ satisfies the condition:
//      ceil(N/2) <= w <= N,
//    where $N$ is the order of the B+ tree.
//
// 3. Hence, a internal (non-leaf) node:
//    a) may hold up to $N$ pointers,
//    b) must hold at least ceil(N/2) pointers.
//
// 4. Let us consider a node containing m pointers $(m ≤ n)$. Then:
//    a) For $i = 2, 3, ..., m−1$, pointer $P_i$ points to the subtree that contains
//       search-key values less than $K_i$ and greater than or equal to $K_{i-1}$.
//    b) Pointer $P_m$ points to the part of the subtree that
//       contains those key values greater than or equal to $K_{m−1}$.
//    c) Pointer $P_1$ points to the part of the subtree that contains those
//       search-key values less than $K_1$.
//
// 4. Unlike other nonleaf nodes, the root node can hold fewer than ceil(N/2) pointers.
//    However, it must hold at least two pointers, unless the tree consists of only one node.
//
//


#include "node.h"
#include <vector>
#include "key.h"


class Internal : public Node
{
public:
    explicit Internal( Internal* parent );
    virtual ~Internal() = default;

    virtual Internal* GetInternal() override;

    virtual bool IsLeaf() const override;

    bool IsFull() const;

    Node* GetChild( const Key& key );

    void Init( Node* left, const Key& key, Node* right );
    void Insert( Node *after, const Key& key, Node* child );

    void SplitInsert( Node *after, const Key& key, Node* child );
private:

    std::vector< Key > m_key;

    std::vector< Node* > m_child;

};

#endif

