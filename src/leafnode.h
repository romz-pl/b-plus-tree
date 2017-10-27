#ifndef BPLUSTREE_LEAFNODE_H
#define BPLUSTREE_LEAFNODE_H

#include <tuple>
#include <utility>
#include <vector>
#include "node.h"
#include "record.h"

class LeafNode : public Node
{
public:
    explicit LeafNode( size_t order );
    explicit LeafNode( size_t order, Node* parent );
    ~LeafNode() override;

    using MappingType = std::pair< KeyType, Record* >;
    using EntryType = std::tuple< KeyType, ValueType, LeafNode* >;

    bool isLeaf() const override;
    LeafNode* next() const;
    void setNext( LeafNode* next );
    size_t size() const override;
    size_t minSize() const override;
    size_t maxSize() const override;
    size_t createAndInsertRecord( KeyType key, ValueType value );
    void insert( KeyType key, Record* record );
    Record* lookup( KeyType key ) const;
    size_t removeAndDeleteRecord( KeyType key );
    KeyType firstKey() const;
    void moveHalfTo( LeafNode* recipient );
    void moveAllTo( LeafNode* recipient, size_t /* Unused */ );
    void moveFirstToEndOf( LeafNode* recipient );
    void moveLastToFrontOf( LeafNode* recipient, size_t parentIndex );
    void copyRangeStartingFrom( KeyType key, std::vector< EntryType >& vector );
    void copyRangeUntil( KeyType aKey, std::vector< EntryType >& vector );
    void copyRange( std::vector< EntryType >& vector );
    std::string toString( bool verbose = false ) const override;

private:
    void copyHalfFrom( std::vector< MappingType >& mappings );
    void copyAllFrom( std::vector< MappingType >& mappings );
    void copyLastFrom( MappingType pair );
    void copyFirstFrom( MappingType pair, size_t parentIndex );

private:
    std::vector< MappingType > m_mappings;

    LeafNode* m_next;
};

#endif

