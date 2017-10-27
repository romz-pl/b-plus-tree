#ifndef BPLUSTREE_EXCEPTIONS_H
#define BPLUSTREE_EXCEPTIONS_H

#include <exception>
#include <string>
#include "definitions.h"

//
//
//
class LeafNotFoundException : public std::exception
{
public:
    explicit LeafNotFoundException( Key key );
    const char* what() const noexcept override;

private:
    Key m_key;
};

//
//
//
class NodeNotFoundException : public std::exception
{
public:
    explicit NodeNotFoundException( std::string searchedNode, std::string containingNode );
    const char* what() const noexcept override;

private:
    std::string m_searchedNode;
    std::string m_containingNode;
};

//
//
//
class RecordNotFoundException : public std::exception
{
public:
    explicit RecordNotFoundException( Key key );
    const char* what() const noexcept override;

private:
    Key m_key;
};

#endif

