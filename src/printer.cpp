#include <iostream>
#include "internalnode.h"
#include "leafnode.h"
#include "node.h"
#include "printer.h"

//
//
//
Printer::Printer()
    : m_verbose( false )
{

}

//
//
//
bool Printer::verbose() const
{
    return m_verbose;
}

//
//
//
void Printer::setVerbose( bool verbose )
{
    m_verbose = verbose;
}

//
//
//
void Printer::printTree( Node *root ) const
{
    if( !root )
    {
        printEmptyTree();
    }
    else
    {
        printNonEmptyTree( root );
    }
}

//
//
//
void Printer::printEmptyTree() const
{
    std::cout << "Empty tree." << std::endl;
}

//
//
//
void Printer::printNonEmptyTree( Node *root ) const
{
    std::queue< Node* > queue0;
    std::queue< Node* > queue1;
    auto currentRank = &queue0;
    auto nextRank = &queue1;
    currentRank->push( root );
    while( !currentRank->empty() )
    {
        printCurrentRank( currentRank, nextRank );
        auto tmp = currentRank;
        currentRank = nextRank;
        nextRank = tmp;
    }
}

//
//
//
void Printer::printCurrentRank( std::queue< Node* >* currentRank, std::queue< Node* >* nextRank ) const
{
    std::cout << "|";
    while( !currentRank->empty() )
    {
        Node* currentNode = currentRank->front();
        std::cout << " " << currentNode->toString( verbose() );
        std::cout << " |";
        if( !currentNode->isLeaf() )
        {
            auto internalNode = static_cast< InternalNode* >( currentNode );
            internalNode->queueUpChildren( nextRank );
        }
        currentRank->pop();
    }
    std::cout << std::endl;
}

//
//
//
void Printer::printLeaves( Node *root )
{
    if( !root )
    {
        printEmptyTree();
        return;
    }

    auto node = root;
    while( !node->isLeaf() )
    {
        node = static_cast< InternalNode* >( node )->firstChild();
    }

    auto leafNode = static_cast< LeafNode* >( node );
    while( leafNode )
    {
        std::cout << "| ";
        std::cout << leafNode->toString( m_verbose );
        leafNode = leafNode->next();
    }
    std::cout << " |" << std::endl;
}
