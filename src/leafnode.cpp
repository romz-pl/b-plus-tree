#include <sstream>
#include "exceptions.h"
#include "internalnode.h"
#include "leafnode.h"

LeafNode::LeafNode(int aOrder)
    : Node(aOrder)
    , fNext{nullptr}
{}

LeafNode::LeafNode(int aOrder, Node* aParent)
    : Node(aOrder, aParent)
    , fNext{nullptr}
{}

LeafNode::~LeafNode()
{
    for (auto mapping : fMappings) {
        delete mapping.second;
    }
}

bool LeafNode::isLeaf() const
{
    return true;
}

LeafNode* LeafNode::next() const
{
    return fNext;
}

void LeafNode::setNext(LeafNode* aNext)
{
    fNext = aNext;
}

size_t LeafNode::size() const
{
    return fMappings.size();
}

size_t LeafNode::minSize() const
{
    return order()/2;
}

size_t LeafNode::maxSize() const
{
    return order() - 1;
}

std::string LeafNode::toString(bool aVerbose) const
{
    std::ostringstream keyToTextConverter;
    if (aVerbose) {
        keyToTextConverter << "[" << std::hex << this << std::dec << "]<" << fMappings.size() << "> ";
    }
    bool first = true;
    for (auto mapping : fMappings) {
        if (first) {
            first = false;
        } else {
            keyToTextConverter << " ";
        }
        keyToTextConverter << mapping.first;
    }
    if (aVerbose) {
        keyToTextConverter << "[" << std::hex << fNext << ">";
    }
    return keyToTextConverter.str();
}

size_t LeafNode::createAndInsertRecord(KeyType aKey, ValueType aValue)
{
    Record* existingRecord = lookup(aKey);
    if (!existingRecord) {
        Record* newRecord = new Record(aValue);
        insert(aKey, newRecord);
    }
    return fMappings.size();
}

void LeafNode::insert(KeyType aKey, Record* aRecord)
{
    auto insertionPoint = fMappings.begin();
    auto end = fMappings.end();
    while (insertionPoint != end && insertionPoint->first < aKey) {
        ++insertionPoint;
    }
    fMappings.insert(insertionPoint, MappingType(aKey, aRecord));
}

Record* LeafNode::lookup(KeyType aKey) const
{
    for (auto mapping : fMappings) {
        if (mapping.first == aKey) {
            return mapping.second;
        }
    }
    return nullptr;
}

void LeafNode::copyRangeStartingFrom(KeyType aKey, std::vector<EntryType>& aVector)
{
    bool found = false;
    for (auto mapping : fMappings) {
        if (mapping.first == aKey) {
            found = true;
        }
        if (found) {
            aVector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
        }
    }
}

void LeafNode::copyRangeUntil(KeyType aKey, std::vector<EntryType>& aVector)
{
    bool found = false;
    for (auto mapping : fMappings) {
        if (!found) {
            aVector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
        }
        if (mapping.first == aKey) {
            found = true;
        }
    }
}

void LeafNode::copyRange(std::vector<EntryType>& aVector)
{
    for (auto mapping : fMappings) {
        aVector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
    }
}


size_t LeafNode::removeAndDeleteRecord(KeyType aKey)
{
    auto removalPoint = fMappings.begin();
    auto end = fMappings.end();
    while (removalPoint != end && removalPoint->first != aKey) {
        ++removalPoint;
    }
    if (removalPoint == end) {
        throw RecordNotFoundException(aKey);
    }
    auto record = *removalPoint;
    fMappings.erase(removalPoint);
    delete record.second;
    return fMappings.size();
}

KeyType LeafNode::firstKey() const
{
    return fMappings[0].first;
}

void LeafNode::moveHalfTo(LeafNode *aRecipient)
{
    aRecipient->copyHalfFrom(fMappings);
    size_t size = fMappings.size();
    for (size_t i = minSize(); i < size; ++i) {
        fMappings.pop_back();
    }
}

void LeafNode::copyHalfFrom(std::vector<std::pair<KeyType, Record*> > &aMappings)
{
    for (size_t i = minSize(); i < aMappings.size(); ++i) {
        fMappings.push_back(aMappings[i]);
    }
}

void LeafNode::moveAllTo(LeafNode *aRecipient, int)
{
    aRecipient->copyAllFrom(fMappings);
    fMappings.clear();
    aRecipient->setNext(next());
}

void LeafNode::copyAllFrom(std::vector<std::pair<KeyType, Record*> > &aMappings)
{
    for (auto mapping : aMappings) {
        fMappings.push_back(mapping);
    }
}

void LeafNode::moveFirstToEndOf(LeafNode* aRecipient)
{
    aRecipient->copyLastFrom(fMappings.front());
    fMappings.erase(fMappings.begin());
    static_cast<InternalNode*>(parent())->setKeyAt(1, fMappings.front().first);
}

void LeafNode::copyLastFrom(MappingType aPair)
{
    fMappings.push_back(aPair);
}

void LeafNode::moveLastToFrontOf(LeafNode *aRecipient, int aParentIndex)
{
    aRecipient->copyFirstFrom(fMappings.back(), aParentIndex);
    fMappings.pop_back();
}

void LeafNode::copyFirstFrom(MappingType aPair, int aParentIndex)
{
    fMappings.insert(fMappings.begin(), aPair);
    static_cast<InternalNode*>(parent())->setKeyAt(aParentIndex, fMappings.front().first);
}
