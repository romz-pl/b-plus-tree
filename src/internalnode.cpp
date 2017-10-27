#include <iostream>
#include <sstream>
#include "exceptions.h"
#include "internalnode.h"

//
//
//
InternalNode::InternalNode( size_t order )
    : Node( order )
{

}

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
        delete mapping.second;
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
    return m_mappings[ index ].first;
}

//
//
//
void InternalNode::setKeyAt( size_t index, Key key )
{
    m_mappings[ index ].first = key;
}

//
//
//
Node* InternalNode::firstChild() const
{
    return m_mappings.front().second;
}

//
//
//
void InternalNode::populateNewRoot( Node *oldNode, Key newKey, Node *newNode )
{
    m_mappings.push_back( std::make_pair( Key::Dummy(), oldNode ) );
    m_mappings.push_back( std::make_pair( newKey, newNode ) );
}

//
//
//
size_t InternalNode::insertNodeAfter( Node *oldNode, Key newKey, Node *newNode )
{
    auto iter = m_mappings.begin();
    for (; iter != m_mappings.end() && iter->second != oldNode; ++iter);
    m_mappings.insert( iter + 1, std::make_pair( newKey, newNode ) );
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
    Node* firstChild = m_mappings.front().second;
    m_mappings.pop_back();
    return firstChild;
}

//
//
//
Key InternalNode::replaceAndReturnFirstKey()
{
    Key newKey = m_mappings[ 0 ].first;
    m_mappings[ 0 ].first = Key::Dummy();
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
void InternalNode::copyHalfFrom( std::vector< MappingType > &mappings )
{
    for( size_t i = minSize(); i < mappings.size(); ++i )
    {
        mappings[ i ].second->setParent( this );
        m_mappings.push_back( mappings[ i ] );
    }
}

//
//
//
void InternalNode::moveAllTo( InternalNode *recipient, size_t indexInParent )
{
    m_mappings[ 0 ].first = static_cast< InternalNode* >( parent() )->keyAt( indexInParent );
    recipient->copyAllFrom( m_mappings );
    m_mappings.clear();
}

//
//
//
void InternalNode::copyAllFrom( std::vector< MappingType > &mappings )
{
    for( auto mapping : mappings )
    {
        mapping.second->setParent( this );
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
    static_cast< InternalNode* >( parent() )->setKeyAt( 1, m_mappings.front().first );
}

//
//
//
void InternalNode::copyLastFrom( MappingType pair )
{
    m_mappings.push_back( pair );
    m_mappings.back().second->setParent( this );
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
void InternalNode::copyFirstFrom( MappingType pair, size_t parentIndex )
{
    m_mappings.front().first = static_cast< InternalNode* >( parent() )->keyAt( parentIndex );
    m_mappings.insert( m_mappings.begin(), pair );
    m_mappings.front().first = Key::Dummy();
    m_mappings.front().second->setParent( this );
    static_cast< InternalNode* >( parent() )->setKeyAt( parentIndex, m_mappings.front().first );
}

//
//
//
Node* InternalNode::lookup( Key key ) const
{
    auto locator = m_mappings.begin();
    auto end = m_mappings.end();
    while( locator != end && key >= locator->first )
    {
        ++locator;
    }
    // locator->first is now the least key k such that aKey < k.
    // One before is the greatest key k such that aKey >= k.
    --locator;
    return locator->second;
}

//
//
//
size_t InternalNode::nodeIndex( Node *node ) const
{
    for( size_t i = 0; i < size(); ++i )
    {
        if ( m_mappings[ i ].second == node )
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
    return m_mappings[ index ].second;
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
        keyToTextConverter << std::dec << entry->first.ToString();
        if( verbose )
        {
            keyToTextConverter << "(" << std::hex << entry->second << std::dec << ")";
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
        queue->push( mapping.second );
    }
}
