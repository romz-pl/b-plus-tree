#ifndef BPLUSTREE_INTERNALNODE_H
#define BPLUSTREE_INTERNALNODE_H


#include <queue>
#include <vector>
#include "definitions.h"
#include "node.h"

class InternalNode : public Node
{
public:
    explicit InternalNode( size_t order );
    explicit InternalNode( size_t order, Node* parent );
    ~InternalNode() override;

    using MappingType = std::pair< KeyType, Node* >;

    bool isLeaf() const override;
    size_t size() const override;
    size_t minSize() const override;
    size_t maxSize() const override;
    KeyType keyAt(int index) const;
    void setKeyAt( int index, KeyType key );
    Node* firstChild() const;
    void populateNewRoot( Node* oldNode, KeyType newKey, Node* newNode );
    size_t insertNodeAfter( Node* oldNode, KeyType newKey, Node* newNode );
    void remove( int index );
    Node* removeAndReturnOnlyChild();
    KeyType replaceAndReturnFirstKey();
    void moveHalfTo( InternalNode* recipient );
    void moveAllTo( InternalNode* recipient, int indexInParent );
    void moveFirstToEndOf( InternalNode* recipient );
    void moveLastToFrontOf( InternalNode* recipient, int parentIndex );
    Node* lookup( KeyType key ) const;
    int nodeIndex( Node* node ) const;
    Node* neighbor( int index ) const;
    std::string toString( bool verbose = false ) const override;
    void queueUpChildren( std::queue< Node* >* queue );

private:
    void copyHalfFrom( std::vector< MappingType >& mappings );
    void copyAllFrom( std::vector< MappingType >& mappings );
    void copyLastFrom( MappingType pair );
    void copyFirstFrom( MappingType pair, int parentIndex );

private:
    std::vector<MappingType> m_mappings;
};

#endif

