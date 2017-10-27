#include <sstream>
#include <string>
#include "exceptions.h"
#include "node.h"

LeafNotFoundException::LeafNotFoundException( KeyType aKey )
    : fKey{ aKey }
{

}

const char* LeafNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Key not found in any leaf node:  ";
    ss << fKey;
    static std::string message;
    message = ss.str();
    return message.c_str();
}


NodeNotFoundException::NodeNotFoundException( std::string aSearchedNode, std::string aContainingNode )
    : fSearchedNode{ aSearchedNode }
    , fContainingNode{ aContainingNode }
{

}

const char* NodeNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Node |" << fSearchedNode << "| not found";
    ss << " as a child of node ";
    ss << fContainingNode;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

RecordNotFoundException::RecordNotFoundException( KeyType aKey )
    : fKey{ aKey }
{

}

const char* RecordNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Record not found with key:  " << fKey;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

