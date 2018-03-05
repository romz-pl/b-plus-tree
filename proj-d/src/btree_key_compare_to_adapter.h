#ifndef ROMZ_BTREE_KEY_COMPARE_TO_ADAPTER_H
#define ROMZ_BTREE_KEY_COMPARE_TO_ADAPTER_H

#include "btree_is_key_compare_to.h"


namespace btree
{

//
// A helper class to convert a boolean comparison into a three-way
// "compare-to" comparison that returns a negative value to indicate
// less-than, zero to indicate equality and a positive value to
// indicate greater-than. This helper class is specialized for
// less<string> and greater<string>. The btree_key_compare_to_adapter
// class is provided so that btree users automatically get the more
// efficient compare-to code when using common google string types
// with common comparison functors.
//
template< typename Compare >
struct btree_key_compare_to_adapter : Compare
{
    btree_key_compare_to_adapter() { }
    btree_key_compare_to_adapter( const Compare &c ) : Compare(c) { }
    btree_key_compare_to_adapter( const btree_key_compare_to_adapter< Compare > &c )
        : Compare( c )
    {
    }
};

//
//
//
template< >
struct btree_key_compare_to_adapter< std::less< std::string > > : public btree_key_compare_to_tag
{
    btree_key_compare_to_adapter() { }
    btree_key_compare_to_adapter( const std::less< std::string >& ) { }
    btree_key_compare_to_adapter( const btree_key_compare_to_adapter< std::less< std::string > >& ) { }

    int operator()( const std::string &a, const std::string &b ) const
    {
        return a.compare( b );
    }
};

//
//
//
template< >
struct btree_key_compare_to_adapter< std::greater< std::string > > : public btree_key_compare_to_tag
{
    btree_key_compare_to_adapter() { }
    btree_key_compare_to_adapter( const std::greater< std::string >& ) { }
    btree_key_compare_to_adapter( const btree_key_compare_to_adapter< std::greater< std::string > >& ) { }

    int operator()( const std::string &a, const std::string &b ) const
    {
        return b.compare(a);
    }
};

}


#endif

