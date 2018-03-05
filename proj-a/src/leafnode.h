#ifndef BPLUSTREE_LEAFNODE_H
#define BPLUSTREE_LEAFNODE_H

#include <tuple>
#include <utility>
#include <vector>
#include "node.h"
#include "record.h"
#include "leafmapping.h"

class LeafNode : public Node
{
public:
    LeafNode( size_t order, InternalNode *parent );
    virtual ~LeafNode();

    bool isLeaf() const override;
    LeafNode* next() const;
    void setNext( LeafNode* next );
    size_t size() const override;
    size_t minSize() const override;
    size_t maxSize() const override;
    size_t createAndInsertRecord( const Key& key, const Value& value );
    void insert( const Key& key, Record* record );
    Record* lookup( const Key& key ) const;
    size_t removeAndDeleteRecord( const Key& key );
    Key firstKey() const;
    void moveHalfTo( LeafNode* recipient );
    void moveAllTo( LeafNode* recipient );
    void moveFirstToEndOf( LeafNode* recipient );
    void moveLastToFrontOf( LeafNode* recipient, size_t parentIndex );

    std::string toString( bool verbose = false ) const override;

    LeafNode* split( size_t order );
    void redistribute( LeafNode* node, size_t index );

    LeafNode* getLeafNode() override;

private:
    void copyHalfFrom( std::vector< LeafMapping >& mappings );
    void copyAllFrom( std::vector< LeafMapping >& mappings );
    void copyLastFrom( const LeafMapping& pair );
    void copyFirstFrom( const LeafMapping& pair, size_t parentIndex );

private:
    std::vector< LeafMapping > m_mappings;

    LeafNode* m_next;
};

#endif

