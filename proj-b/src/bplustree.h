#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

//
// 1. B+ tree is a balanced tree, i.e. every path from the root of the
//    tree to a leaf of the tree is of the same length.
//
// 2. In B+ tree there are:
//    a) Leaf nodes containing keay and data.
//    b) Internal nodes (non leaf nodes) containing kesy and pointers.
//
// 3. Insertion and deletion keeps the tree balanced.
//
// 4. B+ tree does not holds duplicate keys.
//


#include "key.h"
#include "value.h"
#include "leaf.h"
#include "internal.h"


class BPlusTree
{
public:
    explicit BPlusTree( size_t order );
    ~BPlusTree();

    bool IsEmpty() const;
    Value Get( const Key& key );
    void Insert( const Key& key, const Value& value );
    void Remove( const Key& key );
    size_t GetSize() const;

private:
    Value* Find( const Key& key );
    void InsertInParent( Leaf* oldL , const Key& key, Leaf* newL );


private:
    Node* m_root;

};

#endif

