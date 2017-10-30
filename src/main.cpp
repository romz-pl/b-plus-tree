#include <iostream>
#include "bplustree.h"
#include <map>
#include <cassert>
#include <random>
#include <algorithm>
#include <stdexcept>

void Check();
void CheckInsert( BPlusTree& bPlusTree, std::map< Key, Value >& stlMap, std::vector< Key >& key );
void CheckGet( BPlusTree& bPlusTree, std::map< Key, Value >& stlMap, std::vector< Key >& key );
void CheckDelete( BPlusTree& bPlusTree, std::map< Key, Value >& stlMap, std::vector< Key >& key );

std::string GetRandomString();
std::vector< Key > GenerateKeys();
BPlusTree CreateBPlusTree();


bool operator==( const Value& a, const Value& b )
{
    return a.ToString() == b.ToString();
}

bool operator!=( const Value& a, const Value& b )
{
    return !( a == b);
}


int main()
{
    try
    {
        Check();
    }
    catch( std::exception& e )
    {
        std::cout << e.what() << std::endl << std::flush;
        return 1;
    }

    std::cout << "All test passed!\n" << std::flush;
    return 0;
}

//
//
//
void Check()
{
    std::map< Key, Value > stlMap;
    BPlusTree bPlusTree = CreateBPlusTree();
    std::vector< Key > key = GenerateKeys();

    CheckInsert( bPlusTree, stlMap, key );
    CheckGet( bPlusTree, stlMap, key );
    CheckDelete( bPlusTree, stlMap, key );
}

//
// Insert keys
//
void CheckInsert( BPlusTree& bPlusTree, std::map< Key, Value >& stlMap, std::vector< Key >& key )
{
    std::cout << "Inserting..." << std::flush;

    // Make key randomly distributed
    std::shuffle( key.begin(), key.end(), std::mt19937{ std::random_device{}() } );

    for( Key k : key )
    {
        const Value value = Value( GetRandomString() );
        // std::cout << k.ToString() << " " << value.ToString() << std::endl << std::flush;

        bPlusTree.insert( k, value );
        stlMap.insert( std::make_pair( k, value ) );
    }

    if( bPlusTree.count() != stlMap.size() )
    {
        throw std::runtime_error( "Error in CheckInsert: Not equal size" );
    }

    std::cout << "OK\n" << std::flush;
}

//
// Check if keys are in B+tree
//
void CheckGet( BPlusTree& bPlusTree, std::map< Key, Value >& stlMap, std::vector< Key >& key )
{
    std::cout << "Getting..." << std::flush;

    // Make key randomly distributed
    std::shuffle( key.begin(), key.end(), std::mt19937{ std::random_device{}() } );

    for( Key k : key )
    {
        const Value valueA = bPlusTree.get( k );
        const Value valueB = stlMap.at( k );

        if( valueA != valueB )
        {
            throw std::runtime_error( "Error in CheckGet: not equal values." );
        }
    }

    if( bPlusTree.count() != stlMap.size() )
    {
        throw std::runtime_error( "Error in CheckGet:: Not equal size." );
    }

    std::cout << "OK\n" << std::flush;
}

//
// Delete keys in random order
//
void CheckDelete( BPlusTree& bPlusTree, std::map< Key, Value >& stlMap, std::vector< Key >& key )
{
    std::cout << "Deleting..." << std::flush;

    // Make key randomly distributed
    std::shuffle( key.begin(), key.end(), std::mt19937{ std::random_device{}() } );

    for( Key k : key )
    {
        std::cout << k.ToString() << " " << std::flush;

        const Value valueA = bPlusTree.get( k );
        bPlusTree.remove( k );

        const Value valueB = stlMap.at( k );
        stlMap.erase( k );

        if( valueA != valueB )
        {
            throw std::runtime_error( "Error in CheckDelete: not equal values." );
        }
    }

    if( bPlusTree.count() != stlMap.size() )
    {
        throw std::runtime_error( "Error in CheckDelete: Not equal size" );
    }

    std::cout << "OK\n" << std::flush;
}

//
//
//
BPlusTree CreateBPlusTree()
{
    const size_t order = 4;
    BPlusTree bPlusTree( order );
    return bPlusTree;
}

//
// Generate keys
//
std::vector< Key > GenerateKeys()
{
    // Number of tested keys.
    // Chenge this value, if needed!
    const int64_t eltNo = 1400;

    std::vector< Key > key;
    key.reserve( eltNo );
    for( int64_t i = 0; i < eltNo; i++ )
        key.push_back( Key( i ) );

    // Make key randomly distributed
    std::shuffle( key.begin(), key.end(), std::mt19937{ std::random_device{}() } );

    return key;

}


//
// Returns randomply generated string.
// The length of the string is random, and not longer then the length of he alphabet.
//
std::string GetRandomString()
{
    const std::string alphabet( "0123456789"
                                "abcdefghijklmnopqrstuvwxyz"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );

    std::random_device rd;
    std::mt19937 g( rd() );
    std::uniform_int_distribution< std::string::size_type > pick( 0, alphabet.size() );

    std::string::size_type length = pick( g );
    std::string s;
    s.reserve( length );

    while( length-- )
    {
        const char c = alphabet[ pick( g ) ];
        s += c;
    }
    return s;
}

