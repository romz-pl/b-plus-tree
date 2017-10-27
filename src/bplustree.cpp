#include <iostream>
#include <fstream>
#include <string>
#include "bplustree.h"
#include "exceptions.h"
#include "internalnode.h"
#include "leafnode.h"
#include "node.h"

BPlusTree::BPlusTree( size_t order)
    : m_order{ order }
    , m_root{ nullptr }
{

}

bool BPlusTree::isEmpty() const
{
    return !m_root;
}

// INSERTION

void BPlusTree::insert( KeyType key, ValueType value )
{
    if (isEmpty()) {
        startNewTree(key, value);
    } else {
        insertIntoLeaf(key, value);
    }
}

void BPlusTree::startNewTree(KeyType key, ValueType value) {
    LeafNode* newLeafNode = new LeafNode(m_order);
    newLeafNode->createAndInsertRecord(key, value);
    m_root = newLeafNode;
}

void BPlusTree::insertIntoLeaf(KeyType key, ValueType value)
{
    LeafNode* leafNode = findLeafNode(key);
    if (!leafNode) {
        throw LeafNotFoundException(key);
    }
    size_t newSize = leafNode->createAndInsertRecord(key, value);
    if (newSize > leafNode->maxSize()) {
        LeafNode* newLeaf = split(leafNode);
        newLeaf->setNext(leafNode->next());
        leafNode->setNext(newLeaf);
        KeyType newKey = newLeaf->firstKey();
        insertIntoParent(leafNode, newKey, newLeaf);
    }
}

void BPlusTree::insertIntoParent( Node *oldNode, KeyType key, Node *newNode )
{
    InternalNode* parent = static_cast<InternalNode*>(oldNode->parent());
    if (parent == nullptr) {
        m_root = new InternalNode(m_order);
        parent = static_cast<InternalNode*>(m_root);
        oldNode->setParent(parent);
        newNode->setParent(parent);
        parent->populateNewRoot(oldNode, key, newNode);
    } else {
        size_t newSize = parent->insertNodeAfter(oldNode, key, newNode);
        if (newSize > parent->maxSize()) {
            InternalNode* newNode = split(parent);
            KeyType newKey = newNode->replaceAndReturnFirstKey();
            insertIntoParent(parent, newKey, newNode);
        }
    }
}

template <typename T>
T* BPlusTree::split(T* node)
{
    T* newNode = new T(m_order, node->parent());
    node->moveHalfTo(newNode);
    return newNode;
}


// REMOVAL


void BPlusTree::remove(KeyType key)
{
    if (isEmpty()) {
        return;
    } else {
        removeFromLeaf(key);
    }
}

void BPlusTree::removeFromLeaf( KeyType key )
{
    LeafNode* leafNode = findLeafNode(key);
    if (!leafNode) {
        return;
    }
    if (!leafNode->lookup(key)) {
        return;
    }
    size_t newSize = leafNode->removeAndDeleteRecord(key);
    if (newSize < leafNode->minSize()) {
        coalesceOrRedistribute(leafNode);
    }
}

template < typename N >
void BPlusTree::coalesceOrRedistribute( N* node )
{
    if (node->isRoot()) {
        adjustRoot();
        return;
    }
    auto parent = static_cast<InternalNode*>(node->parent());
    int indexOfNodeInParent = parent->nodeIndex(node);
    int neighborIndex = (indexOfNodeInParent == 0) ? 1 : indexOfNodeInParent - 1;
    N* neighborNode = static_cast<N*>(parent->neighbor(neighborIndex));
    if (node->size() + neighborNode->size() <= neighborNode->maxSize()) {
        coalesce(neighborNode, node, parent, indexOfNodeInParent);
    } else {
        redistribute(neighborNode, node, parent, indexOfNodeInParent);
    }
}

template < typename N >
void BPlusTree::coalesce( N* neighborNode, N* node, InternalNode* parent, int index )
{
    if (index == 0) {
        std::swap(node, neighborNode);
        index = 1;
    }
    node->moveAllTo(neighborNode, index);
    parent->remove(index);
    if (parent->size() < parent->minSize()) {
        coalesceOrRedistribute(parent);
    }
    delete node;
}

template < typename N >
void BPlusTree::redistribute( N* neighborNode, N* node, InternalNode* /*aParent*/, int index )
{
    if (index == 0) {
        neighborNode->moveFirstToEndOf(node);
    } else {
        neighborNode->moveLastToFrontOf(node, index);
    }
}

void BPlusTree::adjustRoot()
{
    if (!m_root->isLeaf() && m_root->size() == 1) {
        auto discardedNode = static_cast<InternalNode*>(m_root);
        m_root = static_cast<InternalNode*>(m_root)->removeAndReturnOnlyChild();
        m_root->setParent(nullptr);
        delete discardedNode;
    } else if (!m_root->size()){
        delete m_root;
        m_root = nullptr;
    }
}


// UTILITIES AND PRINTING

LeafNode* BPlusTree::findLeafNode( KeyType key, bool printing, bool verbose )
{
    if (isEmpty()) {
        if (printing) {
            std::cout << "Not found: empty tree." << std::endl;
        }
        return nullptr;
    }
    auto node = m_root;
    if (printing) {
        std::cout << "Root: ";
        if (m_root->isLeaf()) {
            std::cout << "\t" << static_cast<LeafNode*>(m_root)->toString(verbose);
        } else {
            std::cout << "\t" << static_cast<InternalNode*>(m_root)->toString(verbose);
        }
        std::cout << std::endl;
    }
    while (!node->isLeaf()) {
        auto internalNode = static_cast<InternalNode*>(node);
        if (printing && node != m_root) {
            std::cout << "\tNode: " << internalNode->toString(verbose) << std::endl;
        }
        node = internalNode->lookup(key);
    }
    return static_cast<LeafNode*>(node);
}

void BPlusTree::readInputFromFile( std::string fileName )
{
    int key;
    std::ifstream input(fileName);
    while (input) {
        input >> key;
        insert(key, key);
    }
}

void BPlusTree::print( bool verbose )
{
    m_printer.setVerbose(verbose);
    m_printer.printTree(m_root);
}

void BPlusTree::printLeaves( bool verbose )
{
    m_printer.setVerbose(verbose);
    m_printer.printLeaves(m_root);
}

void BPlusTree::destroyTree()
{
    if (m_root->isLeaf()) {
        delete static_cast<LeafNode*>(m_root);
    } else {
        delete static_cast<InternalNode*>(m_root);
    }
    m_root = nullptr;
}

void BPlusTree::printValue( KeyType key, bool verbose )
{
    printValue(key, false, verbose);
}

void BPlusTree::printValue( KeyType key, bool printPath, bool verbose )
{
    LeafNode* leaf = findLeafNode(key, printPath, verbose);
    if (!leaf) {
        std::cout << "Leaf not found with key " << key << "." << std::endl;
        return;
    }
    if (printPath) {
        std::cout << "\t";
    }
    std::cout << "Leaf: " << leaf->toString(verbose) << std::endl;
    Record* record = leaf->lookup(key);
    if (!record) {
        std::cout << "Record not found with key " << key << "." << std::endl;
        return;
    }
    if (printPath) {
        std::cout << "\t";
    }
    std::cout << "Record found at location " << std::hex << record << std::dec << ":" << std::endl;
    std::cout << "\tKey: " << key << "   Value: " << record->value() << std::endl;
}

void BPlusTree::printPathTo( KeyType key, bool verbose )
{
    printValue(key, true, verbose);
}

void BPlusTree::printRange( KeyType start, KeyType end )
{
    auto rangeVector = range(start, end);
    for (auto entry : rangeVector) {
        std::cout << "Key: " << std::get<0>(entry);
        std::cout << "    Value: " << std::get<1>(entry);
        std::cout << "    Leaf: " << std::hex << std::get<2>(entry) << std::dec << std::endl;
    }
}

std::vector<BPlusTree::EntryType> BPlusTree::range( KeyType start, KeyType end )
{
    auto startLeaf = findLeafNode(start);
    auto endLeaf = findLeafNode(end);
    std::vector<std::tuple<KeyType, ValueType, LeafNode*>> entries;
    if (!startLeaf || !endLeaf) {
        return entries;
    }
    startLeaf->copyRangeStartingFrom(start, entries);
    startLeaf = startLeaf->next();
    while (startLeaf != endLeaf) {
        startLeaf->copyRange(entries);
        startLeaf = startLeaf->next();
    }
    startLeaf->copyRangeUntil(end, entries);
    return entries;
}
