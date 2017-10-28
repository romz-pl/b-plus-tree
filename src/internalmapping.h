#ifndef BPLUSTREE_INTERNALMAPPING_H
#define BPLUSTREE_INTERNALMAPPING_H

#include "key.h"
#include "node.h"

class InternalMapping
{
public:
    InternalMapping( const Key& key, Node* node );

public:
    Key m_key;
    Node* m_node;
};

#endif
