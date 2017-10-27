#include <sstream>
#include "exceptions.h"
#include "internalnode.h"
#include "leafnode.h"

LeafNode::LeafNode( size_t order )
    : Node( order )
    , m_next{ nullptr }
{

}

LeafNode::LeafNode( size_t order, Node* parent )
    : Node( order, parent )
    , m_next{ nullptr }
{

}

LeafNode::~LeafNode()
{
    for (auto mapping : m_mappings) {
        delete mapping.second;
    }
}

bool LeafNode::isLeaf() const
{
    return true;
}

LeafNode* LeafNode::next() const
{
    return m_next;
}

void LeafNode::setNext( LeafNode* next )
{
    m_next = next;
}

size_t LeafNode::size() const
{
    return m_mappings.size();
}

size_t LeafNode::minSize() const
{
    return order() / 2;
}

size_t LeafNode::maxSize() const
{
    return order() - 1;
}

std::string LeafNode::toString( bool verbose ) const
{
    std::ostringstream keyToTextConverter;
    if (verbose) {
        keyToTextConverter << "[" << std::hex << this << std::dec << "]<" << m_mappings.size() << "> ";
    }
    bool first = true;
    for (auto mapping : m_mappings) {
        if (first) {
            first = false;
        } else {
            keyToTextConverter << " ";
        }
        keyToTextConverter << mapping.first;
    }
    if (verbose) {
        keyToTextConverter << "[" << std::hex << m_next << ">";
    }
    return keyToTextConverter.str();
}

size_t LeafNode::createAndInsertRecord( KeyType key, ValueType value )
{
    Record* existingRecord = lookup(key);
    if (!existingRecord) {
        Record* newRecord = new Record(value);
        insert(key, newRecord);
    }
    return m_mappings.size();
}

void LeafNode::insert( KeyType key, Record* record )
{
    auto insertionPoint = m_mappings.begin();
    auto end = m_mappings.end();
    while (insertionPoint != end && insertionPoint->first < key) {
        ++insertionPoint;
    }
    m_mappings.insert(insertionPoint, MappingType( key, record ));
}

Record* LeafNode::lookup( KeyType key ) const
{
    for (auto mapping : m_mappings) {
        if (mapping.first == key) {
            return mapping.second;
        }
    }
    return nullptr;
}

void LeafNode::copyRangeStartingFrom( KeyType key, std::vector<EntryType>& vector )
{
    bool found = false;
    for (auto mapping : m_mappings) {
        if (mapping.first == key) {
            found = true;
        }
        if (found) {
            vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
        }
    }
}

void LeafNode::copyRangeUntil( KeyType key, std::vector<EntryType>& vector )
{
    bool found = false;
    for (auto mapping : m_mappings) {
        if (!found) {
            vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
        }
        if (mapping.first == key) {
            found = true;
        }
    }
}

void LeafNode::copyRange( std::vector< EntryType >& vector )
{
    for (auto mapping : m_mappings) {
        vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
    }
}


size_t LeafNode::removeAndDeleteRecord( KeyType key )
{
    auto removalPoint = m_mappings.begin();
    auto end = m_mappings.end();
    while (removalPoint != end && removalPoint->first != key ) {
        ++removalPoint;
    }
    if (removalPoint == end) {
        throw RecordNotFoundException(key );
    }
    auto record = *removalPoint;
    m_mappings.erase(removalPoint);
    delete record.second;
    return m_mappings.size();
}

KeyType LeafNode::firstKey() const
{
    return m_mappings[0].first;
}

void LeafNode::moveHalfTo( LeafNode *recipient )
{
    recipient->copyHalfFrom(m_mappings);
    size_t size = m_mappings.size();
    for (size_t i = minSize(); i < size; ++i) {
        m_mappings.pop_back();
    }
}

void LeafNode::copyHalfFrom( std::vector< std::pair< KeyType, Record* > > &mappings )
{
    for (size_t i = minSize(); i < mappings.size(); ++i) {
        m_mappings.push_back(mappings[i]);
    }
}

void LeafNode::moveAllTo( LeafNode *recipient, int )
{
    recipient->copyAllFrom(m_mappings);
    m_mappings.clear();
    recipient->setNext(next());
}

void LeafNode::copyAllFrom( std::vector< std::pair< KeyType, Record* > > &mappings )
{
    for (auto mapping : mappings) {
        m_mappings.push_back(mapping);
    }
}

void LeafNode::moveFirstToEndOf( LeafNode* recipient )
{
    recipient->copyLastFrom(m_mappings.front());
    m_mappings.erase(m_mappings.begin());
    static_cast<InternalNode*>(parent())->setKeyAt(1, m_mappings.front().first);
}

void LeafNode::copyLastFrom( MappingType pair )
{
    m_mappings.push_back(pair);
}

void LeafNode::moveLastToFrontOf( LeafNode *recipient, int parentIndex)
{
    recipient->copyFirstFrom(m_mappings.back(), parentIndex);
    m_mappings.pop_back();
}

void LeafNode::copyFirstFrom( MappingType pair, int parentIndex )
{
    m_mappings.insert(m_mappings.begin(), pair);
    static_cast<InternalNode*>(parent())->setKeyAt(parentIndex, m_mappings.front().first);
}
