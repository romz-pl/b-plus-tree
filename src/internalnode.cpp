#include <iostream>
#include <sstream>
#include "exceptions.h"
#include "internalnode.h"

//
//
//
InternalNode::InternalNode( size_t order, Node* parent )
    : Node( order, parent )
{

}

//
//
//
InternalNode::~InternalNode()
{
    for( auto mapping : m_mappings )
    {
        delete mapping.m_node;
    }
}

//
//
//
bool InternalNode::isLeaf() const
{
    return false;
}

//
//
//
size_t InternalNode::size() const
{
    return m_mappings.size();
}

//
//
//
size_t InternalNode::minSize() const
{
    return order() / 2;
}

//
//
//
size_t InternalNode::maxSize() const
{
    // Includes the first entry, which
    // has key DUMMY_KEY and a value that
    // points to the left of the first positive key k1
    // (i.e., a node whose keys are all < k1).
    return order();
}

//
//
//
Key InternalNode::keyAt( size_t index ) const
{
    return m_mappings[ index ].m_key;
}

//
//
//
void InternalNode::setKeyAt( size_t index, const Key& key )
{
    m_mappings[ index ].m_key = key;
}

//
//
//
Node* InternalNode::firstChild() const
{
    return m_mappings.front().m_node;
}

//
//
//
void InternalNode::populateNewRoot( Node* oldNode, const Key& newKey, Node* newNode )
{
    m_mappings.push_back( InternalMapping( Key::Dummy(), oldNode ) );
    m_mappings.push_back( InternalMapping( newKey, newNode ) );
}

//
//
//
size_t InternalNode::insertNodeAfter( Node* oldNode, const Key& newKey, Node* newNode )
{
    auto iter = m_mappings.begin();
    for (; iter != m_mappings.end() && iter->m_node != oldNode; ++iter);
    m_mappings.insert( iter + 1, InternalMapping( newKey, newNode ) );
    return size();
}

//
//
//
void InternalNode::remove( size_t index )
{
    m_mappings.erase( m_mappings.begin() + index );
}

//
//
//
Node* InternalNode::removeAndReturnOnlyChild()
{
    Node* firstChild = m_mappings.front().m_node;
    m_mappings.pop_back();
    return firstChild;
}

//
//
//
Key InternalNode::replaceAndReturnFirstKey()
{
    Key newKey = m_mappings[ 0 ].m_key;
    m_mappings[ 0 ].m_key = Key::Dummy();
    return newKey;
}

//
//
//
void InternalNode::moveHalfTo( InternalNode *recipient )
{
    recipient->copyHalfFrom( m_mappings );
    size_t size = m_mappings.size();
    for( size_t i = minSize(); i < size; ++i )
    {
        m_mappings.pop_back();
    }
}

//
//
//
void InternalNode::copyHalfFrom( std::vector< InternalMapping > &mappings )
{
    for( size_t i = minSize(); i < mappings.size(); ++i )
    {
        mappings[ i ].m_node->setParent( this );
        m_mappings.push_back( mappings[ i ] );
    }
}

//
//
//
void InternalNode::moveAllTo( InternalNode *recipient, size_t indexInParent )
{
    m_mappings[ 0 ].m_key = static_cast< InternalNode* >( parent() )->keyAt( indexInParent );
    recipient->copyAllFrom( m_mappings );
    m_mappings.clear();
}

//
//
//
void InternalNode::copyAllFrom( std::vector< InternalMapping > &mappings )
{
    for( auto mapping : mappings )
    {
        mapping.m_node->setParent( this );
        m_mappings.push_back( mapping );
    }
}

//
//
//
void InternalNode::moveFirstToEndOf( InternalNode *recipient )
{
    recipient->copyLastFrom( m_mappings.front() );
    m_mappings.erase( m_mappings.begin() );
    static_cast< InternalNode* >( parent() )->setKeyAt( 1, m_mappings.front().m_key );
}

//
//
//
void InternalNode::copyLastFrom( const InternalMapping& pair )
{
    m_mappings.push_back( pair );
    m_mappings.back().m_node->setParent( this );
}

//
//
//
void InternalNode::moveLastToFrontOf( InternalNode *recipient, size_t parentIndex )
{
    recipient->copyFirstFrom( m_mappings.back(), parentIndex );
    m_mappings.pop_back();
}

//
//
//
void InternalNode::copyFirstFrom( const InternalMapping& pair, size_t parentIndex )
{
    m_mappings.front().m_key = static_cast< InternalNode* >( parent() )->keyAt( parentIndex );
    m_mappings.insert( m_mappings.begin(), pair );
    m_mappings.front().m_key = Key::Dummy();
    m_mappings.front().m_node->setParent( this );
    static_cast< InternalNode* >( parent() )->setKeyAt( parentIndex, m_mappings.front().m_key );
}

//
//
//
Node* InternalNode::lookup( const Key& key ) const
{
    auto locator = m_mappings.begin();
    auto end = m_mappings.end();
    while( locator != end && key >= locator->m_key )
    {
        ++locator;
    }
    // locator->first is now the least key k such that aKey < k.
    // One before is the greatest key k such that aKey >= k.
    --locator;
    return locator->m_node;
}

//
//
//
size_t InternalNode::nodeIndex( Node *node ) const
{
    for( size_t i = 0; i < size(); ++i )
    {
        if ( m_mappings[ i ].m_node == node )
        {
            return i;
        }
    }
    throw NodeNotFoundException( node->toString(), toString() );
}

//
//
//
Node* InternalNode::neighbor( size_t index ) const
{
    return m_mappings[ index ].m_node;
}

//
//
//
std::string InternalNode::toString( bool verbose ) const
{
    if( m_mappings.empty() )
    {
        return "";
    }

    std::ostringstream keyToTextConverter;
    if( verbose )
    {
        keyToTextConverter << "[" << std::hex << this << std::dec << "]<" << m_mappings.size() << "> ";
    }

    auto entry = verbose ? m_mappings.begin() : m_mappings.begin() + 1;
    auto end = m_mappings.end();
    bool first = true;
    while( entry != end )
    {
        if( first )
        {
            first = false;
        }
        else
        {
            keyToTextConverter << " ";
        }
        keyToTextConverter << std::dec << entry->m_key.ToString();
        if( verbose )
        {
            keyToTextConverter << "(" << std::hex << entry->m_node << std::dec << ")";
        }
        ++entry;
    }
    return keyToTextConverter.str();
}

//
//
//
void InternalNode::queueUpChildren( std::queue< Node* >* queue )
{
    for( auto mapping : m_mappings )
    {
        queue->push( mapping.m_node );
    }
}

//
//
//
InternalNode* InternalNode::split( size_t order )
{
    InternalNode* newNode = new InternalNode( order, parent() );
    moveHalfTo( newNode );
    return newNode;
}

//
//
//
void InternalNode::redistribute( InternalNode* node, size_t index )
{
    if( index == 0 )
    {
        moveFirstToEndOf( node );
    }
    else
    {
        moveLastToFrontOf( node, index );
    }
}
