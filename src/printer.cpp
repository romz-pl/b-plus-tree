#include <iostream>
#include "InternalNode.hpp"
#include "leafnode.h"
#include "node.h"
#include "printer.h"

Printer::Printer() : fVerbose(false) {}

bool Printer::verbose() const
{
    return fVerbose;
}

void Printer::setVerbose(bool aVerbose)
{
    fVerbose = aVerbose;
}

void Printer::printTree(Node *aRoot) const
{
    if (!aRoot) {
        printEmptyTree();
    } else {
        printNonEmptyTree(aRoot);
    }
}

void Printer::printEmptyTree() const
{
    std::cout << "Empty tree." << std::endl;
}

void Printer::printNonEmptyTree(Node *aRoot) const
{
    std::queue<Node*> queue0;
    std::queue<Node*> queue1;
    auto currentRank = &queue0;
    auto nextRank = &queue1;
    currentRank->push(aRoot);
    while (!currentRank->empty()) {
        printCurrentRank(currentRank, nextRank);
        auto tmp = currentRank;
        currentRank = nextRank;
        nextRank = tmp;
    }
}

void Printer::printCurrentRank(std::queue<Node*>* aCurrentRank, std::queue<Node*>* aNextRank) const
{
    std::cout << "|";
    while(!aCurrentRank->empty()) {
        Node* currentNode = aCurrentRank->front();
        std::cout << " " << currentNode->toString(verbose());
        std::cout << " |";
        if (!currentNode->isLeaf()) {
            auto internalNode = static_cast<InternalNode*>(currentNode);
            internalNode->queueUpChildren(aNextRank);
        }
        aCurrentRank->pop();
    }
    std::cout << std::endl;
}

void Printer::printLeaves(Node *aRoot)
{
    if (!aRoot) {
        printEmptyTree();
        return;
    }
    auto node = aRoot;
    while (!node->isLeaf()) {
        node = static_cast<InternalNode*>(node)->firstChild();
    }
    auto leafNode = static_cast<LeafNode*>(node);
    while (leafNode) {
        std::cout << "| ";
        std::cout << leafNode->toString(fVerbose);
        leafNode = leafNode->next();
    }
    std::cout << " |" << std::endl;
}
