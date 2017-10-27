#ifndef BPLUSTREE_PRINTER_H
#define BPLUSTREE_PRINTER_H

#include <queue>

class Node;

class Printer
{
public:
    Printer();
    ~Printer() = default;

    bool verbose() const;
    void setVerbose( bool verbose );
    void printTree( Node* root ) const;
    void printLeaves( Node* root );

private:
    void printEmptyTree() const;
    void printNonEmptyTree( Node* root ) const;
    void printCurrentRank( std::queue< Node* >* currentRank, std::queue< Node* >* nextRank ) const;

private:
    bool m_verbose;
};

#endif
