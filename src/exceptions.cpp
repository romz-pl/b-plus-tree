#include <sstream>
#include <string>
#include "exceptions.h"
#include "node.h"

//
//
//
LeafNotFoundException::LeafNotFoundException( KeyType key )
    : m_key{ key }
{

}

//
//
//
const char* LeafNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Key not found in any leaf node:  ";
    ss << m_key;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

//
//
//
NodeNotFoundException::NodeNotFoundException( std::string searchedNode, std::string containingNode )
    : m_searchedNode{ searchedNode }
    , m_containingNode{ containingNode }
{

}

//
//
//
const char* NodeNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Node |" << m_searchedNode << "| not found";
    ss << " as a child of node ";
    ss << m_containingNode;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

//
//
//
RecordNotFoundException::RecordNotFoundException( KeyType key )
    : m_key{ key }
{

}

//
//
//
const char* RecordNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Record not found with key:  " << m_key;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

