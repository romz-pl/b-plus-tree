#include <sstream>
#include "internalnode.h"
#include "leafnode.h"
#include <stdexcept>

//
//
//
LeafNode::LeafNode( size_t order, InternalNode* parent )
    : Node( order, parent )
    , m_next{ nullptr }
{

}

//
//
//
LeafNode::~LeafNode()
{
    for( auto mapping : m_mappings )
    {
        delete mapping.m_record;
    }
}

//
//
//
bool LeafNode::isLeaf() const
{
    return true;
}

//
//
//
LeafNode* LeafNode::next() const
{
    return m_next;
}

//
//
//
void LeafNode::setNext( LeafNode* next )
{
    m_next = next;
}

//
//
//
size_t LeafNode::size() const
{
    return m_mappings.size();
}

//
//
//
size_t LeafNode::minSize() const
{
    return order() / 2;
}

//
//
//
size_t LeafNode::maxSize() const
{
    return order() - 1;
}

//
//
//
std::string LeafNode::toString( bool verbose ) const
{
    std::ostringstream keyToTextConverter;
    if( verbose )
    {
        keyToTextConverter << "[" << std::hex << this << std::dec << "]<" << m_mappings.size() << "> ";
    }

    bool first = true;
    for( auto mapping : m_mappings )
    {
        if( first )
        {
            first = false;
        } else
        {
            keyToTextConverter << " ";
        }
        keyToTextConverter << mapping.m_key.ToString();
    }

    if( verbose )
    {
        keyToTextConverter << "[" << std::hex << m_next << ">";
    }
    return keyToTextConverter.str();
}

//
//
//
size_t LeafNode::createAndInsertRecord( const Key& key, const Value& value )
{
    Record* existingRecord = lookup( key );
    if( !existingRecord )
    {
        Record* newRecord = new Record( value );
        insert( key, newRecord );
    }
    return m_mappings.size();
}

//
//
//
void LeafNode::insert( const Key& key, Record* record )
{
    auto insertionPoint = m_mappings.begin();
    auto end = m_mappings.end();
    while( insertionPoint != end && insertionPoint->m_key < key )
    {
        ++insertionPoint;
    }
    m_mappings.insert( insertionPoint, LeafMapping( key, record ) );
}

//
//
//
Record* LeafNode::lookup( const Key& key ) const
{
    for( auto mapping : m_mappings )
    {
        if( mapping.m_key == key )
        {
            return mapping.m_record;
        }
    }
    return nullptr;
}

//
//
//
size_t LeafNode::removeAndDeleteRecord( const Key& key )
{
    auto removalPoint = m_mappings.begin();
    auto end = m_mappings.end();
    while( removalPoint != end && removalPoint->m_key != key )
    {
        ++removalPoint;
    }

    if( removalPoint == end )
    {
        throw std::runtime_error( "Record not found with key" );
    }

    auto record = *removalPoint;
    m_mappings.erase( removalPoint );
    delete record.m_record;
    return m_mappings.size();
}

//
//
//
Key LeafNode::firstKey() const
{
    return m_mappings[ 0 ].m_key;
}

//
//
//
void LeafNode::moveHalfTo( LeafNode *recipient )
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
void LeafNode::copyHalfFrom( std::vector< LeafMapping > &mappings )
{
    for( size_t i = minSize(); i < mappings.size(); ++i )
    {
        m_mappings.push_back( mappings[ i ] );
    }
}

//
//
//
void LeafNode::moveAllTo( LeafNode *recipient )
{
    recipient->copyAllFrom( m_mappings );
    m_mappings.clear();
    recipient->setNext( next() );
}

//
//
//
void LeafNode::copyAllFrom( std::vector< LeafMapping > &mappings )
{
    for( auto mapping : mappings )
    {
        m_mappings.push_back( mapping );
    }
}

//
//
//
void LeafNode::moveFirstToEndOf( LeafNode* recipient )
{
    recipient->copyLastFrom( m_mappings.front() );
    m_mappings.erase( m_mappings.begin() );
    getParent()->setKeyAt( 1, m_mappings.front().m_key );
}

//
//
//
void LeafNode::copyLastFrom( const LeafMapping &pair )
{
    m_mappings.push_back( pair );
}

//
//
//
void LeafNode::moveLastToFrontOf( LeafNode *recipient, size_t parentIndex )
{
    recipient->copyFirstFrom( m_mappings.back(), parentIndex );
    m_mappings.pop_back();
}

//
//
//
void LeafNode::copyFirstFrom( const LeafMapping &pair, size_t parentIndex )
{
    m_mappings.insert( m_mappings.begin(), pair );
    getParent()->setKeyAt( parentIndex, m_mappings.front().m_key );
}

//
//
//
LeafNode* LeafNode::split( size_t order )
{
    LeafNode* newNode = new LeafNode( order, getParent() );
    moveHalfTo( newNode );
    return newNode;
}


//
//
//
void LeafNode::redistribute( LeafNode* node, size_t index )
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

//
//
//
LeafNode* LeafNode::getLeafNode()
{
    return this;
}
