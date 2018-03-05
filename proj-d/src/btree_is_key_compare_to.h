#ifndef ROMZ_BTREE_IS_KEY_COMPARE_TO_H
#define ROMZ_BTREE_IS_KEY_COMPARE_TO_H

#include <type_traits>

namespace btree
{

//
// A helper type used to indicate that a key-compare-to functor has been
// provided. A user can specify a key-compare-to functor by doing:
//
//  struct MyStringComparer
//      : public util::btree::btree_key_compare_to_tag {
//    int operator()(const string &a, const string &b) const {
//      return a.compare(b);
//    }
//  };
//
// Note that the return type is an int and not a bool. There is a
// COMPILE_ASSERT which enforces this return type.
//
struct btree_key_compare_to_tag
{
};

//
// A helper class that indicates if the Compare parameter is derived from
// btree_key_compare_to_tag.
//
template< typename Compare >
struct btree_is_key_compare_to : public std::is_convertible< Compare, btree_key_compare_to_tag >
{
};

}

#endif
