#include <iostream>
#include "btree_map.h"
#include <map>
#include <cassert>
#include <random>
#include <algorithm>
#include <stdexcept>

using BTreeMap = btree::btree_map< int , std::string >;
using StlMap = std::map< int , std::string >;


void Check();
void CheckInsert( BTreeMap& bMap, StlMap& stlMap, std::vector< int >& key );
void CheckFind( BTreeMap& bMap, StlMap& stlMap, std::vector< int >& key );
void CheckErase( BTreeMap& bMap, StlMap& stlMap, std::vector< int >& key );

std::string GetRandomString();
std::vector< int > GenerateKeys();



//
//
//
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
    catch( ... )
    {
        std::cout << "Caught unknown excpetion.\n" << std::flush;
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
    StlMap stlMap;
    BTreeMap bMap;
    std::vector< int > key = GenerateKeys();

    CheckInsert( bMap, stlMap, key );
    CheckFind( bMap, stlMap, key );
    CheckErase( bMap, stlMap, key );
}

//
// Insert keys
//
void CheckInsert( BTreeMap& bMap, StlMap& stlMap, std::vector< int >& key )
{
    std::cout << "Inserting..." << std::flush;

    // Make key randomly distributed
    std::shuffle( key.begin(), key.end(), std::mt19937{ std::random_device{}() } );

    for( int k : key )
    {
        const std::string value = GetRandomString();

        bMap.insert( std::make_pair( k, value ) );
        stlMap.insert( std::make_pair( k, value ) );
    }

    if( bMap.size() != stlMap.size() )
    {
        throw std::runtime_error( "Error in CheckInsert: Not equal size" );
    }

    std::cout << "OK\n" << std::flush;
}

//
// Check if keys are in B+tree
//
void CheckFind( BTreeMap& bMap, StlMap& stlMap, std::vector< int >& key )
{
    std::cout << "Finding..." << std::flush;

    // Make key randomly distributed
    std::shuffle( key.begin(), key.end(), std::mt19937{ std::random_device{}() } );

    for( int k : key )
    {
        const auto itA = bMap.find( k );
        const auto itB = stlMap.find( k );

        if( *itA != *itB )
        {
            throw std::runtime_error( "Error in CheckGet: not equal values." );
        }
    }

    if( bMap.size() != stlMap.size() )
    {
        throw std::runtime_error( "Error in CheckGet:: Not equal size." );
    }

    std::cout << "OK\n" << std::flush;
}

//
// Delete keys in random order
//
void CheckErase( BTreeMap& bMap, StlMap& stlMap, std::vector< int >& key )
{
    std::cout << "Erasing..." << std::flush;

    // Make key randomly distributed
    std::shuffle( key.begin(), key.end(), std::mt19937{ std::random_device{}() } );

    for( int k : key )
    {
        // std::cout << k << " " << std::flush;

        const auto itA = bMap.find( k );
        const auto itB = stlMap.find( k );

        if( itA->second != itB->second )
        {
            std::cout << itA->second << std::flush << std::endl;
            std::cout << itB->second << std::flush << std::endl;
            throw std::runtime_error( "Error in CheckDelete: not equal values." );
        }

        bMap.erase( k );
        stlMap.erase( k );
    }

    if( bMap.size() != stlMap.size() )
    {
        throw std::runtime_error( "Error in CheckDelete: Not equal size" );
    }

    std::cout << "OK\n" << std::flush;
}


//
// Generate keys
//
std::vector< int > GenerateKeys()
{
    // Number of tested keys.
    // Chenge this value, if needed!
    const int64_t eltNo = 14000;

    std::vector< int > key;
    key.reserve( eltNo );
    for( int64_t i = 0; i < eltNo; i++ )
        key.push_back( i );

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
