#ifndef BPLUSTREE_LEAF_H
#define BPLUSTREE_LEAF_H

//
// 1. Leaf nodes contains keys and pointer to values (data).
//
// 2. Each leaf contains:
//       a) search-key values $K_1, K_2, ..., K_w$,
//       b) pointers $P_1, P_2, ..., P_w$ to values (data).
//    where $w$ satisfies the condition:
//      ceil((N-1)/2) <= w <= N-1,
//    where $N$ is the order of the B+ tree.
//
// 3. Hence, each leaf can:
//     a) each leaf can hold up to $N − 1$ values.
//     b) each leaf must conatin at leas $ceil((N-1)/2)$ values
//
// 4. The search-key values within a node are kept in sorted order:
//       if  $i < j$,  then  $K_i < K_j$
//
// 5. In particular, if $L_i$ and $L_j$ are leaf nodes and $i < j$, then every
// search-key value in $L_i$ is less than or equal to every search-key value in $L_j$.
//
//

#include "node.h"
#include <vector>
#include "key.h"
#include "value.h"
#include <utility>


class Leaf : public Node
{
public:
    explicit Leaf( Internal* parent );
    virtual ~Leaf() = default;

    virtual Leaf* GetLeaf() override;

    virtual bool IsLeaf() const override;

    Value* Find( const Key& key );

    void Append( const Key& key, const Value& value );
    void Insert( const Key& key, const Value& value );
    std::pair< Leaf*, Key > SplitInsert( const Key& key, const Value& value );

    bool IsFull() const;

    Leaf* GetNext();
    void SetNext( Leaf* next );

private:
    // Vector of keys in the leaf
    std::vector< Key > m_key;

    // Vector of pointers to data
    std::vector< Value > m_value;

    // Pointer to the next leaf
    Leaf* m_next;

};

#endif

