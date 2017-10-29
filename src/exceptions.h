#ifndef BPLUSTREE_EXCEPTIONS_H
#define BPLUSTREE_EXCEPTIONS_H

#include <exception>
#include <string>
#include "key.h"

//
//
//
class LeafNotFoundException : public std::exception
{
public:
    explicit LeafNotFoundException( const Key& key );
    const char* what() const noexcept override;

private:
    const Key m_key;
};

//
//
//
class NodeNotFoundException : public std::exception
{
public:
    explicit NodeNotFoundException( const std::string& searchedNode, const std::string& containingNode );
    const char* what() const noexcept override;

private:
    const std::string m_searchedNode;
    const std::string m_containingNode;
};

//
//
//
class RecordNotFoundException : public std::exception
{
public:
    explicit RecordNotFoundException( const Key& key );
    const char* what() const noexcept override;

private:
    const Key m_key;
};

#endif

