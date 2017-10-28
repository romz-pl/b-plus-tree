#ifndef BPLUSTREE_INTERNALNODE_H
#define BPLUSTREE_INTERNALNODE_H


#include <queue>
#include <vector>
#include "definitions.h"
#include "node.h"
#include "internalmapping.h"

class InternalNode : public Node
{
public:
    explicit InternalNode( size_t order );
    explicit InternalNode( size_t order, Node* parent );
    ~InternalNode() override;

    bool isLeaf() const override;
    size_t size() const override;
    size_t minSize() const override;
    size_t maxSize() const override;
    Key keyAt( size_t index) const;
    void setKeyAt( size_t index, const Key& key );
    Node* firstChild() const;
    void populateNewRoot( Node* oldNode, const Key& newKey, Node* newNode );
    size_t insertNodeAfter( Node* oldNode, const Key& newKey, Node* newNode );
    void remove( size_t index );
    Node* removeAndReturnOnlyChild();
    Key replaceAndReturnFirstKey();
    void moveHalfTo( InternalNode* recipient );
    void moveAllTo( InternalNode* recipient, size_t indexInParent );
    void moveFirstToEndOf( InternalNode* recipient );
    void moveLastToFrontOf( InternalNode* recipient, size_t parentIndex );
    Node* lookup( const Key& key ) const;
    size_t nodeIndex( Node* node ) const;
    Node* neighbor( size_t index ) const;
    std::string toString( bool verbose = false ) const override;
    void queueUpChildren( std::queue< Node* >* queue );

private:
    void copyHalfFrom( std::vector< InternalMapping >& mappings );
    void copyAllFrom( std::vector< InternalMapping >& mappings );
    void copyLastFrom( const InternalMapping& pair );
    void copyFirstFrom( const InternalMapping& pair, size_t parentIndex );

private:
    std::vector< InternalMapping > m_mappings;
};

#endif

